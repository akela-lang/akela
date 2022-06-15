#ifndef _TEST_PARSE_H
#define _TEST_PARSE_H

#include "assert.h"
#include "alba/dag.h"
#include "alba/scan.h"
#include "alba/parse.h"
#include "alba/allocator.h"
#include "alba/result.h"
#include "alba/uconv.h"
#include "alba/input.h"
#include "alba/buffer.h"

void parse_setup(struct allocator* al, char* line, struct token_state* ts, struct dag_node** root)
{
	enum result r;

	allocator_init(al);

	struct buffer* bf;
	r = allocator_malloc(al, &bf, sizeof(struct buffer));
	assert_ok(r, "allocator malloc string");
	buffer_init(bf);
	r = array2buffer(al, line, bf);
	assert_ok(r, "ok");

	struct string_data* sd;
	r = allocator_malloc(al, &sd, sizeof(struct string_data));
	assert_ok(r, "allocator_malloc string_data");
	string_data_init(bf, sd);

	UConverter* conv;
	r = conv_open(&conv);
	assert_ok(r, "conv_open");

	struct input_state* is;
	r = allocator_malloc(al, &is, sizeof(struct input_state));
	assert_ok(r, "malloc input state");
	input_state_init(string_getchar, sd, conv, is);

	token_state_init(is, ts);

	r = parse(al, ts, root);
	assert_ok(r, "parse");
}

void parse_teardown(struct allocator* al, struct token_state* ts)
{
	conv_close(ts->is->conv);
	allocator_destroy(al);
}

void test_parse_num()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "32", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_number, "number");
	expect_str(&root->value, "32", "32");

	parse_teardown(&al, &ts);
}

void test_parse_id()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "lot", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_id, "id");
	expect_str(&root->value, "lot", "lot");

	parse_teardown(&al, &ts);
}

void test_parse_num_negative()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "-30", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_sign, "sign");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, dag_type_minus, "minus");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "30", "30");

	parse_teardown(&al, &ts);
}

void test_parse_num_positive()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "+30", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_sign, "sign");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, dag_type_plus, "plus");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "30", "30");

	parse_teardown(&al, &ts);
}

void test_parse_add()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed + 1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(dag_get_child(root, 2), "only 2 children");

	parse_teardown(&al, &ts);
}

void test_parse_add_positive()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed + +1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
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


	parse_teardown(&al, &ts);
}

void test_parse_add_negative()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed + -1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
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


	parse_teardown(&al, &ts);
}

void test_parse_sub()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "100 - delta", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_minus, "minus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "100", "100");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_id, "id");
	expect_str(&right->value, "delta", "delta");

	assert_null(dag_get_child(root, 2), "only 2 children");

	parse_teardown(&al, &ts);
}

void test_parse_sub_positive()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed - +1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_minus, "minus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
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


	parse_teardown(&al, &ts);
}

void test_parse_sub_negative()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed - -1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_minus, "minus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
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


	parse_teardown(&al, &ts);
}


void test_parse_mult()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "5 * 2", &ts, &root);

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

	parse_teardown(&al, &ts);
}

void test_parse_mult_positive()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed * +1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
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


	parse_teardown(&al, &ts);
}

void test_parse_mult_negative()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed * -1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
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


	parse_teardown(&al, &ts);
}


void test_parse_divide()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "52 / 2", &ts, &root);

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

	parse_teardown(&al, &ts);
}

void test_parse_add_add()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "1 + 2 + 3", &ts, &root);

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

	parse_teardown(&al, &ts);
}

void test_parse_mult_mult()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "1 * 2 * 3", &ts, &root);

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

	parse_teardown(&al, &ts);
}

void test_parse_add_mult()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "5 + 3 * 2", &ts, &root);

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

	parse_teardown(&al, &ts);
}

void test_parse_mult_add()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "4 * 3 + 2", &ts, &root);

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

	parse_teardown(&al, &ts);
}

void test_parse_paren_num()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "(32)", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_number, "number");
	expect_str(&root->value, "32", "32");

	parse_teardown(&al, &ts);
}

void test_parse_paren_add()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "(speed + 1)", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(dag_get_child(root, 2), "only 2 children");

	parse_teardown(&al, &ts);
}

void test_parse_paren_add2()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "(speed) + 1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(dag_get_child(root, 2), "only 2 children");

	parse_teardown(&al, &ts);
}

void test_parse_paren_add3()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed + (1)", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(dag_get_child(root, 2), "only 2 children");

	parse_teardown(&al, &ts);
}

void test_parse_paren_add_add()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "1 + (2 + 3)", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "1", "1");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_plus, "plus 2");

	assert_null(dag_get_child(root, 2), "null");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "right");
	expect_int_equal(left2->type, dag_type_number, "number 2");
	expect_str(&left2->value, "2", "2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	parse_teardown(&al, &ts);
}

