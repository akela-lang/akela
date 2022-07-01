#ifndef _TESTING_H
#define _TESTING_H

void assert_int_equal(int a, int b, char* message);
void assert_true(int value, char* message);
void assert_ptr(void* p, char* message);
void assert_null(void* p, char* message);
void assert_ok(enum result r, char* message);
void assert_error(enum result r, char* message);
void expect_int_equal(int a, int b, char* message);
void expect_true(int value, char* message);
void expect_ptr(void* p, char* message);
void expect_null(void* p, char* message);
void expect_ok(enum result r, char* message);
void expect_str(struct buffer* a, char* b, char* message);
void test_name(const char* fmt, ...);
void print_results();
void expect_error_message(char* s);

#endif
