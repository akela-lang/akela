#ifndef _TEST_PARSE_H
#define _TEST_PARSE_H

#include "setup.h"
#include "assert.h"
#include "alba/dag.h"
#include "alba/scan.h"
#include "alba/parse.h"

void setup_parse(char* line)
{
	setup(line);
	enum result r;
	r = scan(&s, &tl);
	assert_ok(r, "scan");
	r = parse(&tl, &root);
	assert_ok(r, "parse");
}

void test_parse_num()
{
	test_name(__func__);

	setup_parse("32");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_number, "number");
	expect_str(&root->value, "32", "32");

	teardown();
}

void test_parse_word()
{
	test_name(__func__);

	setup_parse("lot");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_word, "word");
	expect_str(&root->value, "lot", "lot");

	teardown();
}

void test_parse_num_negative()
{
	test_name(__func__);

	setup_parse("-30");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_sign, "sign");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, dag_type_minus, "minus");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "30", "30");

	teardown();
}

void test_parse_num_positive()
{
	test_name(__func__);

	setup_parse("+30");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_sign, "sign");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, dag_type_plus, "plus");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "30", "30");

	teardown();
}

void test_parse_add()
{
	test_name(__func__);

	setup_parse("speed + 1");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_word, "word");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(dag_get_child(root, 2), "only 2 children");

	teardown();
}

void test_parse_add_positive()
{
	test_name(__func__);

	setup_parse("speed + +1");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_word, "word");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");
	
	assert_null(dag_get_child(root, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_plus, "plus2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	teardown();
}

void test_parse_add_negative()
{
	test_name(__func__);

	setup_parse("speed + -1");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_word, "word");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(root, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_minus, "minus");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	teardown();
}

void test_parse_sub()
{
	test_name(__func__);

	setup_parse("100 - delta");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_minus, "minus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "100", "100");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_word, "word");
	expect_str(&right->value, "delta", "delta");

	assert_null(dag_get_child(root, 2), "only 2 children");

	teardown();
}

void test_parse_sub_positive()
{
	test_name(__func__);

	setup_parse("speed - +1");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_minus, "minus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_word, "word");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(root, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_plus, "plus");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	teardown();
}

void test_parse_sub_negative()
{
	test_name(__func__);

	setup_parse("speed - -1");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_minus, "minus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_word, "word");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(root, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_minus, "minus2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	teardown();
}


void test_parse_mult()
{
	test_name(__func__);

	setup_parse("5 * 2");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "5", "5");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "2", "2");

	teardown();
}

void test_parse_mult_positive()
{
	test_name(__func__);

	setup_parse("speed * +1");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_word, "word");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(root, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_plus, "plus");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	teardown();
}

void test_parse_mult_negative()
{
	test_name(__func__);

	setup_parse("speed * -1");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_word, "word");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(root, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_minus, "minus2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	teardown();
}


void test_parse_divide()
{
	test_name(__func__);

	setup_parse("52 / 2");

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_divide, "divide");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "52", "52");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "2", "2");

	teardown();
}

void test_parse_add_add()
{
	test_name(__func__);

	setup_parse("1 + 2 + 3");

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "1", "1");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right ->type, dag_type_plus, "plus 2");

	assert_null(dag_get_child(root,2), "null");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "right");
	expect_int_equal(left2->type, dag_type_number, "number 2");
	expect_str(&left2->value, "2", "2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	teardown();
}

void test_parse_mult_mult()
{
	test_name(__func__);

	setup_parse("1 * 2 * 3");

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "1", "1");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_mult, "mult 2");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left 2");
	expect_int_equal(left2->type, dag_type_number, "number 2");
	expect_str(&left2->value, "2", "2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	teardown();
}

void test_parse_add_mult()
{
	test_name(__func__);

	setup_parse("5 + 3 * 2");

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "5", "5");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_mult, "mult");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_number, "number 2");
	expect_str(&left2->value, "3", "3");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_number, "number 3");
	expect_str(&right2->value, "2", "2");

	teardown();
}

void test_parse_mult_add()
{
	test_name(__func__);

	setup_parse("4 * 3 + 2");

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_mult, "mult");

	struct dag_node* left2 = dag_get_child(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_number, "number 4");
	expect_str(&left2->value, "4", "4");

	struct dag_node* right2 = dag_get_child(left, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number 2");
	expect_str(&right->value, "2", "2");

	teardown();
}

void test_parse()
{
	test_parse_num();
	test_parse_word();
	test_parse_num_negative();
	test_parse_num_positive();
	test_parse_add();
	test_parse_add_positive();
	test_parse_add_negative();
	test_parse_sub();
	test_parse_sub_positive();
	test_parse_sub_negative();
	test_parse_mult();
	test_parse_mult_positive();
	test_parse_mult_negative();
	test_parse_divide();
	test_parse_add_add();
	test_parse_mult_mult();
	test_parse_add_mult();
	test_parse_mult_add();
}

#endif
