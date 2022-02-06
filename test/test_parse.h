#ifndef _TEST_PARSE_H
#define _TEST_PARSE_H

#include "setup.h"
#include "alba/dag.h"

void setup_parse(char* line)
{
	setup(line);
	enum result r;
	r = scan(&s, &tl);
	assert_ok(r, "scan");
	r = parse(&tl, &root);
	assert_ok(r, "parse");
}

void test_parse_addition()
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

void test_parse_subtraction()
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

void test_parse_multiplication()
{
	test_name(__func__);

	setup_parse("5 * 2");

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_mult, "mult");

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

void test_parse()
{
	test_parse_addition();
	test_parse_subtraction();
	test_parse_multiplication();
	test_parse_divide();
}

#endif