void test_parse_paren_add_add2()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "(1 + 2) + 3", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_plus, "plus 2");

	struct dag_node* left2 = dag_get_child(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_number, "number");
	expect_str(&left2->value, "1", "1");

	struct dag_node* right = dag_get_child(left, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number 2");
	expect_str(&right->value, "2", "2");

	struct dag_node* right2 = dag_get_child(root, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	parse_teardown(&al, &ts);
}

void test_parse_paren_mult()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "(5 * 2)", &ts, &root);

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

	parse_teardown(&al, &ts);
}

void test_parse_paren_mult_mult()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "1 * (2 * 3)", &ts, &root);

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

	parse_teardown(&al, &ts);
}

void test_parse_paren_mult_mult2()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "(1 * 2) * 3", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_mult, "mult 2");
	
	struct dag_node* left2 = dag_get_child(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_number, "number");
	expect_str(&left2->value, "1", "1");

	struct dag_node* right = dag_get_child(left, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number 2");
	expect_str(&right->value, "2", "2");

	struct dag_node* right2 = dag_get_child(root, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	parse_teardown(&al, &ts);
}

void test_parse_assign()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "a = 1", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_assign, "assign");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "a", "a");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	parse_teardown(&al, &ts);
}

void test_parse_assign2()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "a = 1 + 2", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_assign, "assign");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "a", "a");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_plus, "plus");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_number, "number");
	expect_str(&left2->value, "1", "1");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right");
	expect_int_equal(right2->type, dag_type_number, "number2");
	expect_str(&right2->value, "2", "2");

	parse_teardown(&al, &ts);
}

void test_parse_stmts()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "i + 1\nx * 1", &ts, &root);

	assert_ptr(root, "ptr1");
	expect_int_equal(root->type, dag_type_stmts, "stmts");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_plus, "plus");

	struct dag_node* left2 = dag_get_child(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_id, "id");
	expect_str(&left2->value, "i", "i");

	struct dag_node* right = dag_get_child(left, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	struct dag_node* right2 = dag_get_child(root, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_mult, "mult");

	struct dag_node* left3 = dag_get_child(right2, 0);
	assert_ptr(left3, "left3");
	expect_int_equal(left3->type, dag_type_id, "id");
	expect_str(&left3->value, "x", "x");

	struct dag_node* right3 = dag_get_child(right2, 1);
	assert_ptr(right3, "right3");
	expect_int_equal(right3->type, dag_type_number, "number2");
	expect_str(&right3->value, "1", "1");

	parse_teardown(&al, &ts);
}

void test_parse_stmts2()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "x+1\n5-4\n", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_stmts, "stmts");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_plus, "plus a");

	struct dag_node* b = dag_get_child(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	struct dag_node* c = dag_get_child(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, dag_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	struct dag_node* d = dag_get_child(root, 1);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_minus, "minus d");

	struct dag_node* e = dag_get_child(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, dag_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	struct dag_node* f = dag_get_child(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

	parse_teardown(&al, &ts);
}

void test_parse_stmts3()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "x+1\n5-4\ny+z", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_stmts, "stmts");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_plus, "plus a");

	struct dag_node* b = dag_get_child(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	struct dag_node* c = dag_get_child(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, dag_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	struct dag_node* d = dag_get_child(root, 1);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_minus, "minus d");

	struct dag_node* e = dag_get_child(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, dag_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	struct dag_node* f = dag_get_child(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

	struct dag_node* g = dag_get_child(root, 2);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, dag_type_plus, "plus g");

	struct dag_node* h = dag_get_child(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, dag_type_id, "id h");
	expect_str(&h->value, "y", "y h");

	struct dag_node* i = dag_get_child(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, dag_type_id, "id i");
	expect_str(&i->value, "z", "z c");

	parse_teardown(&al, &ts);
}

void test_parse_function()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "function foo()\nx+1\n5+4\nend", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_function, "function");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_id, "id");

	struct dag_node* b = dag_get_child(root, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_stmts, "stmts");

	struct dag_node* c = dag_get_child(root, 2);
	assert_null(c, "ptr c");

	struct dag_node* d = dag_get_child(b, 0);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_plus, "plus");

	struct dag_node* e = dag_get_child(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, dag_type_id, "id e");
	expect_str(&e->value, "x", "x");

	struct dag_node* f = dag_get_child(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_number, "number f");
	expect_str(&f->value, "1", "1");

	struct dag_node* g = dag_get_child(b, 1);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, dag_type_plus, "plus");

	struct dag_node* h = dag_get_child(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, dag_type_number, "number h");
	expect_str(&h->value, "5", "5");

	struct dag_node* i = dag_get_child(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, dag_type_number, "number i");
	expect_str(&i->value, "4", "4");

	parse_teardown(&al, &ts);
}

