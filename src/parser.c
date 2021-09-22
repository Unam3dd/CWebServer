#include <string.h>
#include <stddef.h>
#include "http_server.h"

#define IS_SPACE(chr) (chr == ' ')
#define IS_CRLF(str) (str[0] == '\r' && str[1] == '\n')

uint8_t parse_request(http_buffer_t *buf, http_request_t *req)
{
    if (parse_method(buf, req))
        return (1);
    
    if (parse_uri(buf, req))
        return (1);
    
    if (parse_version(buf, req))
        return (1);
    
    if (parse_headers(buf, req))
        return (1);
    
    return (0);   
}

uint8_t parse_method(http_buffer_t *buf, http_request_t *req)
{
    char *tmp = (buf->buf + buf->pos);
    
    char *space = strchr(tmp, ' ');

    if (space == NULL)
        return (1);
    
    *space++ = 0;
    req->method = tmp;
    buf->pos += (space - tmp);

    return (0);
}

uint8_t parse_uri(http_buffer_t *buf, http_request_t *req)
{
    char *tmp = (buf->buf + buf->pos);

    char *space = strchr(tmp , ' ');

    if (space == NULL)
        return (1);
    
    *space++ = 0;
    req->uri = tmp;
    buf->pos += (space - tmp);

    return (0);
}

uint8_t parse_version(http_buffer_t *buf, http_request_t *req)
{
    char *tmp = (buf->buf + buf->pos);

    char *crlf = strstr(tmp, "\r\n");

    if (crlf == NULL && (crlf - tmp) != 8)
        return (1);
    
    *crlf = 0;
    
    crlf += 2;

    req->version = tmp;
    buf->pos += (crlf - tmp);

    return (0);
}

uint8_t parse_header(http_buffer_t *buf, http_header_t *header)
{
    char *tmp = strchr((char *) (buf->buf + buf->pos), ':');

    if (!tmp)
        return (1);
    
    *tmp++ = 0;
    header->key = (char *) (buf->buf + buf->pos);
    buf->pos += (tmp - (char *)(buf->buf + buf->pos));

    tmp = strstr((char *) (buf->buf + buf->pos), "\r\n");

    if (!tmp)
        return (1);
    
    *tmp = 0;
    header->value = (char *) (buf->buf + buf->pos);
    
    tmp += 2;

    buf->pos += (tmp - (char *)(buf->buf + buf->pos));
    
    return (0);
}


uint8_t parse_headers(http_buffer_t *buf, http_request_t *req)
{
    http_header_t *hdr = NULL;

    req->headers = new_header();
    
    char *ptr = (buf->buf + buf->pos);

    while (*ptr && !IS_CRLF(ptr)) {
        
        if (hdr == NULL) {

            hdr = req->headers;
            
            if (parse_header(buf, hdr))
                return (1);
                
        } else {

            hdr = new_header();

            if (parse_header(buf, hdr))
                return (1);

            push_header_back(&req->headers, hdr);
        }

        ptr = (buf->buf + buf->pos);
    }

    return (0);
}