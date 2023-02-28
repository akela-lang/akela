#ifndef _UNIT_TEST_COMPILER_H
#define _UNIT_TEST_COMPILER_H

#include "alba_api.h"
#include "uconv.h"
#include "source.h"

/* dynamic-output-none */
ALBA_API void expect_utf8_char(char* a, char* b, char* message);

/* dynamic-output-none */
ALBA_API void expect_utf32_char(UChar32 a, UChar32 b, char* message);

#endif
