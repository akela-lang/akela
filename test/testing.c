#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "../src/result.h"
#include "../src/ustring.h"

int test_count = 0;
int error_count = 0;

void panic()
{
	fprintf(stderr, "Exiting because of assertion error.\n");
	exit(1);
}

void assert_int_equal(int a, int b, char* message)
{
	test_count++;
	if (a == b) return;
	printf("%d = %d assertion error: %s\n", a, b, message);
	error_count++;
	panic();
}

void assert_true(int value, char* message)
{
	test_count++;
	if (value) return;
	printf("%d = true assertion error: %s\n", value, message);
	error_count++;
	panic();
}

void assert_ptr(void* p, char* message)
{
	test_count++;
	if (p) return;
	printf("%p != NULL assertion error: %s\n", p, message);
	error_count++;
	panic();
}

void assert_null(void* p, char* message)
{
	test_count++;
	if (p == NULL) return;
	printf("%p == null assertion error: %s\n", p, message);
	error_count++;
	panic();
}

void assert_ok(enum result_enum r, char* message)
{
	test_count++;
	if (r == ok_result) return;
	printf("%d is ok assertion error: %s: %s\n", r, message, error_message);
	error_count++;
	panic();
}

void expect_int_equal(int a, int b, char* message)
{
	test_count++;
	if (a == b) return;
	printf("%d = %d error: %s\n", a, b, message);
	error_count++;
}

void expect_true(int value, char* message)
{
	test_count++;
	if (value) return;
	printf("%d = true error: %s\n", value, message);
	error_count++;
}

void expect_ptr(void* p, char* message)
{
	test_count++;
	if (p) return;
	printf("%p != NULL error: %s\n", p, message);
	error_count++;
}

void expect_null(void* p, char* message)
{
	test_count++;
	if (p == NULL) return;
	printf("%p == null error: %s\n", p, message);
	error_count++;
}

void expect_ok(enum result_enum r, char* message)
{
	test_count++;
	if (r == ok_result) return;
	printf("%d is ok error: %s: %s\n", r, message, error_message);
	error_count++;
}

void expect_str(struct string* a, char* b, char* message)
{
	test_count++;
	if (str_compare(a, b)) return;
	char* temp;
	enum result_enum r = string2array(a, &temp);
	if (r == error_result) {
		printf("<string> equals %s error: %s\n", b, message);
	} else {
		printf("%s equals %s error: %s\n", temp, b, message);
		free(temp);
	}
	error_count++;
}

#define TEST_NAME_SIZE 100
void test_name(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char name[TEST_NAME_SIZE] = "";
	char buf[TEST_NAME_SIZE];
	int len;

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

void print_results()
{
	printf("tests: %d\n", test_count);
	printf("errors: %d\n", error_count);
}
