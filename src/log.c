#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "log.h"


char *get_time(char *time_buf)
{
    time_t time_stamp = time(NULL);
    struct tm *time_info = localtime(&time_stamp);

    strftime(time_buf, 0x20, "%d/%m/%Y %H:%M:%S", time_info);

    return (time_buf);
}

void ft_putchar(char c)
{
    write(STDOUT_FILENO, &c, sizeof(char));
}

void ft_write_str(char *str)
{
    while (*str)
        ft_putchar(*str++);
}

char *int_to_str(int number, char *output)
{
    char *tmp = output;
    int rev = 0;

    while (number) {
        rev *= 10;
        rev += (number % 10);
        number /= 10;
    }

    while (rev >= 0) {
        if (rev) {
            *tmp++ = (rev % 10) + '0';
            rev /= 10;
        } else {
            *tmp++ = '0';
            rev = -1;
        }
    }

    *tmp = 0;

    return (output);
}

void print_log(const char *format, ...)
{
    char *tmp = (char *) format;
    char buf[0x20] = {0};
    va_list parameters;

    va_start(parameters, format);

    while (*tmp) {

        if (*tmp != '%') {
            ft_putchar(*tmp++);
            continue;
        }

        ++tmp;

        switch(*tmp) {

            case 's': {
                ft_write_str(va_arg(parameters, char *));
                break;
            }

            case 't': {
                ft_write_str(get_time(buf));
                break;
            }

            case 'd': {
                ft_write_str(int_to_str(va_arg(parameters, int), buf));
                break;
            }

            default:
                break;
        }

        tmp++;
    }

    va_end(parameters);
}

void print_error(void)
{
    print_log("[%t] error : %s\n", strerror(errno));
}