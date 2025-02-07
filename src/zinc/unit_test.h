#ifndef ZINC_UNIT_TEST_H
#define ZINC_UNIT_TEST_H

#include "api.h"
#include "zstring.h"
#include "string_list.h"
#include "vector.h"
#include "String_slice.h"
#include <stdint.h>

#define TEST_NAME_SIZE 100

typedef struct Zinc_test_run {
	char test_case_name[TEST_NAME_SIZE];
	int test_case_count;
	int test_case_error_count;
	int test_case_has_test;
	int test_case_has_error;
	int check_count;
	int check_error_count;
} Zinc_test_run;

ZINC_API void Zinc_test_name(const char* fmt, ...);
ZINC_API void Zinc_panic();
ZINC_API void Zinc_test_called();
ZINC_API void Zinc_error_triggered();
ZINC_API void Zinc_print_results();
ZINC_API void Zinc_assert_int_equal(int a, int b, const char* message);
ZINC_API void Zinc_assert_true(int value, const char* message);
ZINC_API void Zinc_assert_false(int value, const char* message);
ZINC_API void Zinc_assert_ptr(void* p, const char* message);
ZINC_API void Zinc_assert_null(void* p, const char* message);
ZINC_API void Zinc_assert_ok(Zinc_result r, const char* message);
ZINC_API void Zinc_assert_error(Zinc_result r, const char* message);
ZINC_API void Zinc_expect_int_equal(int a, int b, const char* message);
ZINC_API void Zinc_expect_long_equal(long a, long b, const char* message);
ZINC_API void Zinc_expect_unsigned_long_equal(unsigned long a, unsigned long b, const char* message);
ZINC_API void Zinc_expect_long_long_equal(long long a, long long b, const char* message);
ZINC_API void Zinc_expect_u_long_equal(unsigned long a, unsigned long b, const char* message);
ZINC_API void Zinc_expect_double_equal(double a, double b, const char* message);
ZINC_API void Zinc_expect_ptr_equal(void* a, void* b, const char* message);
ZINC_API void Zinc_expect_char_equal(char a, char b, const char* message);
ZINC_API void Zinc_expect_uint_equal(unsigned int a, unsigned int b, const char* message);
ZINC_API void Zinc_assert_size_t_equal(size_t a, size_t b, const char* message);
ZINC_API void Zinc_expect_size_t_equal(size_t a, size_t b, const char* message);
ZINC_API void Zinc_expect_true(int value, const char* message);
ZINC_API void Zinc_expect_false(int value, const char* message);
ZINC_API void Zinc_expect_ptr(void* p, const char* message);
ZINC_API void Zinc_expect_null(void* p, const char* message);
ZINC_API void Zinc_expect_ok(Zinc_result r, const char* message);
ZINC_API void Zinc_expect_string(Zinc_string* a, const char* b, const char* message);
ZINC_API void Zinc_expect_vector_str(Zinc_vector* a, const char* b, const char* message);
ZINC_API void Zinc_expect_vector(Zinc_vector* a, Zinc_vector* b, const char* message);
ZINC_API void Zinc_expect_strcmp(const char* a, const char* b, const char* message);
ZINC_API void Zinc_expect_nts(const char* a, const char* b, const char* message);
ZINC_API void Zinc_expect_error_message(const char* s);
ZINC_API void Zinc_expect_vector_double(Zinc_vector* a, Zinc_vector* b, double threshold, const char* message);
ZINC_API void Zinc_expect_utf8_char(char* a, char* b, char* message);
ZINC_API void Zinc_expect_utf8_char_str(char a[4], int num, char* b, char* message);
ZINC_API void Zinc_expect_buffer_list_count(Zinc_string_list* bl, size_t count, char* message);
ZINC_API void Zinc_expect_buffer_list_item(Zinc_string_list* bl, size_t index, char* text, char* message);
ZINC_API void Zinc_expect_string_slice(Zinc_string_slice* sl, Zinc_string* bf, char* message);
ZINC_API void Zinc_expect_string_slice_str(Zinc_string_slice* sl, char* expected, char* message);
ZINC_API void Zinc_expect_int64_t_equal(int64_t a, int64_t b, const char* message);
ZINC_API void Zinc_expect_uint64_t_equal(uint64_t a, uint64_t b, const char* message);

#endif
