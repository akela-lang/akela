#ifndef _TEST_SCAN_H
#define _TEST_SCAN_H

#include "assert.h"
#include "alba/token.h"
#include "alba/scan.h"
#include "alba/allocator.h"

enum result scan_setup(struct allocator* al, char* line, struct input_state* is)
{
	enum result r;

	allocator_init(al);

	struct string* s;
	r = allocator_malloc(al, &s, sizeof(struct string));
	assert_ok(r, "allocator malloc string");
	string_init(s);
	r = array2string(al, line, s);
	assert_ok(r, "ok");

	struct string_data* sd;
	r = allocator_malloc(al, &sd, sizeof(struct string_data));
	assert_ok(r, "allocator_malloc string_data");
	string_data_init(s, sd);

	UConverter* conv;
	r = conv_open(&conv);
	assert_ok(r, "conv_open");
	input_state_init(string_getchar, sd, conv, is);
	return r;
}

void scan_teardown(struct allocator* al, struct input_state* is)
{
	conv_close(is->conv);
	allocator_destroy(al);
}

void test_scan_assign()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token t;
	int have_token;

	scan_setup(&al, "a = 1", &is);

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "scan_get_token 0");
	expect_int_equal(t.type, token_word, "word");
	expect_str(&t.value, "a", "a");
	assert_true(have_token, "have token 0");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "scan get_token 1");
	expect_int_equal(t.type, token_equal, "equal");
	assert_true(have_token, "have token 1");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "scan_get_token2");
	expect_int_equal(t.type, token_number, "number");
	expect_str(&t.value, "1", "1");
	assert_true(have_token, "have token 2");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "scan_get_token3");
	assert_true(!have_token, "no token 3");

	scan_teardown(&al, &is);
}

void test_scan_num()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token t;
	int have_token;

	scan_setup(&al, "11", &is);

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "scan_get_token");
	expect_int_equal(t.type, token_number, "number");
	assert_true(have_token, "have token");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "scan_get_token");
	assert_true(!have_token, "no token");

	scan_teardown(&al, &is);
}

void test_scan_addition()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token t;
	int have_token;

	scan_setup(&al, "speed + 1", &is);

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t.type, token_word, "word");
	expect_str(&t.value, "speed", "speed");
	assert_true(have_token, "have token 0");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t.type, token_plus, "plus");
	assert_true(have_token, "have token 1");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t.type, token_number, "number");
	expect_str(&t.value, "1", "1");
	assert_true(have_token, "have token 2");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "scan_get_token3");
	assert_true(!have_token, "no token 3");

	scan_teardown(&al, &is);
}

void test_scan_subtraction()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token t;
	int have_token;

	scan_setup(&al, "100 - delta", &is);

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t.type, token_number, "number");
	expect_str(&t.value, "100", "100");
	assert_true(have_token, "have token 0");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t.type, token_minus, "minus");
	assert_true(have_token, "have token 1");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t.type, token_word, "word");
	expect_str(&t.value, "delta", "delta");
	assert_true(have_token, "have token 2");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "scan_get_token3");
	assert_true(!have_token, "no token 3");

	scan_teardown(&al, &is);
}

void test_scan_multiplication()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token t;
	int have_token;

	scan_setup(&al, "100 * 20", &is);

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t.type, token_number, "number");
	expect_str(&t.value, "100", "100");
	assert_true(have_token, "have token 0");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t.type, token_mult, "mult");
	assert_true(have_token, "have token 1");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t.type, token_number, "number");
	expect_str(&t.value, "20", "20");
	assert_true(have_token, "have token 2");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "scan_get_token3");
	assert_true(!have_token, "no token 3");

	scan_teardown(&al, &is);
}

void test_scan_divide()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token t;
	int have_token;

	scan_setup(&al, "45 / 11", &is);

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t.type, token_number, "number");
	expect_str(&t.value, "45", "45");
	assert_true(have_token, "have token 0");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t.type, token_divide, "divide");
	assert_true(have_token, "have token 1");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t.type, token_number, "is number");
	expect_str(&t.value, "11", "11");
	assert_true(have_token, "have token 2");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "scan_get_token3");
	assert_true(!have_token, "no token 3");

	scan_teardown(&al, &is);
}