void test_parse_function2()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "function foo(x)\nx+1\n5+4\nend", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_function, "function");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_id, "id");

	struct dag_node* seq = dag_get_child(root, 1);
	assert_ptr(seq, "ptr seq");
	expect_int_equal(seq->type, dag_type_seq, "seq");

	struct dag_node* seq_a = dag_get_child(seq, 0);
	assert_ptr(seq_a, "ptr seq a");
	expect_int_equal(seq_a->type, dag_type_id, "id seq_a");
	expect_str(&seq_a->value, "x", "x seq_a");

	struct dag_node* b = dag_get_child(root, 2);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_stmts, "stmts");

	struct dag_node* c = dag_get_child(root, 3);
	assert_null(c, "ptr c");

	struct dag_node* d = dag_get_child(b, 0);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_plus, "plus");

	struct dag_node* e = dag_get_child(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, dag_type_id, "id e");
	expect_str(&e->value, "x", "x");

	struct dag_node* f = dag_get_child(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_number, "number f");
	expect_str(&f->value, "1", "1");

	struct dag_node* g = dag_get_child(b, 1);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, dag_type_plus, "plus");

	struct dag_node* h = dag_get_child(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, dag_type_number, "number h");
	expect_str(&h->value, "5", "5");

	struct dag_node* i = dag_get_child(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, dag_type_number, "number i");
	expect_str(&i->value, "4", "4");

	parse_teardown(&al, &ts);
}

void test_parse_function3()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "function foo(x, y)\nx+1\n5+4\nend", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_function, "function");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_id, "id");

	struct dag_node* seq = dag_get_child(root, 1);
	assert_ptr(seq, "ptr seq");
	expect_int_equal(seq->type, dag_type_seq, "seq");

	struct dag_node* seq_a = dag_get_child(seq, 0);
	assert_ptr(seq_a, "ptr seq a");
	expect_int_equal(seq_a->type, dag_type_id, "id seq_a");
	expect_str(&seq_a->value, "x", "x seq_a");

	struct dag_node* seq_b = dag_get_child(seq, 1);
	assert_ptr(seq_b, "ptr seq b");
	expect_int_equal(seq_b->type, dag_type_id, "id seq_b");
	expect_str(&seq_b->value, "y", "y seq_b");

	struct dag_node* b = dag_get_child(root, 2);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_stmts, "stmts");

	struct dag_node* c = dag_get_child(root, 3);
	assert_null(c, "ptr c");

	struct dag_node* d = dag_get_child(b, 0);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_plus, "plus");

	struct dag_node* e = dag_get_child(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, dag_type_id, "id e");
	expect_str(&e->value, "x", "x");

	struct dag_node* f = dag_get_child(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_number, "number f");
	expect_str(&f->value, "1", "1");

	struct dag_node* g = dag_get_child(b, 1);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, dag_type_plus, "plus");

	struct dag_node* h = dag_get_child(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, dag_type_number, "number h");
	expect_str(&h->value, "5", "5");

	struct dag_node* i = dag_get_child(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, dag_type_number, "number i");
	expect_str(&i->value, "4", "4");

	parse_teardown(&al, &ts);
}

void test_parse_function4()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "function foo(x, y, z)\nx+1\n5+4\nend", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_function, "function");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_id, "id");

	struct dag_node* seq = dag_get_child(root, 1);
	assert_ptr(seq, "ptr seq");
	expect_int_equal(seq->type, dag_type_seq, "seq");

	struct dag_node* seq_a = dag_get_child(seq, 0);
	assert_ptr(seq_a, "ptr seq a");
	expect_int_equal(seq_a->type, dag_type_id, "id seq_a");
	expect_str(&seq_a->value, "x", "x seq_a");

	struct dag_node* seq_b = dag_get_child(seq, 1);
	assert_ptr(seq_b, "ptr seq b");
	expect_int_equal(seq_b->type, dag_type_id, "id seq_b");
	expect_str(&seq_b->value, "y", "y seq_b");

	struct dag_node* seq_c = dag_get_child(seq, 2);
	assert_ptr(seq_c, "ptr seq c");
	expect_int_equal(seq_c->type, dag_type_id, "id seq_c");
	expect_str(&seq_c->value, "z", "z seq_c");

	struct dag_node* b = dag_get_child(root, 2);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_stmts, "stmts");

	struct dag_node* c = dag_get_child(root, 3);
	assert_null(c, "ptr c");

	struct dag_node* d = dag_get_child(b, 0);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_plus, "plus");

	struct dag_node* e = dag_get_child(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, dag_type_id, "id e");
	expect_str(&e->value, "x", "x");

	struct dag_node* f = dag_get_child(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_number, "number f");
	expect_str(&f->value, "1", "1");

	struct dag_node* g = dag_get_child(b, 1);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, dag_type_plus, "plus");

	struct dag_node* h = dag_get_child(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, dag_type_number, "number h");
	expect_str(&h->value, "5", "5");

	struct dag_node* i = dag_get_child(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, dag_type_number, "number i");
	expect_str(&i->value, "4", "4");

	parse_teardown(&al, &ts);
}

void test_parse()
{
	test_parse_num();
	test_parse_id();
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
	test_parse_paren_num();
	test_parse_paren_add();
	test_parse_paren_add2();
	test_parse_paren_add3();
	test_parse_paren_add_add();
	test_parse_paren_add_add2();
	test_parse_paren_mult();
	test_parse_paren_mult_mult();
	test_parse_paren_mult_mult2();
	test_parse_assign();
	test_parse_assign2();
	test_parse_stmts();
	test_parse_stmts2();
	test_parse_stmts3();
	test_parse_function();
	test_parse_function2();
	test_parse_function3();
	test_parse_function4();
}

#endif
