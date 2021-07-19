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