#pragma once
#include <stdarg.h>
#include <stdint.h>

uint8_t count_digits(uint32_t i);
char *get_time(char *time_buf);
char *int_to_str(int number, char *output);
void ft_putchar(char c);
void ft_write_str(char *str);
void print_log(const char *str, ...);
void print_error(void);