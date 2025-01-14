#ifndef ZINC_UNICODE_H
#define ZINC_UNICODE_H

#include <stdlib.h>
#include "buffer.h"
#include <stdbool.h>

bool is_hex_digit(char c[4], int num);
int code_to_utf8(unsigned char *const buffer, const unsigned int code);
unsigned int char_to_hex(char c);
unsigned int char4_to_hex(char* src, int num);
char num4_to_hex_char(u_int8_t cp);
int num32_to_hex_str(u_int32_t cp, struct Zinc_string* bf);

#endif