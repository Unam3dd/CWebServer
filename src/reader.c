#include "http_server.h"
#include <sys/ioctl.h>
#include <unistd.h>


uint8_t fd_is_readable(http_server_t *server, http_client_t *client)
{
    size_t len = 0;

    ioctl(client->fd, FIONREAD, &len);

    return (len);
}

uint16_t handle_read(http_server_t *server, http_client_t *client)
{
    client->data.pos = 0;

    return (read(client->fd, client->data.buffer, HTTP_HEADER_SIZE_DEFAULT));
}