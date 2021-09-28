#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "http_server.h"

//////////////////////////////////////////////
//
//        DEFAULTS CALLBACKS - cb.c
//
//////////////////////////////////////////////

void http_callback_get(http_server_t *server, http_client_t *client, http_request_t *req)
{
    http_header_t *hdr_ptr = NULL;
    http_file_t file;
    char filename[0x100];

    print_log("[%t] %s %s %s\n", req->method, req->uri, req->version);

    snprintf(filename, sizeof(filename), "%s%s", server->path, req->uri);

    if (open_file(filename, &file)) {
        dprintf(client->fd, "HTTP/1.1 404 Not found.\r\nContent-Length: 14\r\n\r\nFile not found\r\n");
        return;
    }

    dprintf(client->fd, "HTTP/1.1 200 OK\r\nContent-Length: %zd\r\nContent-Types: text/html\r\n\r\n%s\r\n", file.size, file.buf);

    close_file(&file);

    hdr_ptr = http_get_header(&req->headers, "Connection");

    if (hdr_ptr && strcmp(hdr_ptr->value, "close") == 0) {
        free_http_headers(&req->headers);
        http_server_close_client(server, client);
    }
}