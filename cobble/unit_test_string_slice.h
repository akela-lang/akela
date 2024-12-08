#ifndef UNIT_TEST_STRING_SLICE_H
#define UNIT_TEST_STRING_SLICE_H

#include "String_slice.h"
#include "zinc/buffer.h"

void expect_string_slice(String_slice* sl, struct buffer* bf, char* message);
void expect_string_slice_str(String_slice* sl, char* expected, char* message);

#endif