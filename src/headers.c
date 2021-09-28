#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "http_server.h"

//////////////////////////////////////////////
//
//          HEADERS - headers.c
//
//////////////////////////////////////////////

http_header_t *new_header(void)
{
    http_header_t *header = (http_header_t *)malloc(sizeof(http_header_t));

    header->key = NULL;
    header->next = NULL;
    header->prev = NULL;
    header->value = NULL;

    return (header);
}

void free_header(http_header_t *header)
{
    if (header)
        free(header);
    
    header = NULL;
}

void push_header(http_header_t **list, http_header_t *new)
{
    (*list)->prev = new;
    new->next = *list;
    new->prev = NULL;
    *list = new;
}

void push_header_back(http_header_t **list, http_header_t *new)
{
    http_header_t *actual = *list;

    while (actual->next)
        actual = actual->next;
    
    new->next = NULL;
    new->prev = actual;
    actual->next = new;
}

void pop_header(http_header_t **list)
{
    http_header_t *next = (*list)->next;

    free(*list);

    *list = next;

    (*list)->prev = NULL;
}

void pop_header_back(http_header_t **list)
{
    http_header_t *actual = *list;
    http_header_t *prev = NULL;

    while (actual->next)
        actual = actual->next;
    
    prev = actual->prev;

    free(actual);

    actual = prev;
    actual->next = NULL;
    actual->prev = prev->prev;
}

void free_http_headers(http_header_t **list)
{
    http_header_t *next = NULL;

    while (*list) {
        next = (*list)->next;
        free_header(*list);
        *list = next;
    }

    *list = NULL;
}

http_header_t *http_get_header(http_header_t **list, char *key)
{
    http_header_t *hdr = *list;

    while (hdr && strcmp(hdr->key, key))
        hdr = hdr->next;
    
    return (hdr);
}