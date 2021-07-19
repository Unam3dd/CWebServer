#include "http_error.h"
#include "log.h"
#include <errno.h>
#include <string.h>

void print_error(void)
{
    print_log("%t error : %s\n", strerror(errno));
}