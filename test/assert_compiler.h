#ifndef _ASSERT_COMPILER_H
#define _ASSERT_COMPILER_H

#include "alba/uconv.h"
#include "alba/source.h"

/* static-output */
void expect_utf8_char(char* a, char* b, char* message);

/* static-output */
void expect_utf32_char(UChar32 a, UChar32 b, char* message);

/* static-output */
struct compile_error* assert_compile_error(struct compile_error_list* el, char* s);

/* static-output */
void expect_compile_error_fields(struct compile_error* e, size_t line, size_t col, size_t byte_pos);

#endif
