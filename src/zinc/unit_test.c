#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "result.h"
#include "buffer.h"
#include "buffer_list.h"
#include "vector.h"
#include "unit_test.h"
#include <assert.h>
#include <math.h>
#include "utf8.h"
#include "String_slice.h"

struct test_run tr;

/* static-output */
void test_name(const char* fmt, ...)
{
#if defined(WIN32)
	va_list args = NULL;
#elif defined(unix)
		va_list args;
#endif
	va_start(args, fmt);
	char buf[TEST_NAME_SIZE];
	int len;

	tr.test_case_name[0] = '\0';
	tr.test_case_has_test = 0;
	tr.test_case_has_error = 0;

	char* name = tr.test_case_name;
	char last_last = 0;
	char last = 0;
	int i = 0;
	while (*fmt != '\0') {
		if (last == '%' && *fmt == '%') {
			if (i < TEST_NAME_SIZE) name[i++] = '%';
		} else if (*fmt == '%') {
			/* nothing */
		} else if (last == '%' && *fmt == 'd') {
			len = snprintf(buf, TEST_NAME_SIZE, "%d", va_arg(args, int));
			for (int j = 0; j < len; j++) {
				if (i < TEST_NAME_SIZE) name[i++] = buf[j];
			}
		} else if (last_last == '%' && last == 'z' && *fmt == 'u') {
			len = snprintf(buf, TEST_NAME_SIZE, "%zu", va_arg(args, size_t));
			for (int j = 0; j < len; j++) {
				if (i < TEST_NAME_SIZE) name[i++] = buf[j];
			}
		} else if (last == '%' && *fmt == 's') {
			len = snprintf(buf, TEST_NAME_SIZE, "%s", va_arg(args, char*));
			for (int j = 0; j < len; j++) {
				if (i < TEST_NAME_SIZE) name[i++] = buf[j];
			}
		} else if (last == '%' && *fmt == 'c') {
			len = snprintf(buf, TEST_NAME_SIZE, "%c", va_arg(args, int));
			for (int j = 0; j < len; j++) {
				if (i < TEST_NAME_SIZE) name[i++] = buf[j];
			}
		} else {
			if (i < TEST_NAME_SIZE) name[i++] = *fmt;
		}
		last_last = last;
		last = *fmt;
		fmt++;
	}

	if (i > TEST_NAME_SIZE - 1) {
		i = TEST_NAME_SIZE - 1;
	}
	name[i++] = '\0';

	va_end(args);
}

/* static-output */
void panic()
{
	fprintf(stderr, "Exiting because of assertion error.\n");
	exit(1);
}

/* static-output */
void test_called()
{
	tr.check_count++;
	if (!tr.test_case_has_test) {
		tr.test_case_count++;
		tr.test_case_has_test = 1;
	}
}

/* static-output */
void error_triggered()
{
	tr.check_error_count++;
	if (!tr.test_case_has_error) {
		tr.test_case_error_count++;
		tr.test_case_has_error = 1;
		fprintf(stderr, "\n%s:\n", tr.test_case_name);
	}
}

/* static-output */
void print_results()
{
	fprintf(stdout, "\ntest case count: %d\n", tr.test_case_count);
	fprintf(stdout, "test case error count: %d\n\n", tr.test_case_error_count);

	fprintf(stdout, "check count: %d\n", tr.check_count);
	fprintf(stdout, "check error count: %d\n", tr.check_error_count);
    if (tr.test_case_error_count || tr.check_error_count) {
        exit(1);
    }
}

/* static-output */
void assert_int_equal(int a, int b, const char* message)
{
	test_called();

	if (a == b) return;
	fprintf(stderr, "%d = %d assertion error: %s\n", a, b, message);

	error_triggered();
	panic();
}

/* static-output */
void assert_true(int value, const char* message)
{
	test_called();

	if (value) return;
	error_triggered();
	fprintf(stderr, "%d = true assertion error: %s\n", value, message);
	panic();
}

/* static-output */
void assert_false(int value, const char* message)
{
	test_called();

	if (!value) return;
	error_triggered();
	fprintf(stderr, "%d = false assertion error: %s\n", value, message);
	panic();
}

/* static-output */
void assert_ptr(void* p, const char* message)
{
	test_called();

	if (p) return;
	error_triggered();
	fprintf(stderr, "%p != NULL assertion error: %s\n", p, message);
	panic();
}

/* static-output */
void assert_null(void* p, const char* message)
{
	test_called();

	if (p == NULL) return;
	error_triggered();
	fprintf(stderr, "%p == null assertion error: %s\n", p, message);
	panic();
}

/* static-output */
void assert_ok(enum result r, const char* message)
{
	test_called();

	if (r == result_ok) return;
	error_triggered();
	fprintf(stderr, "ok assertion error: %s: %s\n", message, error_message);
	panic();
}

/* static-output */
void assert_error(enum result r, const char* message)
{
	test_called();

	if (r == result_error) return;
	error_triggered();
	fprintf(stderr, "no error detected assertion error: %s\n", message);
	panic();
}

