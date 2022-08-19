#ifndef _UNIT_TEST_COMPILER_H
#define _UNIT_TEST_COMPILER_H

#include "alba_api.h"
#include "uconv.h"
#include "source.h"

/* dynamic-output-none */
ALBA_API void assert_has_errors(struct compile_error_list* el);

ALBA_API void expect_has_errors(struct compile_error_list* el);

/* dynamic-output-none */
ALBA_API void assert_no_errors(struct compile_error_list* el);

/* dynamic-output-none */
ALBA_API void expect_no_errors(struct compile_error_list* el);

/* dynamic-output-none */
ALBA_API void expect_utf8_char(char* a, char* b, char* message);

/* dynamic-output-none */
ALBA_API void expect_utf32_char(UChar32 a, UChar32 b, char* message);

/* dynamic-output-none */
ALBA_API struct compile_error* assert_compile_error(struct compile_error_list* el, char* s);

ALBA_API struct compile_error* expect_compile_error(struct compile_error_list* el, char* s);

/* dynamic-output-none */
ALBA_API void expect_compile_error_fields(struct compile_error* e, size_t line, size_t col, size_t byte_pos);

#endif
