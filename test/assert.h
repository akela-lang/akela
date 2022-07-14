#ifndef _ASSERT_H
#define _ASSERT_H

#include "alba/buffer.h"

#define TEST_NAME_SIZE 100

struct test_run {
	char test_case_name[TEST_NAME_SIZE];
	int test_case_count;
	int test_case_error_count;
	int test_case_has_test;
	int test_case_has_error;
	int check_count;
	int check_error_count;
};

void panic();
void test_called();
void error_triggered();

void assert_int_equal(int a, int b, char* message);
void assert_true(int value, char* message);
void assert_ptr(void* p, char* message);
void assert_null(void* p, char* message);
void assert_ok(enum result r, char* message);
void assert_error(enum result r, char* message);
void expect_int_equal(int a, int b, char* message);
void expect_char_equal(char a, char b, char* message);
void expect_uint_equal(unsigned int a, unsigned int b, char* message);
void expect_size_t_equal(size_t a, size_t b, char* message);
void expect_true(int value, char* message);
void expect_false(int value, char* message);
void expect_ptr(void* p, char* message);
void expect_null(void* p, char* message);
void expect_ok(enum result r, char* message);
void expect_str(struct buffer* a, char* b, char* message);
void expect_strcmp(char* a, char* b, char* message);
void expect_nts(char* a, char* b, char* message);
void test_name(const char* fmt, ...);
void print_results();
void expect_error_message(char* s);

#endif
