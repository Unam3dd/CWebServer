#include "hex.h"
#include <string.h>
#include <stddef.h>

char get_nibble_byte(char c)
{
    return ((c >= '0' && c <= '9') ? (c & 0xF) : (c >= 'A' && c <= 'F') ? 10 + (c - 'A') : (c >= 'a' && c <= 'f') ? 10 + (c - 'a') : 0);
}

char *base16_encode(char *dst, char *src, uint64_t dst_size)
{
    if ((strlen(src) << 1) > dst_size)
        return (NULL);

    char *tmp = dst;

    while (*src) {
        *tmp++ = HEX_TAB[*src >> 4];
        *tmp++ = HEX_TAB[*src++ & 0xF];
    }

    *tmp = 0;

    return (dst);
}

char *base16_decode(char *dst, char *src, uint64_t dst_size)
{
    if ((strlen(src) << 1) > dst_size)
        return (NULL);
    
    char *tmp = dst;

    while (*src) {
        *tmp = get_nibble_byte(*src++);
        *tmp <<= 4;
        *tmp++ += get_nibble_byte(*src++);
    }

    *tmp = 0;
    
    return (dst);
}