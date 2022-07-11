#ifndef _TESTING_H
#define _TESTING_H

#include "alba/buffer.h"
#include "alba/uconv.h"

void assert_int_equal(int a, int b, char* message);
void assert_true(int value, char* message);
void assert_ptr(void* p, char* message);
void assert_null(void* p, char* message);
void assert_ok(enum result r, char* message);
void assert_error(enum result r, char* message);
void expect_int_equal(int a, int b, char* message);
void expect_char_equal(char a, char b, char* message);
void expect_uint_equal(unsigned int a, unsigned int b, char* message);
void expect_true(int value, char* message);
void expect_false(int value, char* message);
void expect_ptr(void* p, char* message);
void expect_null(void* p, char* message);
void expect_ok(enum result r, char* message);
void expect_str(struct buffer* a, char* b, char* message);
void expect_strcmp(char* a, char* b, char* message);
void expect_utf8_char(char* a, char* b, char* message);
void expect_utf32_char(UChar32 a, UChar32 b, char* message);
void test_name(const char* fmt, ...);
void print_results();
void expect_error_message(char* s);

#endif
