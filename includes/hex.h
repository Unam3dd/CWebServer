#pragma once
#include <stdint.h>

static const char HEX_TAB[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                   'A', 'B', 'C', 'D', 'E', 'F'};

// hex.c
char get_nibble_byte(char c);
char *base16_encode(char *dst, char *src, uint64_t dst_size);
char *base16_decode(char *dst, char *src, uint64_t dst_size);