/* static-output */
void expect_int_equal(int a, int b, const char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	fprintf(stderr, "%d = %d error: %s\n", a, b, message);
}

void expect_long_equal(long a, long b, const char* message)
{
    test_called();
    if (a == b) return;
    error_triggered();
    fprintf(stderr, "%ld = %ld error: %s\n", a, b, message);
}

void expect_long_long_equal(long long a, long long b, const char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	fprintf(stderr, "%lld = %lld error: %s\n", a, b, message);
}

void expect_u_long_equal(unsigned long a, unsigned long b, const char* message)
{
    test_called();
    if (a == b) return;
    error_triggered();
    fprintf(stderr, "%ld = %ld error: %s\n", a, b, message);
}

void expect_double_equal(double a, double b, const char* message)
{
    test_called();
    if (isnan(a) && isnan(b)) return;
    if (a == b) return;
    error_triggered();
    fprintf(stderr, "%lf = %lf error: %s\n", a, b, message);
}

/* static-output */
void expect_ptr_equal(void* a, void* b, const char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	fprintf(stderr, "%p = %p error: %s\n", a, b, message);
}

/* static-output */
void expect_char_equal(char a, char b, const char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	fprintf(stderr, "(%c) = (%c) error: %s\n", a, b, message);
}

/* static-output */
void expect_uint_equal(unsigned int a, unsigned int b, const char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	fprintf(stderr, "(%u) = (%u) error: %s\n", a, b, message);
}

/* static-output */
void assert_size_t_equal(size_t a, size_t b, const char* message)
{
    test_called();
    if (a == b) return;
    error_triggered();
    fprintf(stderr, "(%zu) = (%zu) error: %s\n", a, b, message);
    panic();
}

/* static-output */
void expect_size_t_equal(size_t a, size_t b, const char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	fprintf(stderr, "(%zu) = (%zu) error: %s\n", a, b, message);
}

/* static-output */
void expect_true(int value, const char* message)
{
	test_called();
	if (value) return;
	error_triggered();
	fprintf(stderr, "%d = true error: %s\n", value, message);
}

/* static-output */
void expect_false(int value, const char* message)
{
	test_called();
	if (!value) return;
	error_triggered();
	fprintf(stderr, "(%d) = (false) error: %s\n", value, message);
}

/* static-output */
void expect_ptr(void* p, const char* message)
{
	test_called();
	if (p) return;
	error_triggered();
	fprintf(stderr, "%p != NULL error: %s\n", p, message);
}

/* static-output */
void expect_null(void* p, const char* message)
{
	test_called();
	if (p == NULL) return;
	error_triggered();
	fprintf(stderr, "%p == null error: %s\n", p, message);
}

/* static-output */
void expect_ok(enum result r, const char* message)
{
	test_called();
	if (r == result_ok) return;
	error_triggered();
	fprintf(stderr, "ok error: %s: %s\n", message, error_message);
}

/* static-output */
/* dynamic-temp temp */
void expect_str(struct buffer* a, const char* b, const char* message)
{
	test_called();
	if (buffer_compare_str(a, b)) return;
	error_triggered();
	char* temp;

	/* allocate temp */
	buffer2array(a, &temp);
	fprintf(stderr, "%s equals %s error: %s\n", temp, b, message);

	/* destroy temp */
	free(temp);
}

void expect_vector_str(Vector* a, const char* b, const char* message)
{
    test_called();
    if (a->value_size != sizeof(char)) {
        fprintf(stderr,
                "Vector elements must be size of char to compare to string: %s\n",
                message);
        error_triggered();
        return;
    }
    VectorAddNull(a);
    size_t len = strlen(b);
    if (a->count != len) {
        fprintf(stderr,
                "Vector and string not the same size: (%s) (%s): %s\n",
                VECTOR_STRING(a), b, message);
        error_triggered();
    } else {
        for (int i = 0; i < a->count; i++) {
            if (VECTOR_CHAR(a, i) != b[i]) {
                fprintf(stderr,
                        "Vector and string not equal: (%s) (%s): %s\n",
                        VECTOR_STRING(a), b, message);
                error_triggered();
                break;
            }
        }
    }
}

void expect_vector(Vector* a, Vector* b, const char* message)
{
    test_called();
    if (a->value_size != b->value_size) {
        fprintf(stderr,
                "Vector elements must be the same size: %s\n",
                message);
        error_triggered();
        return;
    }
    if (a->count != b->count) {
        fprintf(stderr,
                "Vectors not the same size: (%zu) (%zu): %s\n",
                a->count, b->count, message);
        error_triggered();
    } else {
        for (int i = 0; i < a->count * a->value_size; i++) {
            if (VECTOR_CHAR(a, i) != VECTOR_CHAR(b, i)) {
                fprintf(stderr,
                        "Vectors not equal: %d: %s\n",
                        i, message);
                error_triggered();
                break;
            }
        }
    }
}