void test_scan_stmts_expr()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token t;
	int have_token;

	scan_setup(&al, "i + 1\nx * 4", &is);
	
	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t.type, token_word, "word");
	expect_str(&t.value, "i", "i");
	assert_true(have_token, "have token 0");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t.type, token_plus, "plus");
	assert_true(have_token, "have token 1");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t.type, token_number, "number");
	expect_str(&t.value, "1", "1");
	assert_true(have_token, "have token 2");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 3");
	expect_int_equal(t.type, token_newline, "newline");
	assert_true(have_token, "have token 3");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 4");
	expect_int_equal(t.type, token_word, "word2");
	expect_str(&t.value, "x", "x");
	assert_true(have_token, "have token 4");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 5");
	expect_int_equal(t.type, token_mult, "mult");
	assert_true(have_token, "have token 5");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 6");
	expect_int_equal(t.type, token_number, "number2");
	expect_str(&t.value, "4", "4");
	assert_true(have_token, "have token 6");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "scan_get_token7");
	assert_true(!have_token, "no token 7");

	scan_teardown(&al, &is);
}

void test_scan_stmts_expr2()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token t;
	int have_token;

	scan_setup(&al, "i + 1\nx * 4\n", &is);

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t.type, token_word, "word");
	expect_str(&t.value, "i", "i");
	assert_true(have_token, "have token 0");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t.type, token_plus, "plus");
	assert_true(have_token, "have token 1");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t.type, token_number, "number");
	expect_str(&t.value, "1", "1");
	assert_true(have_token, "have token 2");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 3");
	expect_int_equal(t.type, token_newline, "newline");
	assert_true(have_token, "have token 3");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 4");
	expect_int_equal(t.type, token_word, "word2");
	expect_str(&t.value, "x", "x");
	assert_true(have_token, "have token 4");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 5");
	expect_int_equal(t.type, token_mult, "mult");
	assert_true(have_token, "have token 5");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 6");
	expect_int_equal(t.type, token_number, "number2");
	expect_str(&t.value, "4", "4");
	assert_true(have_token, "have token 6");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 7");
	expect_int_equal(t.type, token_newline, "newline2");
	assert_true(have_token, "have token 7");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 8");
	assert_true(!have_token, "no token 8");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "scan_get_token9");
	assert_true(!have_token, "no token 9");

	scan_teardown(&al, &is);
}

void test_scan_stmts_assign()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token t;
	int have_token;

	scan_setup(&al, "i + 1\nx = 4", &is);

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t.type, token_word, "word");
	expect_str(&t.value, "i", "i");
	assert_true(have_token, "have token 0");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t.type, token_plus, "plus");
	assert_true(have_token, "have token 1");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t.type, token_number, "number");
	expect_str(&t.value, "1", "1");
	assert_true(have_token, "have token 2");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 3");
	expect_int_equal(t.type, token_newline, "newline");
	assert_true(have_token, "have token 3");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 4");
	expect_int_equal(t.type, token_word, "word2");
	expect_str(&t.value, "x", "x");
	assert_true(have_token, "have token 4");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 5");
	expect_int_equal(t.type, token_equal, "equal");
	assert_true(have_token, "have token 5");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "get token 6");
	expect_int_equal(t.type, token_number, "number2");
	expect_str(&t.value, "4", "4");
	assert_true(have_token, "have token 6");

	r = scan_get_token(&al, &is, &have_token, &t);
	assert_ok(r, "scan_get_token7");
	assert_true(!have_token, "no token 7");

	scan_teardown(&al, &is);
}

void test_scan()
{
	test_scan_assign();
	test_scan_num();
	test_scan_addition();
	test_scan_subtraction();
	test_scan_multiplication();
	test_scan_divide();
	test_scan_stmts_expr();
	test_scan_stmts_expr2();
	test_scan_stmts_assign();
}

#endif
