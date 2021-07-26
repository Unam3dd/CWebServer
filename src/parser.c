#include "http_server.h"
#include <string.h>
#include <stdio.h>

#define IS_SP(chr) (chr == ' ')
#define IS_CRLF(str) (str[0] == '\r' && str[1] == '\n')

uint8_t parse_request(http_buffer_t *buf, http_request_t *req)
{
    if (!buf)
        return (1);
    
    if (parse_method(buf, req))
        return (1);

    if (parse_path(buf, req))
        return (1);
    
    if (parse_version(buf, req))
        return (1);

    return (0);
}

uint8_t parse_method(http_buffer_t *buf, http_request_t *req)
{
    char *tmp = buf->buffer;

    while (*tmp && !IS_SP(*tmp))
        tmp++;
    
    if (*tmp == 0)
        return (1);

    *tmp++ = 0;
    
    req->method = buf->buffer;
    buf->pos += (tmp - buf->buffer); // point to the next word

    return (0);
}

uint8_t parse_path(http_buffer_t *buf, http_request_t *req)
{
    char *tmp = (buf->buffer + buf->pos);
    char *original = (buf->buffer + buf->pos);

    while (*tmp && !IS_SP(*tmp))
        tmp++;
    
    if (*tmp == 0)
        return (1);
    
    *tmp++ = 0;

    req->uri = original;
    buf->pos += (tmp - original);

    return (0);
}

uint8_t parse_version(http_buffer_t *buf, http_request_t *req)
{
    char *tmp = (buf->buffer + buf->pos);
    char *original = (buf->buffer + buf->pos);

    while (*tmp && !IS_CRLF(tmp))
        tmp++;

    if (*tmp == 0)
        return (1);

    *tmp++ = 0;

    req->http_version = original;
    
    buf->pos += (tmp - original);

    return (0);
}

http_headers_t *parse_header(http_buffer_t *buf, http_headers_t *new)
{
    char *tmp = (buf->buffer + buf->pos);

    char *original = NULL;

    while (*tmp && *tmp <= 32)
        tmp++;
    
    if (*tmp == 0)
        return (NULL);
    
    original = tmp;

    while (*tmp && *tmp != ':')
        tmp++;
    
    if (*tmp == 0)
        return (NULL);
    
    *tmp = 0;
    new->key = original;
    original = ++tmp;

    while (*tmp && !IS_CRLF(tmp))
        tmp++;
    
    if (*tmp == 0)
        return (NULL);
    
    *tmp = 0;
    new->value = original;

    buf->pos += (tmp - (buf->buffer + buf->pos)) + 2;

    printf("%s", tmp);

    return (new);
}

uint8_t parse_headers(http_buffer_t *buf, http_headers_t **list)
{
    http_headers_t *new_hdr = NULL;
    char *tmp = NULL;

    *list = new_header();

    if (parse_header(buf, *list) == NULL)
        return (1);
    
    tmp = (buf->buffer + buf->pos);

    while (*tmp && !IS_CRLF(tmp)) {

        new_hdr = new_header();

        if (parse_header(buf, new_hdr) == NULL)
            return (1);
        
        push_header_back(list, new_hdr);

        tmp = (buf->buffer + buf->pos);
    }

    return (0);
}

void parse_body(http_buffer_t *buf, http_request_t *req)
{
    char *tmp = (buf->buffer + buf->pos);

    if (IS_CRLF(tmp))
        tmp += 2;
    
    req->body = tmp;
}