#include "http_server.h"

const http_callback_t http_callbacks[] = {
    { "GET", &callback_http_get },
    { 0 },
};