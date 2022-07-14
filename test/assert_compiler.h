#ifndef _ASSERT_COMPILER_H
#define _ASSERT_COMPILER_H

#include "alba/uconv.h"
#include "alba/source.h"

void expect_utf8_char(char* a, char* b, char* message);
void expect_utf32_char(UChar32 a, UChar32 b, char* message);
void expect_compile_error_message(struct compile_error_list* el, char* s, size_t line, size_t col, size_t byte_pos);

#endif
