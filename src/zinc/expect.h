#ifndef ZINC_UNIT_H
#define ZINC_UNIT_H

#include "test.h"
#include "error.h"
#include "vector.h"

void Zinc_test_assert_ok(Zinc_test* test, Zinc_result r, const char* message);
void Zinc_test_assert_ptr(Zinc_test* test, void* p, const char* message);
void Zinc_test_assert_true(Zinc_test* test, int value, const char* message);
void Zinc_test_assert_false(Zinc_test* test, int value, const char* message);
void Zinc_test_assert_null(Zinc_test* test, void* p, const char* message);
void Zinc_test_assert_error(Zinc_test* test, Zinc_result r, const char* message);

void Zinc_test_expect_null(Zinc_test* test, void* p, const char* message);
void Zinc_test_expect_size_t_equal(Zinc_test* test, size_t a, size_t b, const char* message);
void Zinc_test_expect_string(Zinc_test* test, Zinc_string* a, const char* b, const char* message);
void Zinc_test_expect_true(Zinc_test* test, int value, const char* message);
void Zinc_test_expect_false(Zinc_test* test, int value, const char* message);
void Zinc_test_expect_strcmp(Zinc_test* test, const char* a, const char* b, const char* message);
void Zinc_test_expect_int_equal(Zinc_test* test, int a, int b, const char* message);
void Zinc_test_expect_ptr_equal(Zinc_test* test, const void* a, const void* b, const char* message);
void Zinc_test_assert_int_equal(Zinc_test* test, int a, int b, const char* message);
void Zinc_test_expect_int64_t_equal(Zinc_test* test, int64_t a, int64_t b, const char* message);
void Zinc_test_expect_uint64_t_equal(Zinc_test* test, uint64_t a, uint64_t b, const char* message);
void Zinc_test_expect_int32_t_equal(Zinc_test* test, int32_t a, int32_t b, const char* message);
void Zinc_test_expect_uint32_t_equal(Zinc_test* test, uint32_t a, uint32_t b, const char* message);
void Zinc_test_expect_long_equal(Zinc_test* test, long a, long b, const char* message);
void Zinc_test_expect_double_equal(Zinc_test* test, double a, double b, const char* message);
void Zinc_test_expect_unsigned_long_equal(Zinc_test* test, unsigned long a, unsigned long b, const char* message);
void Zinc_test_expect_long_long_equal(Zinc_test* test, long long a, long long b, const char* message);
void Zinc_test_expect_u_long_equal(Zinc_test* test, unsigned long a, unsigned long b, const char* message);

void Zinc_test_assert_no_errors(Zinc_test* test, Zinc_error_list* el);
void Zinc_test_expect_no_errors(Zinc_test* test, Zinc_error_list* el);
void Zinc_test_expect_error_count(Zinc_test* test, Zinc_error_list* el, size_t count);
void Zinc_test_assert_has_errors(Zinc_test* test, Zinc_error_list* el);
void Zinc_test_expect_has_errors(Zinc_test* test, Zinc_error_list* el);
Zinc_error* Zinc_test_assert_source_error(Zinc_test* test, Zinc_error_list* el, const char message[]);
Zinc_error* Zinc_test_expect_source_error(Zinc_test* test, Zinc_error_list* el, const char message[]);
void Zinc_test_expect_vector_str(Zinc_test* test, Zinc_vector* a, const char* b, const char* message);
void Zinc_test_expect_char_equal(Zinc_test* test, char a, char b, const char* message);
void Zinc_test_expect_ok(Zinc_test* test, Zinc_result r, const char* message);
void Zinc_test_expect_error_message(Zinc_test* test, const char* s);

#endif