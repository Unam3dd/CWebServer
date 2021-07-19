#include "http_server.h"
#include "log.h"
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>


http_client_t *create_new_http_client(void)
{
    http_client_t *client = (http_client_t *)malloc(sizeof(http_client_t));

    return (client);
}

void delete_http_client(http_client_t *client)
{
    free(client);
    client = NULL;
}

void close_connection(http_client_t *client)
{
    close(client->fd);
    print_log("[%t] Connection closed %s:%d !\n", inet_ntoa(client->sin.sin_addr), ntohs(client->sin.sin_port));
    delete_http_client(client);
}