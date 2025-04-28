#ifndef ZINC_UNIT_H
#define ZINC_UNIT_H

#include "test.h"

void Zinc_test_expect_null(Zinc_test* test, void* p, const char* message);
void Zinc_test_expect_size_t_equal(Zinc_test* test, size_t a, size_t b, const char* message);
void Zinc_test_expect_string(Zinc_test* test, Zinc_string* a, const char* b, const char* message);

#endif