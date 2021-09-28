#include <stddef.h>
#include "http_types.h"
#include "http_server.h"

const http_error_t err_table[] = {
    { "505", "HTTP Version not supported", 0x1},
    { 0 }
};

http_error_t *http_get_error(http_request_t *req)
{
    uint8_t err = http_get_error_code(req);
    http_error_t *err_ptr = NULL;

    for (uint8_t i = 0; err_table[i].error_code; i++) {
        
        if (err_table[i].error_code != err)
            continue;

        err_ptr = (http_error_t *)&err_table[i];
    }

    return (err_ptr);
}

uint8_t http_get_error_code(http_request_t *req)
{
    if (http_check_version(req, HTTP_VERSION_1_1))
        return (1);
    
    return (0);
}