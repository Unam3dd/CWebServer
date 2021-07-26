#include "http_server.h"
#include "log.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handle_signal(int sig)
{
    signal(sig, SIG_IGN);
    usleep(2000);
    print_log("[%t] wait moment....\n");
    run = 0;
}

uint8_t create_http_server(http_server_t *server)
{
    struct sockaddr_in socks = {0};
    struct epoll_event epoll = {0};

    server->server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server->server_fd < 0)
        return (1);
    
    socks.sin_addr.s_addr = inet_addr(server->addr);
    socks.sin_port = htons(server->port);
    socks.sin_family = server->family;

    if (setsockopt(server->server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)))
        return (1);

    if (bind(server->server_fd, (struct sockaddr *)&socks, sizeof(socks)) < 0)
        return (1);
    
    if (listen(server->server_fd, SOMAXCONN) < 0)
        return (1);
    
    server->epoll_fd = epoll_create1(0);

    if (server->epoll_fd < 0)
        return (1);
    
    epoll.data.fd = server->server_fd;
    epoll.events = EPOLLIN;

    if (epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, server->server_fd, &epoll) < 0)
        return (1);
    
    print_log("[%t] Server on  %s:%d !\n", server->addr, server->port);

    return (0);
}

uint8_t close_http_server(http_server_t *server)
{
    if (server->epoll_fd)
        close(server->epoll_fd);
    
    if (server->server_fd)
        close(server->server_fd);

    print_log("[%t] Server closed %s:%d !\n", server->addr, server->port);
    
    return (0);
}

uint8_t wait_http_server(http_server_t *server)
{
    struct epoll_event evs[EVENTS_MAX] = {0};

    int ev_fd = 0, current_fd = 0;

    signal(SIGINT, handle_signal);

    while (run) {
        ev_fd = epoll_wait(server->epoll_fd, evs, EVENTS_MAX, -1);

        
        for (current_fd = 0; current_fd < ev_fd; current_fd++) {

            if (evs[current_fd].data.fd == server->server_fd) {

                // Handle accept incoming connection
                http_client_t *client_ptr = create_new_http_client();

                if (accept_http_server(server, client_ptr)) {
                    print_error();
                    delete_http_client(client_ptr);
                }
                
            } else
                handle_http_server(server, (http_client_t *)evs[current_fd].data.ptr);
        }
    }

    return (0);
}

uint8_t accept_http_server(http_server_t *server, http_client_t *client)
{
    struct epoll_event event = {0};

    socklen_t size_sin = sizeof(client->sin);

    if ((client->fd = accept(server->server_fd, (struct sockaddr *)&client->sin, &size_sin)) < 0)
        return (1);
    
    event.data.fd = client->fd;
    event.data.ptr = (http_client_t *)client;
    event.events = EPOLLIN;

    if (epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, client->fd, &event) < 0)
        return (1);
    
    print_log("[%t] New incoming connection %s:%d !\n", inet_ntoa(client->sin.sin_addr), ntohs(client->sin.sin_port));

    return (0);
}

uint8_t handle_http_server(http_server_t *server, http_client_t *client)
{
    print_log("[%t] Processing of %s:%d !\n", inet_ntoa(client->sin.sin_addr), ntohs(client->sin.sin_port));

    if (fd_is_readable(server, client) == 0) {
        close_connection(client);
        return (1);
    }

    if (handle_read(server, client) < 0) {
        print_log("[%t] error in handle_read()\n");
        return (1);
    }

    http_request_t req;
    http_response_t res;

    if (parse_request(&client->data, &req)) {
        print_log("[%t] error in parse_request()\n");
        return (1);
    }

    if (parse_headers(&client->data, &req.headers)) {
        print_log("[%t] error in parse_headers()\n");
        return (1);
    }

    parse_body(&client->data, &req);

    for (uint8_t i = 0; http_callbacks[i].method; i++) {

        if (strcmp(req.method, http_callbacks[i].method) == 0)
            http_callbacks[i].callback(client, &req, &res);
    }

    free_headers(&req.headers);

    return (0);
}