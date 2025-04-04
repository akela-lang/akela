#ifndef ZINC_UNICODE_H
#define ZINC_UNICODE_H

#include <stdlib.h>
#include "zstring.h"
#include <stdbool.h>
#include <stdint.h>

bool Zinc_is_hex_digit(char c[4], int num);
unsigned int Zinc_char_to_hex(char c);
unsigned int Zinc_char4_to_hex(char* src, int num);
char Zinc_num4_to_hex_char(uint8_t cp);
int Zinc_num32_to_hex_str(uint32_t cp, Zinc_string* bf);
int Zinc_code_to_utf8(unsigned char *const buffer, const unsigned int code);
int Zinc_utf8_to_utf32(const char *utf8, uint32_t *cp);

#endif