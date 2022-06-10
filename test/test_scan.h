#ifndef _TEST_SCAN_H
#define _TEST_SCAN_H

#include "assert.h"
#include "alba/token.h"
#include "alba/scan.h"
#include "alba/allocator.h"

enum result scan_setup(struct allocator* al, char* line, struct token_list *tl)
{
	enum result r;
	struct string s;
	allocator_init(al);
	token_list_init(tl);
	string_init(&s);
	r = array2string(al, line, &s);
	assert_ok(r, "ok");
	r = scan(al, &s, tl);
	assert_ok(r, "scan");
	return r;
}

void scan_teardown(struct allocator* al)
{
	allocator_destroy(al);
}

void test_scan_assign()
{
	test_name(__func__);

	struct allocator al;
	struct token_list tl;
	scan_setup(&al, "a = 1", &tl);
	scan_teardown(&al);
}

void test_scan_num()
{
	test_name(__func__);

	struct allocator al;
	struct token_list tl;
	scan_setup(&al, "11", &tl);

	struct token* t0 = get_token(tl.head, 0);
	assert_ptr(t0, "get token");
	expect_int_equal(t0->type, token_number, "number");

	scan_teardown(&al);
}

void test_scan_addition()
{
	test_name(__func__);

	struct allocator al;
	struct token_list tl;
	scan_setup(&al, "speed + 1", &tl);

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

	scan_teardown(&al);
}

void test_scan_subtraction()
{
	test_name(__func__);

	struct allocator al;
	struct token_list tl;
	scan_setup(&al, "100 - delta", &tl);

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

	scan_teardown(&al);
}

void test_scan_multiplication()
{
	test_name(__func__);

	struct allocator al;
	struct token_list tl;
	scan_setup(&al, "100 * 20", &tl);

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

	scan_teardown(&al);
}

void test_scan_divide()
{
	test_name(__func__);

	struct allocator al;
	struct token_list tl;

	scan_setup(&al, "45 / 11", &tl);
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

	scan_teardown(&al);
}

void test_scan_stmts_expr()
{
	test_name(__func__);

	struct allocator al;
	struct token_list tl;

	scan_setup(&al, "i + 1\nx * 4", &tl);
	
	struct token* t0 = get_token(tl.head, 0);
	assert_ptr(t0, "ptr0");
	expect_int_equal(t0->type, token_word, "word");
	expect_str(&t0->value, "i", "i");

	struct token* t1 = get_token(tl.head, 1);
	assert_ptr(t1, "ptr1");
	expect_int_equal(t1->type, token_plus, "plus");

	struct token* t2 = get_token(tl.head, 2);
	assert_ptr(t2, "ptr2");
	expect_int_equal(t2->type, token_number, "number");
	expect_str(&t2->value, "1", "1");

	struct token* t3 = get_token(tl.head, 3);
	assert_ptr(t3, "ptr3");
	expect_int_equal(t3->type, token_newline, "newline");

	struct token* t4 = get_token(tl.head, 4);
	assert_ptr(t4, "ptr4");
	expect_int_equal(t4->type, token_word, "word2");
	expect_str(&t4->value, "x", "x");

	struct token* t5 = get_token(tl.head, 5);
	assert_ptr(t5, "ptr5");
	expect_int_equal(t5->type, token_mult, "mult");

	struct token* t6 = get_token(tl.head, 6);
	assert_ptr(t6, "ptr6");
	expect_int_equal(t6->type, token_number, "number2");
	expect_str(&t6->value, "4", "4");

	scan_teardown(&al);
}

void test_scan_stmts_expr2()
{
	test_name(__func__);

	struct allocator al;
	struct token_list tl;

	scan_setup(&al, "i + 1\nx * 4\n", &tl);

	struct token* t0 = get_token(tl.head, 0);
	assert_ptr(t0, "ptr0");
	expect_int_equal(t0->type, token_word, "word");
	expect_str(&t0->value, "i", "i");

	struct token* t1 = get_token(tl.head, 1);
	assert_ptr(t1, "ptr1");
	expect_int_equal(t1->type, token_plus, "plus");

	struct token* t2 = get_token(tl.head, 2);
	assert_ptr(t2, "ptr2");
	expect_int_equal(t2->type, token_number, "number");
	expect_str(&t2->value, "1", "1");

	struct token* t3 = get_token(tl.head, 3);
	assert_ptr(t3, "ptr3");
	expect_int_equal(t3->type, token_newline, "newline");

	struct token* t4 = get_token(tl.head, 4);
	assert_ptr(t4, "ptr4");
	expect_int_equal(t4->type, token_word, "word2");
	expect_str(&t4->value, "x", "x");

	struct token* t5 = get_token(tl.head, 5);
	assert_ptr(t5, "ptr5");
	expect_int_equal(t5->type, token_mult, "mult");

	struct token* t6 = get_token(tl.head, 6);
	assert_ptr(t6, "ptr6");
	expect_int_equal(t6->type, token_number, "number2");
	expect_str(&t6->value, "4", "4");

	struct token* t7 = get_token(tl.head, 7);
	assert_ptr(t7, "ptr7");
	expect_int_equal(t7->type, token_newline, "newline2");

	scan_teardown(&al);
}

void test_scan_stmts_assign()
{
	test_name(__func__);

	struct allocator al;
	struct token_list tl;

	scan_setup(&al, "i + 1\nx = 4", &tl);

	struct token* t0 = get_token(tl.head, 0);
	assert_ptr(t0, "ptr0");
	expect_int_equal(t0->type, token_word, "word");
	expect_str(&t0->value, "i", "i");

	struct token* t1 = get_token(tl.head, 1);
	assert_ptr(t1, "ptr1");
	expect_int_equal(t1->type, token_plus, "plus");

	struct token* t2 = get_token(tl.head, 2);
	assert_ptr(t2, "ptr2");
	expect_int_equal(t2->type, token_number, "number");
	expect_str(&t2->value, "1", "1");

	struct token* t3 = get_token(tl.head, 3);
	assert_ptr(t3, "ptr3");
	expect_int_equal(t3->type, token_newline, "newline");

	struct token* t4 = get_token(tl.head, 4);
	assert_ptr(t4, "ptr4");
	expect_int_equal(t4->type, token_word, "word2");
	expect_str(&t4->value, "x", "x");

	struct token* t5 = get_token(tl.head, 5);
	assert_ptr(t5, "ptr5");
	expect_int_equal(t5->type, token_equal, "equal");

	struct token* t6 = get_token(tl.head, 6);
	assert_ptr(t6, "ptr6");
	expect_int_equal(t6->type, token_number, "number2");
	expect_str(&t6->value, "4", "4");

	scan_teardown(&al);
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
