#include "http_server.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "usage : %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    http_server_t server;

    server.addr = argv[1];
    server.port = atoi(argv[2]);
    server.family = AF_INET;

    if (create_http_server(&server)) {
        print_error();
        return (1);
    }

    wait_http_server(&server);
    
    if (close_http_server(&server)) {
        print_error();
        return (1);
    }

    return (0);
}
