#pragma once

#include <stdint.h>
#include <arpa/inet.h>

#define HTTP_HEADER_SIZE_DEFAULT 0x2000
#define HTTP_VERSION_1_1 "HTTP/1.1"

#define HTTP_BAD_VERSION 0x1
#define HTTP_FILE_NOT_FOUND 0x2

typedef struct http_server_t http_server_t;
typedef struct http_client_t http_client_t;
typedef struct http_buffer_t http_buffer_t;
typedef struct http_request_t http_request_t;
typedef struct http_callback_t http_callback_t;
typedef struct http_response_t http_response_t;

struct http_buffer_t
{
    char buffer[HTTP_HEADER_SIZE_DEFAULT];
    int pos;
};

struct http_server_t
{
    char *root;
    char *addr;
    int server_fd;
    int epoll_fd;
    uint16_t port;
    uint16_t family;
};

typedef struct http_headers
{
    struct http_headers *next;
    struct http_headers *prev;
    char *key;
    char *value;
} http_headers_t;

struct http_request_t
{
    http_headers_t *headers;
    char *method;
    char *uri;
    char *http_version;
    char *body;
};

struct http_client_t
{
    http_buffer_t data;
    struct sockaddr_in sin;
    int fd;
};

struct http_response_t
{
    http_headers_t *headers;
    char *http_version;
    char *status;
    char *status_code;
    char *content;
    uint64_t content_size;
    uint8_t error_code;
};

struct http_callback_t
{
    char *method;
    void (*callback)(http_client_t *, http_request_t *, http_response_t *);
};

extern const http_callback_t http_callbacks[];