#ifndef _TEST_SCAN_H
#define _TEST_SCAN_H

#include "setup.h"
#include "assert.h"
#include "alba/token.h"
#include "alba/scan.h"

void setup_scan(char* line)
{
	setup(line);
	enum result r;
	r = scan(&s, &tl);
	assert_ok(r, "scan");
}

void test_scan_addition()
{
	test_name(__func__);

	setup_scan("speed + 1");

	struct token* t0 = get_token(tl.head, 0);
	assert_ptr(t0, "get token");
	expect_int_equal(t0->type, token_word, "is word");
	expect_str(&t0->value, "speed", "speed");

	struct token* t1 = get_token(tl.head, 1);
	assert_ptr(t1, "get token");
	expect_int_equal(t1->type, token_plus, "is plus");

	struct token* t2 = get_token(tl.head, 2);
	assert_ptr(t2, "get token");
	expect_int_equal(t2->type, token_number, "is number");
	expect_str(&t2->value, "1", "1");

	struct token* t3 = get_token(tl.head, 3);
	expect_null(t3, "no 3rd argument");

	teardown();
}

void test_scan_subtraction()
{
	test_name(__func__);

	setup_scan("100 - delta");

	struct token* t0 = get_token(tl.head, 0);
	assert_ptr(t0, "get token");
	expect_int_equal(t0->type, token_number, "is number");
	expect_str(&t0->value, "100", "100");

	struct token* t1 = get_token(tl.head, 1);
	assert_ptr(t1, "get token");
	expect_int_equal(t1->type, token_minus, "is minus");

	struct token* t2 = get_token(tl.head, 2);
	assert_ptr(t2, "get token");
	expect_int_equal(t2->type, token_word, "is word");
	expect_str(&t2->value, "delta", "delta");

	struct token* t3 = get_token(tl.head, 3);
	expect_null(t3, "no 3rd argument");

	teardown();
}

void test_scan_multiplication()
{
	test_name(__func__);

	setup_scan("100 * 20");

	struct token* t0 = get_token(tl.head, 0);
	assert_ptr(t0, "get token");
	expect_int_equal(t0->type, token_number, "is number");
	expect_str(&t0->value, "100", "100");

	struct token* t1 = get_token(tl.head, 1);
	assert_ptr(t1, "get token");
	expect_int_equal(t1->type, token_mult, "is mult");

	struct token* t2 = get_token(tl.head, 2);
	assert_ptr(t2, "get token");
	expect_int_equal(t2->type, token_number, "is number");
	expect_str(&t2->value, "20", "20");

	struct token* t3 = get_token(tl.head, 3);
	expect_null(t3, "no 3rd argument");

	teardown();
}

void test_scan_divide()
{
	test_name(__func__);

	setup_scan("45 / 11");
	struct token* t0 = get_token(tl.head, 0);
	assert_ptr(t0, "get token");
	expect_int_equal(t0->type, token_number, "is number");
	expect_str(&t0->value, "45", "45");

	struct token* t1 = get_token(tl.head, 1);
	assert_ptr(t1, "get token");
	expect_int_equal(t1->type, token_divide, "divide");

	struct token* t2 = get_token(tl.head, 2);
	assert_ptr(t2, "get token");
	expect_int_equal(t2->type, token_number, "is number");
	expect_str(&t2->value, "11", "11");

	struct token* t3 = get_token(tl.head, 3);
	expect_null(t3, "no 3rd argument");

	teardown();
}

void test_scan()
{
	test_scan_addition();
	test_scan_subtraction();
	test_scan_multiplication();
	test_scan_divide();
}

#endif
