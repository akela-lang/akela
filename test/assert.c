#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "alba/result.h"
#include "alba/buffer.h"

int test_case_count = 0;
int test_case_error_count = 0;
int test_case_has_test = 0;
int test_case_has_error = 0;

int check_count = 0;
int check_error_count = 0;

#define TEST_NAME_SIZE 100
void test_name(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char name[TEST_NAME_SIZE] = "";
	char buf[TEST_NAME_SIZE];
	int len;

	test_case_has_test = 0;
	test_case_has_error = 0;

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

	printf("%s\n", name);
}

void panic()
{
	fprintf(stderr, "Exiting because of assertion error.\n");
	exit(1);
}

void test_called()
{
	check_count++;
	if (!test_case_has_test) {
		test_case_count++;
		test_case_has_test = 1;
	}
}

void error_triggered()
{
	check_error_count++;
	if (!test_case_has_error) {
		test_case_error_count++;
		test_case_has_error = 1;
	}
}

void print_results()
{
	printf("\ntest case count: %d\n", test_case_count);
	printf("test case error count: %d\n\n", test_case_error_count);

	printf("check count: %d\n", check_count);
	printf("check error count: %d\n", check_error_count);
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
	printf("%d = true assertion error: %s\n", value, message);
	error_triggered();
	panic();
}

void assert_ptr(void* p, char* message)
{
	test_called();

	if (p) return;
	printf("%p != NULL assertion error: %s\n", p, message);

	error_triggered();
	panic();
}

void assert_null(void* p, char* message)
{
	test_called();

	if (p == NULL) return;
	printf("%p == null assertion error: %s\n", p, message);
	error_triggered();
	panic();
}

void assert_ok(enum result r, char* message)
{
	test_called();

	if (r == result_ok) return;
	printf("ok assertion error: %s: %s\n", message, error_message);
	error_triggered();
	panic();
}

void assert_error(enum result r, char* message)
{
	test_called();

	if (r == result_error) return;
	printf("no error detected assertion error: %s\n", message);
	error_triggered();
	panic();
}

void expect_int_equal(int a, int b, char* message)
{
	test_called();
	if (a == b) return;
	printf("%d = %d error: %s\n", a, b, message);
	error_triggered();
}

void expect_char_equal(char a, char b, char* message)
{
	test_called();
	if (a == b) return;
	printf("(%c) = (%c) error: %s\n", a, b, message);
	error_triggered();
}

void expect_uint_equal(unsigned int a, unsigned int b, char* message)
{
	test_called();
	if (a == b) return;
	printf("%d = %d error: %s\n", a, b, message);
	error_triggered();
}

void expect_true(int value, char* message)
{
	test_called();
	if (value) return;
	printf("%d = true error: %s\n", value, message);
	error_triggered();
}

void expect_false(int value, char* message)
{
	test_called();
	if (!value) return;
	printf("(%d) = (false) error: %s\n", value, message);
	error_triggered();
}

void expect_ptr(void* p, char* message)
{
	test_called();
	if (p) return;
	printf("%p != NULL error: %s\n", p, message);
	error_triggered();
}

void expect_null(void* p, char* message)
{
	test_called();
	if (p == NULL) return;
	printf("%p == null error: %s\n", p, message);
	error_triggered();
}

void expect_ok(enum result r, char* message)
{
	test_called();
	if (r == result_ok) return;
	printf("ok error: %s: %s\n", message, error_message);
	error_triggered();
}

void expect_str(struct buffer* a, char* b, char* message)
{
	test_called();
	if (buffer_str_compare(a, b)) return;
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
	error_triggered();
}

void expect_error_message(char* s)
{
	test_called();
	if (strcmp(error_message, s) == 0) return;
	printf("(%s) = (%s): error message does not match\n", error_message, s);
	error_triggered();

}
