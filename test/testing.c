#include <stdlib.h>
#include "../src/result.h"

int test_count = 0;
int error_count = 0;

void assert_int_equal(int a, int b, char* message)
{
	test_count++;
	if (a == b) return;
	printf("%d = %d error: %s\n", a, b, message);
	error_count++;
}

void assert_true(int value, char* message)
{
	test_count++;
	if (value) return;
	printf("%d = true error: %s\n", value, message);
	error_count++;
}

void assert_ptr(void* p, char* message)
{
	test_count++;
	if (p) return;
	printf("%p != NULL error: %s\n", p, message);
	error_count++;
}

void assert_null(void* p, char* message)
{
	test_count++;
	if (p == NULL) return;
	printf("%p == null error: %s\n", p, message);
	error_count++;
}

void assert_ok(enum result_enum r, char* message)
{
	test_count++;
	if (r == ok_result) return;
	printf("%d is ok error: %s\n", r, message);
	error_count++;
}

void test_name(char* name)
{
	printf("%s\n", name);
}

void print_results()
{
	printf("tests: %d\n", test_count);
	printf("errors: %d\n", error_count);
}
