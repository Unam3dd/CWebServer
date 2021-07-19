#include "http_server.h"
#include <sys/ioctl.h>

uint8_t fd_is_readable(http_server_t *server, http_client_t *client)
{
    size_t len = 0;

    ioctl(client->fd, FIONREAD, &len);

    return (len ? 0 : 1);
}