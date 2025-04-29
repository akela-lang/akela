#ifndef ZINC_UNIT_H
#define ZINC_UNIT_H

#include "test.h"

void Zinc_test_assert_ok(Zinc_test* test, Zinc_result r, const char* message);
void Zinc_test_assert_ptr(Zinc_test* test, void* p, const char* message);

void Zinc_test_expect_null(Zinc_test* test, void* p, const char* message);
void Zinc_test_expect_size_t_equal(Zinc_test* test, size_t a, size_t b, const char* message);
void Zinc_test_expect_string(Zinc_test* test, Zinc_string* a, const char* b, const char* message);
void Zinc_test_expect_true(Zinc_test* test, int value, const char* message);
void Zinc_test_expect_false(Zinc_test* test, int value, const char* message);
void Zinc_test_expect_strcmp(Zinc_test* test, const char* a, const char* b, const char* message);
void Zinc_test_expect_int_equal(Zinc_test* test, int a, int b, const char* message);
void Zinc_test_expect_ptr_equal(Zinc_test* test, const void* a, const void* b, const char* message);

#endif