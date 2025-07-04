#ifndef ZINC_UNIT_H
#define ZINC_UNIT_H

#include "test.h"
#include "error.h"
#include "vector.h"
#include "string_list.h"

void Zinc_assert();

void Zinc_expect_check(Zinc_test* test);
void Zinc_expect_passed(Zinc_test* test);
void Zinc_expect_failed(Zinc_test* test);
bool Zinc_expect_result(Zinc_test* test, Zinc_result r, const char* message);
bool Zinc_expect_null(Zinc_test* test, void* p, const char* message);
bool Zinc_expect_size_t_equal(Zinc_test* test, size_t a, size_t b, const char* message);
bool Zinc_expect_string(Zinc_test* test, Zinc_string* a, const char* b, const char* message);
bool Zinc_expect_true(Zinc_test* test, bool value, const char* fmt, ...);
bool Zinc_expect_false(Zinc_test* test, int value, const char* message);
bool Zinc_expect_strcmp(Zinc_test* test, const char* a, const char* b, const char* message);
bool Zinc_expect_int_equal(Zinc_test* test, int a, int b, const char* message);
bool Zinc_expect_ptr_equal(Zinc_test* test, const void* a, const void* b, const char* message);
bool Zinc_expect_long_equal(Zinc_test* test, long a, long b, const char* message);
bool Zinc_expect_unsigned_long_equal(Zinc_test* test, unsigned long a, unsigned long b, const char* message);
bool Zinc_expect_long_long_equal(Zinc_test* test, long long a, long long b, const char* message);
bool Zinc_expect_u_long_equal(Zinc_test* test, unsigned long a, unsigned long b, const char* message);
bool Zinc_expect_char_equal(Zinc_test* test, char a, char b, const char* message);

bool Zinc_expect_no_errors(Zinc_test* test, Zinc_error_list* el);
bool Zinc_expect_error_count(Zinc_test* test, Zinc_error_list* el, size_t count);
bool Zinc_expect_has_errors(Zinc_test* test, Zinc_error_list* el);
Zinc_error* Zinc_expect_source_error(Zinc_test* test, Zinc_error_list* el, const char message[]);
bool Zinc_expect_vector_str(Zinc_test* test, Zinc_vector* a, const char* b, const char* message);
bool Zinc_expect_ok(Zinc_test* test, Zinc_result r, const char* message);
bool Zinc_expect_error_message(Zinc_test* test, const char* s);
bool Zinc_expect_ptr(Zinc_test* test, void* p, const char* fmt, ...);
bool Zinc_expect_vector(Zinc_test* test, Zinc_vector* a, Zinc_vector* b, const char* message);

bool Zinc_expect_buffer_list_count(Zinc_test* test, Zinc_string_list* bl, size_t count, char* message);
bool Zinc_expect_buffer_list_item(Zinc_test* test, Zinc_string_list* bl, size_t index, char* text, char* message);

bool Zinc_expect_utf8_char_str(Zinc_test* test, char a[4], int num, char* b, char* message);

bool Zinc_expect_uint_equal(Zinc_test* test, unsigned int a, unsigned int b, const char* message);
bool Zinc_expect_string_slice(Zinc_test* test, Zinc_string_slice* sl, Zinc_string* bf, char* message);
bool Zinc_expect_string_slice_str(Zinc_test* test, Zinc_string_slice* sl, char* expected, char* message);
bool Zinc_expect_utf8_char(Zinc_test* test, char* a, char* b, char* message);
bool Zinc_expect_vector_double(Zinc_test* test, Zinc_vector* a, Zinc_vector* b, double threshold, const char* message);
bool Zinc_expect_nts(Zinc_test* test, const char* a, const char* b, const char* message);

//
// Sized Integer
//
bool Zinc_expect_int8_t_equal(Zinc_test* test, int8_t a, int8_t b, const char* message);
bool Zinc_expect_int16_t_equal(Zinc_test* test, int16_t a, int16_t b, const char* message);
bool Zinc_expect_int32_t_equal(Zinc_test* test, int32_t a, int32_t b, const char* message);
bool Zinc_expect_int64_t_equal(Zinc_test* test, int64_t a, int64_t b, const char* message);

//
// Sized Unsigned Integer
//
bool Zinc_expect_uint8_t_equal(Zinc_test* test, uint8_t a, uint8_t b, const char* message);
bool Zinc_expect_uint16_t_equal(Zinc_test* test, uint16_t a, uint16_t b, const char* message);
bool Zinc_expect_uint32_t_equal(Zinc_test* test, uint32_t a, uint32_t b, const char* message);
bool Zinc_expect_uint64_t_equal(Zinc_test* test, uint64_t a, uint64_t b, const char* message);

//
// Float
//
bool Zinc_expect_float_equal(Zinc_test* test, float a, float b, const char* message);
bool Zinc_expect_double_equal(Zinc_test* test, double a, double b, const char* message);

//
// Boolean
//
bool Zinc_expect_boolean_equal(Zinc_test* test, bool a, bool b, const char* message);

#endif