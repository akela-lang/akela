#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "alba/result.h"
#include "alba/buffer.h"
#include "alba/uconv.h"
#include "assert.h"

struct test_run tr;

void test_name(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char buf[TEST_NAME_SIZE];
	int len;

	tr.test_case_name[0] = '\0';
	tr.test_case_has_test = 0;
	tr.test_case_has_error = 0;

	char* name = tr.test_case_name;
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
		} else if (last == '%' && *fmt == 's') {
			len = snprintf(buf, TEST_NAME_SIZE, "%s", va_arg(args, char*));
			for (int j = 0; j < len; j++) {
				if (i < TEST_NAME_SIZE) name[i++] = buf[j];
			}
		} else {
			if (i < TEST_NAME_SIZE) name[i++] = *fmt;
		}
		last = *fmt;
		fmt++;
	}

	if (i > TEST_NAME_SIZE - 1) {
		i = TEST_NAME_SIZE - 1;
	}
	name[i++] = '\0';

	va_end(args);
}

void panic()
{
	fprintf(stderr, "Exiting because of assertion error.\n");
	exit(1);
}

void test_called()
{
	tr.check_count++;
	if (!tr.test_case_has_test) {
		tr.test_case_count++;
		tr.test_case_has_test = 1;
	}
}

void error_triggered()
{
	tr.check_error_count++;
	if (!tr.test_case_has_error) {
		tr.test_case_error_count++;
		tr.test_case_has_error = 1;
		printf("%s:\n", tr.test_case_name);
	}
}

void print_results()
{
	printf("\ntest case count: %d\n", tr.test_case_count);
	printf("test case error count: %d\n\n", tr.test_case_error_count);

	printf("check count: %d\n", tr.check_count);
	printf("check error count: %d\n", tr.check_error_count);
}

void assert_int_equal(int a, int b, char* message)
{
	test_called();

	if (a == b) return;
	printf("%d = %d assertion error: %s\n", a, b, message);

	error_triggered();
	panic();
}

void assert_true(int value, char* message)
{
	test_called();

	if (value) return;
	error_triggered();
	printf("%d = true assertion error: %s\n", value, message);
	panic();
}

void assert_ptr(void* p, char* message)
{
	test_called();

	if (p) return;
	error_triggered();
	printf("%p != NULL assertion error: %s\n", p, message);
	panic();
}

void assert_null(void* p, char* message)
{
	test_called();

	if (p == NULL) return;
	error_triggered();
	printf("%p == null assertion error: %s\n", p, message);
	panic();
}

void assert_ok(enum result r, char* message)
{
	test_called();

	if (r == result_ok) return;
	error_triggered();
	printf("ok assertion error: %s: %s\n", message, error_message);
	panic();
}

void assert_error(enum result r, char* message)
{
	test_called();

	if (r == result_error) return;
	error_triggered();
	printf("no error detected assertion error: %s\n", message);
	panic();
}

void expect_int_equal(int a, int b, char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	printf("%d = %d error: %s\n", a, b, message);
}

void expect_char_equal(char a, char b, char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	printf("(%c) = (%c) error: %s\n", a, b, message);
}

void expect_uint_equal(unsigned int a, unsigned int b, char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	printf("(%u) = (%u) error: %s\n", a, b, message);
}

void expect_size_t_equal(size_t a, size_t b, char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	printf("(%zu) = (%zu) error: %s\n", a, b, message);
}

void expect_true(int value, char* message)
{
	test_called();
	if (value) return;
	error_triggered();
	printf("%d = true error: %s\n", value, message);
}

void expect_false(int value, char* message)
{
	test_called();
	if (!value) return;
	error_triggered();
	printf("(%d) = (false) error: %s\n", value, message);
}

void expect_ptr(void* p, char* message)
{
	test_called();
	if (p) return;
	error_triggered();
	printf("%p != NULL error: %s\n", p, message);
}

void expect_null(void* p, char* message)
{
	test_called();
	if (p == NULL) return;
	error_triggered();
	printf("%p == null error: %s\n", p, message);
}

void expect_ok(enum result r, char* message)
{
	test_called();
	if (r == result_ok) return;
	error_triggered();
	printf("ok error: %s: %s\n", message, error_message);
}

void expect_str(struct buffer* a, char* b, char* message)
{
	test_called();
	if (buffer_str_compare(a, b)) return;
	error_triggered();
	struct allocator al;
	allocator_init(&al);
	char* temp;
	enum result r = buffer2array(&al, a, &temp);
	if (r == result_error) {
		printf("<string> equals %s error: %s\n", b, message);
	} else {
		printf("%s equals %s error: %s\n", temp, b, message);
	}
	allocator_destroy(&al);
}

void expect_strcmp(char* a, char* b, char* message)
{
	test_called();
	error_triggered();
	if (strcmp(a, b) == 0) return;
}

void expect_utf8_char(char* a, char* b, char* message)
{
	test_called();
	int count_a = NUM_BYTES(a[0]);
	int count_b = NUM_BYTES(b[0]);
	if (count_a == count_b) {
		for (int i = 0; i < count_a; i++) {
			if (a[i] != b[i]) {
				break;
			}
		}
		return;
	}
	error_triggered();
	printf("utf8 chars not equal: %s\n", message);
}

void expect_utf32_char(UChar32 a, UChar32 b, char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	printf("(%d:%c) = (%d:%c) utf32 error: %s\n", a, a, b, b, message);
}

void expect_error_message(char* s)
{
	test_called();
	if (strcmp(error_message, s) == 0) return;
	error_triggered();
	printf("(%s) = (%s): error message does not match\n", error_message, s);
}
