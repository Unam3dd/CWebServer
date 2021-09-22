#pragma once
#include <stdint.h>
#include <arpa/inet.h>

#define MAX_HEADER_SIZE 0x2000

typedef struct http_server_t http_server_t;
typedef struct http_client_t http_client_t;
typedef struct http_buffer_t http_buffer_t;
typedef struct http_header_t http_header_t;
typedef struct http_request_t http_request_t;

struct http_buffer_t 
{
    char buf[MAX_HEADER_SIZE];
    int pos;
};

struct http_client_t
{
    http_buffer_t buffer;
    struct sockaddr_in sin;
    int tfd;
    int fd;
};

struct http_server_t
{
    uint8_t (*start)(http_server_t *);
    uint8_t (*accept)(http_server_t *, http_client_t *);
    void (*wait)(http_server_t *);
    void (*handle)(http_server_t *, http_client_t *);
    void (*close)(http_server_t *);
    char *addr;
    int server_fd;
    int epoll_fd;
    uint16_t port;
    uint8_t use_default_callbacks : 1;
};

struct http_header_t
{
    http_header_t *next;
    http_header_t *prev;
    char *key;
    char *value;
};

struct http_request_t
{
    http_header_t *headers;
    char *method;
    char *uri;
    char *version;
};