void expect_vector_double(Vector* a, Vector* b, double threshold, const char* message)
{
    test_called();
    if (a->value_size != b->value_size) {
        fprintf(stderr,
                "Vector elements must be the same size: %s\n",
                message);
        error_triggered();
        return;
    }
    if (a->count != b->count) {
        fprintf(stderr,
                "Vectors not the same size: (%zu) (%zu): %s\n",
                a->count, b->count, message);
        error_triggered();
    } else {
        for (size_t i = 0; i < a->count; i++) {
            double a_value = VECTOR_DOUBLE(a, i);
            double b_value = VECTOR_DOUBLE(b, i);
            if (isnan(a_value) && isnan(b_value)) {
                continue;
            }
            double delta = a_value - b_value;
            if (delta < 0) {
                delta *= -1;
            }
            if (isnan(delta) || delta >= threshold) {
                fprintf(stderr,
                        "Vector element %zu not equal: (%15.15lf) (%15.15lf): %s\n",
                        i, a_value, b_value, message);
                error_triggered();
                break;
            }
        }
    }
}

/* static-output */
void expect_strcmp(const char* a, const char* b, const char* message)
{
	test_called();
	if (strcmp(a, b) == 0) return;
	error_triggered();
	fprintf(stderr, "(%s) = (%s) strcmp error: %s\n", a, b, message);
}

/* static-output */
void expect_nts(const char* a, const char* b, const char* message)
{
	test_called();
	if (strcmp(a, b) == 0) return;
	error_triggered();
	fprintf(stderr, "string compare: (%s) = (%s): %s\n", a, b, message);
}

/* static-output */
void expect_error_message(const char* s)
{
	test_called();
	if (strcmp(error_message, s) == 0) return;
	error_triggered();
	fprintf(stderr, "(%s) = (%s): error message does not memory_match\n", error_message, s);
}

void expect_utf8_char(char* a, char* b, char* message)
{
    test_called();
    int count_a = NUM_BYTES(a[0]);
    int count_b = NUM_BYTES(b[0]);
    if (count_a == count_b) {
        for (int i = 0; i < count_a; i++) {
            if (a[i] != b[i]) {
            	error_triggered();
            	fprintf(stderr, "utf8 chars not equal: %s\n", message);
            	return;
            }
        }
        return;
    }
    error_triggered();
    fprintf(stderr, "utf8 chars not equal: %s\n", message);
}

void expect_utf8_char_str(char a[4], int num, char* b, char* message)
{
	test_called();
	int count_a = NUM_BYTES(a[0]);
	int count_b = NUM_BYTES(b[0]);
	size_t len = strlen(b);
	if (num == count_a && num == count_b && num == len) {
		for (int i = 0; i < num; i++) {
			if (a[i] != b[i]) {
				error_triggered();
				fprintf(stderr, "utf8 chars not equal: %s\n", message);
				return;
			}
		}
		return;
	}
	error_triggered();
	fprintf(stderr, "utf8 chars not equal: %s\n", message);
}

void expect_buffer_list_count(struct buffer_list* bl, size_t count, char* message)
{
	test_called();
	size_t actual_count = buffer_list_count(bl);
	if (actual_count == count) return;
	error_triggered();
	fprintf(stderr, "buffer list count not equal: %zu != %zu: %s\n", actual_count, count, message);
}

void expect_buffer_list_item(struct buffer_list* bl, size_t index, char* text, char* message)
{
	struct buffer* bf = buffer_list_get(bl, index);
	if (!bf) {
		error_triggered();
		fprintf(stderr, "buffer list index out of bounds: %zu: %s\n", index, message);
		return;
	}

	if (!buffer_compare_str(bf, text)) {
		error_triggered();
		buffer_finish(bf);
		fprintf(stderr,
			"buffer list item (%zu) text not equal: (%s) (%s): %s\n", index, bf->buf, text, message);
	}
}

void expect_string_slice(String_slice* sl, struct buffer* bf, char* message)
{
	test_called();
	bool has_error = false;

	if (sl->size == bf->size) {
		for (size_t i = 0; i < sl->size; i++) {
			if (sl->p[i] != bf->buf[i]) {
				has_error = true;
				break;
			}
		}
	} else {
		has_error = true;
	}

	if (has_error) {
		error_triggered();
		struct buffer actual;
		buffer_init(&actual);
		for (size_t i = 0; i < sl->size; i++) {
			buffer_add_char(&actual, sl->p[i]);
		}
		buffer_finish(&actual);

		buffer_finish(bf);
		fprintf(stderr, "%s = %s error: %s\n", actual.buf, bf->buf, message);
		buffer_destroy(&actual);
	}
}

void expect_string_slice_str(String_slice* sl, char* expected, char* message)
{
	test_called();
	bool has_error = false;

	size_t size = strlen(expected);
	if (sl->size == size) {
		for (size_t i = 0; i < sl->size; i++) {
			if (sl->p[i] != expected[i]) {
				has_error = true;
				break;
			}
		}
	} else {
		has_error = true;
	}

	if (has_error) {
		error_triggered();
		struct buffer actual;
		buffer_init(&actual);
		for (size_t i = 0; i < sl->size; i++) {
			buffer_add_char(&actual, sl->p[i]);
		}
		buffer_finish(&actual);

		fprintf(stderr, "%s = %s error: %s\n", actual.buf, expected, message);
		buffer_destroy(&actual);
	}
}