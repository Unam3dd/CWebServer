#pragma once
#include <stdint.h>
#include <arpa/inet.h>

typedef struct http_server_t http_server_t;
typedef struct http_client_t http_client_t;

struct http_server_t
{
    char *addr;
    int server_fd;
    int epoll_fd;
    uint16_t port;
    uint16_t family;
};

struct http_client_t
{
    struct sockaddr_in sin;
    int fd;
};