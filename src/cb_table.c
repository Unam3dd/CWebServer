#include "http_server.h"

//////////////////////////////////////////////
//
//       CALLBACK TABLE - cb_table.c
//
//////////////////////////////////////////////

http_callbacks_t cb_table[] = {
    { "GET", &http_callback_get },
    {0}
};