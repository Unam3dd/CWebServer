#pragma once
#include <stdint.h>
#include "http_type.h"
#include "http_error.h"

#define EVENTS_MAX 16

static volatile int run = 1;

// server.c
void handle_signal(int sig);
uint8_t create_http_server(http_server_t *server);
uint8_t close_http_server(http_server_t *server);
uint8_t wait_http_server(http_server_t *server);
uint8_t accept_http_server(http_server_t *server, http_client_t *client);
uint8_t handle_http_server(http_server_t *server, http_client_t *client);

// client.c

http_client_t *create_new_http_client(void);
void delete_http_client(http_client_t *client);
void close_connection(http_client_t *client);

// reader.c
uint8_t fd_is_readable(http_server_t *server, http_client_t *client);
uint16_t handle_read(http_server_t *server, http_client_t *client);


// http_request.c
http_request_t *new_request(void);
http_headers_t *new_header(void);
void free_request(http_request_t *req);
void free_header(http_headers_t *hdr);
void free_headers(http_headers_t **list);
void push_header(http_headers_t **list, http_headers_t *new);
void push_header_back(http_headers_t **list, http_headers_t *new);
void pop_header(http_headers_t **list);
void pop_header_back(http_headers_t **list);
http_headers_t *get_headers_field(http_headers_t **list, const char *key);

// parser.c
uint8_t parse_request(http_buffer_t *buf, http_request_t *req);
uint8_t parse_method(http_buffer_t *buf, http_request_t *req);
uint8_t parse_path(http_buffer_t *buf, http_request_t *req);
uint8_t parse_version(http_buffer_t *buf, http_request_t *req);
http_headers_t *parse_header(http_buffer_t *buf, http_headers_t *new);
uint8_t parse_headers(http_buffer_t *buf, http_headers_t **list);
void parse_body(http_buffer_t *buf, http_request_t *req);

// callbacks.c
void callback_http_get(http_client_t *client, http_request_t *req, http_response_t *res);
void callback_http_post(http_client_t *client, http_request_t *req, http_response_t *res);
void callback_http_put(http_client_t *client, http_request_t *req, http_response_t *res);
void callback_http_head(http_client_t *client, http_request_t *req, http_response_t *res);

// utils.c
char *read_file(const char *filename, uint64_t *buffer_size);