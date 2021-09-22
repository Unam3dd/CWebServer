#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "http_server.h"
#include "log.h"

///////////////////////////////////////////////
//
//          SIGNAL HANDLER
//
///////////////////////////////////////////////

void handle_signal(int sig)
{
    // ingore signal
    signal(sig, SIG_IGN);

    usleep(2000);

    print_log("[%t] Closing server...wait moment !\n");
    
    run = 0;
}

void http_server_initialize(http_server_t *server)
{
    server->start = &http_server_init;
    server->accept = &http_server_accept;
    server->wait = &http_server_wait;
    server->handle = &http_server_handle;
    server->close = &http_server_close;
}

void http_server_destroy(http_server_t *server)
{
    server->server_fd = -1;
    server->epoll_fd  = -1;
    server->start = NULL;
    server->wait = NULL;
    server->handle = NULL;
    server->accept = NULL;
    server->addr = NULL;
    server->port = 0;
    server->close = NULL;
}

uint8_t http_server_close_client(http_server_t *server, http_client_t *client)
{
    if (client->tfd)
        close(client->tfd);
    
    if (client->fd)
        close(client->fd);
    
    print_log("[%t] Client connection closed  %s:%d\n", inet_ntoa(client->sin.sin_addr), ntohs(client->sin.sin_port));

    memset(client->buffer.buf, 0, sizeof(client->buffer.buf));
    client->buffer.pos = 0;
    
    free_client(client);

    return (0);
}

///////////////////////////////////////////////
//
//          HTTP SERVER CALLBACK
//
///////////////////////////////////////////////


uint8_t http_server_init(http_server_t *server)
{
    struct sockaddr_in socks = {0};
    struct epoll_event event = {0};

    if ((server->server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        return (1);
    
    // Convert address to network byte order
    socks.sin_addr.s_addr = inet_addr(server->addr);
    socks.sin_port = htons(server->port);
    socks.sin_family = AF_INET;

    if (setsockopt(server->server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
        return (1);

    if (bind(server->server_fd, (struct sockaddr *)&socks, sizeof(socks)) < 0)
        return (1);
    
    if (listen(server->server_fd, SOMAXCONN) < 0)
        return (1);
    
    if ((server->epoll_fd = epoll_create1(0)) < 0)
        return (1);
    
    event.data.fd = server->server_fd;
    event.events = EPOLLIN;
    
    if (epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, server->server_fd, &event) < 0)
        return (1);
    
    print_log("[%t] Listening on %s:%d\n", server->addr, server->port);

    return (0);
}

void http_server_close(http_server_t *server)
{
    if (server->epoll_fd)
        close(server->epoll_fd);
    
    if (server->server_fd)
        close(server->server_fd);
    
    print_log("[%t] Server Closed %s:%d\n", server->addr, server->port);
}

void http_server_wait(http_server_t *server)
{
    struct epoll_event ev[EPOLL_EVENTS] = {0};
    http_client_t *client_ptr = NULL;

    int event_fd, current_fd;

    signal(SIGINT, handle_signal);

    while (run) {
        event_fd = epoll_wait(server->epoll_fd, ev, EPOLL_EVENTS, -1);

        for (current_fd = 0; current_fd < event_fd; current_fd++) {

            if (ev[current_fd].data.fd == server->server_fd) {

                http_client_t *client = new_client();
                
                if (server->accept(server, client))
                    print_error();

            } else {
                http_client_t *ptr_data = (http_client_t *)ev[current_fd].data.ptr;

                if (ev[current_fd].data.fd == ptr_data->tfd)
                    http_server_close_client(server, ptr_data);
                else
                    server->handle(server, ptr_data);
            }
        }
    }

    for (current_fd = 0; ev[current_fd].data.ptr; current_fd++) {
        
        if (ev[current_fd].data.fd == server->server_fd)
            continue;
        
        client_ptr = (http_client_t *)ev[current_fd].data.ptr;

        http_server_close_client(server, client_ptr);
    }
}

uint8_t http_server_accept(http_server_t *server, http_client_t *client)
{
    struct epoll_event ev;
    
    struct itimerspec timer = {
        .it_interval = {30, 0},
        .it_value = {30, 0},
    };

    if ((client->fd = accept(server->server_fd, (struct sockaddr *)&client->sin, &(socklen_t){sizeof(client->sin)})) < 0)
        return (1);
    
    if ((client->tfd = timerfd_create(CLOCK_MONOTONIC, 0)) < 0)
        return (1);
    
    timerfd_settime(client->tfd, 0, &timer, NULL);
    
    ev.events = EPOLLIN;
    ev.data.fd = client->fd;
    ev.data.ptr = (http_client_t *)client;

    if (epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, client->fd, &ev) < 0)
        return (1);

    ev.events = EPOLLIN;
    ev.data.fd = client->tfd;
    ev.data.ptr = (http_client_t *)client;
    
    if (epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, client->tfd, &ev) < 0)
        return (1);
    
    print_log("[%t] New incoming connection %s:%d\n", inet_ntoa(client->sin.sin_addr), ntohs(client->sin.sin_port));
    
    return (0);
}

void http_server_handle(http_server_t *server, http_client_t *client)
{
    if (fd_is_readable(client)) {
        http_server_close_client(server, client);
        return;
    }

    http_request_t req;

    size_t len = read(client->fd, client->buffer.buf, MAX_HEADER_SIZE);
    
    client->buffer.buf[len] = 0;

    if (parse_request(&client->buffer, &req)) {
        print_log("[-] Error parsing request !\n");
        return;
    }



    // HANDLE REQUEST

    
    

    free_http_headers(&req.headers);
}