#include "http_server.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

http_request_t *new_request(void)
{
    http_request_t *new = (http_request_t *)malloc(sizeof(http_request_t));

    new->headers = NULL;
    new->http_version = NULL;
    new->method = NULL;
    new->uri = NULL;

    return (new);
}

http_headers_t *new_header(void)
{
    http_headers_t *new = (http_headers_t *)malloc(sizeof(http_headers_t));

    new->key = NULL;
    new->next = NULL;
    new->prev = NULL;
    new->value = NULL;

    return (new);
}

void free_request(http_request_t *req)
{
    if (req)
        free(req);
    
    req = NULL;
}

void free_header(http_headers_t *hdr)
{
    if (hdr)
        free(hdr);
    
    hdr = NULL;
}

void free_headers(http_headers_t **list)
{
    http_headers_t *next = NULL;

    while (*list) {
        next = (*list)->next;
        free(*list);
        *list = next;
    }

    *list = NULL;
}

void push_header(http_headers_t **list, http_headers_t *new)
{
    (*list)->prev = new;
    new->next = (*list);
    new->prev = NULL;
    *list = new;
}

void push_header_back(http_headers_t **list, http_headers_t *new)
{
    http_headers_t *actual = *list;

    while (actual->next)
        actual = actual->next;
    
    new->next = NULL;
    new->prev = actual;

    actual->next = new;
}

void pop_header(http_headers_t **list)
{
    http_headers_t *next = (*list)->next;
    free(*list);
    *list = next;
    (*list)->prev = NULL;
}

void pop_header_back(http_headers_t **list)
{
    http_headers_t *actual = *list;
    http_headers_t *prev = NULL;

    while (actual->next)
        actual = actual->next;
    
    prev = actual->prev;

    free(actual);

    actual = prev;
    actual->next = NULL;
    actual->prev = prev->prev;
}

http_headers_t *get_headers_field(http_headers_t **list, const char *key)
{
    http_headers_t *actual = *list;

    while (actual->next && strcmp(key, actual->key))
        actual = actual->next;

    return (actual);
}