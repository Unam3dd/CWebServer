#include <string.h>
#include "http_types.h"

//////////////////////////////////////////////
//
//        HTTP ERRORS SUBROUTINES
//         errors_subroutines.c
//
/////////////////////////////////////////////

uint8_t http_check_version(http_request_t *req, char *version)
{
    return (strcmp(req->version, version) != 0);
}