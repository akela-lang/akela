#ifndef _UNIT_TEST_H
#define _UNIT_TEST_H

#include "api.h"
#include "buffer.h"
#include "buffer_list.h"
#include "vector.h"
#include "String_slice.h"

#define TEST_NAME_SIZE 100

struct test_run {
	char test_case_name[TEST_NAME_SIZE];
	int test_case_count;
	int test_case_error_count;
	int test_case_has_test;
	int test_case_has_error;
	int check_count;
	int check_error_count;
};

ZINC_API void test_name(const char* fmt, ...);
ZINC_API void panic();
ZINC_API void test_called();
ZINC_API void error_triggered();
ZINC_API void print_results();
ZINC_API void assert_int_equal(int a, int b, const char* message);
ZINC_API void assert_true(int value, const char* message);
ZINC_API void assert_false(int value, const char* message);
ZINC_API void assert_ptr(void* p, const char* message);
ZINC_API void assert_null(void* p, const char* message);
ZINC_API void assert_ok(enum result r, const char* message);
ZINC_API void assert_error(enum result r, const char* message);
ZINC_API void expect_int_equal(int a, int b, const char* message);
ZINC_API void expect_long_equal(long a, long b, const char* message);
ZINC_API void expect_long_long_equal(long long a, long long b, const char* message);
ZINC_API void expect_u_long_equal(unsigned long a, unsigned long b, const char* message);
ZINC_API void expect_double_equal(double a, double b, const char* message);
ZINC_API void expect_ptr_equal(void* a, void* b, const char* message);
ZINC_API void expect_char_equal(char a, char b, const char* message);
ZINC_API void expect_uint_equal(unsigned int a, unsigned int b, const char* message);
ZINC_API void assert_size_t_equal(size_t a, size_t b, const char* message);
ZINC_API void expect_size_t_equal(size_t a, size_t b, const char* message);
ZINC_API void expect_true(int value, const char* message);
ZINC_API void expect_false(int value, const char* message);
ZINC_API void expect_ptr(void* p, const char* message);
ZINC_API void expect_null(void* p, const char* message);
ZINC_API void expect_ok(enum result r, const char* message);
ZINC_API void expect_str(struct Zinc_string* a, const char* b, const char* message);
ZINC_API void expect_vector_str(Vector* a, const char* b, const char* message);
ZINC_API void expect_vector(Vector* a, Vector* b, const char* message);
ZINC_API void expect_strcmp(const char* a, const char* b, const char* message);
ZINC_API void expect_nts(const char* a, const char* b, const char* message);
ZINC_API void expect_error_message(const char* s);
ZINC_API void expect_vector_double(Vector* a, Vector* b, double threshold, const char* message);
ZINC_API void expect_utf8_char(char* a, char* b, char* message);
ZINC_API void expect_utf8_char_str(char a[4], int num, char* b, char* message);
ZINC_API void expect_buffer_list_count(struct Zinc_string_list* bl, size_t count, char* message);
ZINC_API void expect_buffer_list_item(struct Zinc_string_list* bl, size_t index, char* text, char* message);
ZINC_API void expect_string_slice(String_slice* sl, struct Zinc_string* bf, char* message);
ZINC_API void expect_string_slice_str(String_slice* sl, char* expected, char* message);

#endif
