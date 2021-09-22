#pragma once

//////////////////////////////////////////////
//
//          HTTP TYPES - http_types.h
//
/////////////////////////////////////////////
#include "http_types.h"
#include "log.h"


//////////////////////////////////////////////
//
//              DEFINES
//
//////////////////////////////////////////////
#define EPOLL_EVENTS 0x10

//////////////////////////////////////////////
//
//              VOLATILE
//
///////////////////////////////////////////////
static volatile int run = 1;


//////////////////////////////////////////////
//
//          HANDLE SIGNAL - server.c
//
/////////////////////////////////////////////
void handle_signal(int sig);

///////////////////////////////////////////////
//
//          SERVER - server.c
//
///////////////////////////////////////////////

void http_server_initialize(http_server_t *server);
void http_server_destroy(http_server_t *server);
uint8_t http_server_close_client(http_server_t *server, http_client_t *client);

// server callbacks
uint8_t http_server_init(http_server_t *server);
void http_server_close(http_server_t *server);
void http_server_wait(http_server_t *server);
uint8_t http_server_accept(http_server_t *server, http_client_t *client);
void http_server_handle(http_server_t *server, http_client_t *client);

///////////////////////////////////////////////
//
//          UTILS - utils.c
//
///////////////////////////////////////////////

uint8_t fd_is_readable(http_client_t *client);

///////////////////////////////////////////////
//
//          CLIENT - clients.c
//
///////////////////////////////////////////////
http_client_t *new_client(void);
void free_client(http_client_t *client);


//////////////////////////////////////////////
//
//          HEADERS - headers.c
//
//////////////////////////////////////////////

http_header_t *new_header(void);
void free_header(http_header_t *header);
void push_header(http_header_t **list, http_header_t *new);
void push_header_back(http_header_t **list, http_header_t *new);
void pop_header(http_header_t **list);
void pop_header_back(http_header_t **list);
void free_http_headers(http_header_t **list);


//////////////////////////////////////////////
//
//          PARSER - parser.c
//
//////////////////////////////////////////////
uint8_t parse_request(http_buffer_t *buf, http_request_t *req);
uint8_t parse_method(http_buffer_t *buf, http_request_t *req);
uint8_t parse_uri(http_buffer_t *buf, http_request_t *req);
uint8_t parse_version(http_buffer_t *buf, http_request_t *req);
uint8_t parse_header(http_buffer_t *buf, http_header_t *header);
uint8_t parse_headers(http_buffer_t *buf, http_request_t *req);