#include <stddef.h>
#include <stdlib.h>
#include "http_server.h"


///////////////////////////////////////////////
//
//          CLIENT - clients.c
//
///////////////////////////////////////////////

http_client_t *new_client(void)
{
    return ((http_client_t *)malloc(sizeof(http_client_t)));
}

void free_client(http_client_t *client)
{
    if (client)
        free(client);
    
    client = NULL;
}