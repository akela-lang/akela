#include <stdbool.h>
#include "zinc/unit_test.h"
#include "test_parse.h"
#include "alba/dag.h"
#include "alba/parse_tools.h"
#include "assert_compiler.h"

/* dynamic-output-none */
void test_parse_blank()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	root = check_stmts(root, "stmts root");

	assert_null(root, "null root");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_add()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var speed::Int64 = 100; speed + 1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* add = dag_get_child(root, 1);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, dag_type_plus, "plus add");

	struct dag_node* left = dag_get_child(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(add, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(dag_get_child(root, 2), "only 2 children");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_add_positive()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var speed::Int64 = 100; speed + +1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* add = dag_get_child(root, 1);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, dag_type_plus, "plus add");

	struct dag_node* left = dag_get_child(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(add, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(root, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_plus, "plus2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_add_negative()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; speed + -1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* add = dag_get_child(root, 1);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, dag_type_plus, "plus add");

	struct dag_node* left = dag_get_child(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(add, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(add, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_minus, "minus");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_sub()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var delta::Int64 = 3; 100 - delta", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* sub = dag_get_child(root, 1);
	assert_ptr(sub, "ptr sub");
	expect_int_equal(sub->type, dag_type_minus, "minus sub");

	struct dag_node* left = dag_get_child(sub, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "100", "100");

	struct dag_node* right = dag_get_child(sub, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_id, "id");
	expect_str(&right->value, "delta", "delta");

	assert_null(dag_get_child(root, 2), "only 2 children");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_sub_positive()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; speed - +1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* sub = dag_get_child(root, 1);
	assert_ptr(sub, "root");
	expect_int_equal(sub->type, dag_type_minus, "minus sub");

	struct dag_node* left = dag_get_child(sub, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(sub, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(sub, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_plus, "plus");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_sub_negative()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; speed - -1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* sub = dag_get_child(root, 1);
	assert_ptr(sub, "ptr sub");
	expect_int_equal(sub->type, dag_type_minus, "minus sub");

	struct dag_node* left = dag_get_child(sub, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(sub, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(sub, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_minus, "minus2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}


/* dynamic-output-none */
void test_parse_mult()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("5 * 2", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* mult = dag_get_child(root, 0);
	assert_ptr(mult, "root");
	expect_int_equal(mult->type, dag_type_mult, "mult mult");

	struct dag_node* left = dag_get_child(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "5", "5");

	struct dag_node* right = dag_get_child(mult, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "2", "2");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_mult_positive()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; speed * +1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* mult = dag_get_child(root, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, dag_type_mult, "mult mult");

	struct dag_node* left = dag_get_child(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(mult, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(mult, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_plus, "plus");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_mult_negative()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; speed * -1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* mult = dag_get_child(root, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, dag_type_mult, "mult mult");

	struct dag_node* left = dag_get_child(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(mult, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(mult, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_minus, "minus2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_divide()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("52 / 2", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* divide = dag_get_child(root, 0);
	assert_ptr(divide, "ptr divide");
	assert_int_equal(divide->type, dag_type_divide, "divide divide");

	struct dag_node* left = dag_get_child(divide, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "52", "52");

	struct dag_node* right = dag_get_child(divide, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "2", "2");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_add_add()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("1 + 2 + 3", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* plus0 = dag_get_child(root, 0);
	assert_ptr(plus0, "ptr plus0");
	assert_int_equal(plus0->type, dag_type_plus, "plus plus0");

	struct dag_node* left = dag_get_child(plus0, 0);
	assert_ptr(left, "ptr left");
	expect_int_equal(left->type, dag_type_plus, "plus left");

	struct dag_node* left2 = dag_get_child(left, 0);
	assert_ptr(left2, "ptr left2");
	expect_int_equal(left2->type, dag_type_number, "number left2");
	expect_str(&left2->value, "1", "1 left2");

	struct dag_node* right = dag_get_child(left, 1);
	assert_ptr(right, "ptr right");
	expect_int_equal(right->type, dag_type_number, "number right");
	expect_str(&right->value, "2", "2 right");

	struct dag_node* right2 = dag_get_child(plus0, 1);
	assert_ptr(right2, "ptr right2");
	expect_int_equal(right2->type, dag_type_number, "number right2");
	expect_str(&right2->value, "3", "3 right2");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_mult_mult()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("1 * 2 * 3", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* mult = dag_get_child(root, 0);
	assert_ptr(mult, "ptr mult");
	assert_int_equal(mult->type, dag_type_mult, "mult mult");

	struct dag_node* left = dag_get_child(mult, 0);
	assert_ptr(left, "ptr left");
	expect_int_equal(left->type, dag_type_mult, "mult left");

	struct dag_node* left2 = dag_get_child(left, 0);
	assert_ptr(left2, "ptr left2");
	expect_int_equal(left2->type, dag_type_number, "number left2");
	expect_str(&left2->value, "1", "1 left2");

	struct dag_node* right = dag_get_child(left, 1);
	assert_ptr(right, "ptr right");
	expect_int_equal(right->type, dag_type_number, "number right");
	expect_str(&right->value, "2", "2 right");

	struct dag_node* right2 = dag_get_child(mult, 1);
	assert_ptr(right2, "ptr right2");
	expect_int_equal(right2->type, dag_type_number, "number right2");
	expect_str(&right2->value, "3", "3 right2");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_add_mult()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("5 + 3 * 2", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	root = check_stmts(root, "stmts root");

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

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_mult_add()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("4 * 3 + 2", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	root = check_stmts(root, "stmts root");

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

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_num()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("(32)", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_number, "number");
	expect_str(&root->value, "32", "32");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_add()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; (speed + 1)", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* plus = dag_get_child(root, 1);
	assert_ptr(plus, "root");
	expect_int_equal(plus->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(plus, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(plus, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(dag_get_child(root, 2), "only 2 children");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_add2()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; (speed) + 1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* plus = dag_get_child(root, 1);
	assert_ptr(plus, "root");
	expect_int_equal(plus->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(plus, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(plus, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(dag_get_child(root, 2), "only 2 children");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_add3()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; speed + (1)", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* plus = dag_get_child(root, 1);
	assert_ptr(plus, "root");
	expect_int_equal(plus->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(plus, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(plus, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_add_add()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("1 + (2 + 3)", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	root = check_stmts(root, "stmts root");

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

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_add_add2()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("(1 + 2) + 3", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	root = check_stmts(root, "stmts root");

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

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_mult()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("(5 * 2)", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	root = check_stmts(root, "stmts root");

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

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_mult_mult()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("1 * (2 * 3)", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	root = check_stmts(root, "stmts root");

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

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_mult_mult2()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("(1 * 2) * 3", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	root = check_stmts(root, "stmts root");

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

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_comparison()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var count::Int64\ncount == 10\ncount != 11\ncount <= 12\ncount >= 13", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	struct dag_node* cond0 = dag_get_child(root, 1);
	assert_ptr(cond0, "ptr cond0");
	expect_int_equal(cond0->type, dag_type_equality, "equality cond0");

	struct dag_node* left0 = dag_get_child(cond0, 0);
	assert_ptr(left0, "ptr left0");
	expect_int_equal(left0->type, dag_type_id, "id left0");
	expect_str(&left0->value, "count", "count left0");

	struct dag_node* right0 = dag_get_child(cond0, 1);
	assert_ptr(right0, "ptr right0");
	expect_int_equal(right0->type, dag_type_number, "number right0");
	expect_str(&right0->value, "10", "10 right0");

	struct dag_node* cond1 = dag_get_child(root, 2);
	assert_ptr(cond1, "ptr cond1");
	expect_int_equal(cond1->type, dag_type_not_equal, "not equal cond1");

	struct dag_node* left1 = dag_get_child(cond1, 0);
	assert_ptr(left1, "ptr left1");
	expect_int_equal(left1->type, dag_type_id, "id left1");
	expect_str(&left1->value, "count", "count left1");

	struct dag_node* right1 = dag_get_child(cond1, 1);
	assert_ptr(right1, "ptr right1");
	expect_int_equal(right1->type, dag_type_number, "number right1");
	expect_str(&right1->value, "11", "11 right1");

	struct dag_node* cond2 = dag_get_child(root, 3);
	assert_ptr(cond2, "ptr cond2");
	expect_int_equal(cond2->type, dag_type_less_than_or_equal, "less than or equal cond2");

	struct dag_node* left2 = dag_get_child(cond2, 0);
	assert_ptr(left2, "ptr left2");
	expect_int_equal(left2->type, dag_type_id, "id left2");
	expect_str(&left2->value, "count", "count left2");

	struct dag_node* right2 = dag_get_child(cond2, 1);
	assert_ptr(right2, "ptr right2");
	expect_int_equal(right2->type, dag_type_number, "number right2");
	expect_str(&right2->value, "12", "12 right2");

	struct dag_node* cond3 = dag_get_child(root, 4);
	assert_ptr(cond3, "ptr cond3");
	expect_int_equal(cond3->type, dag_type_greater_than_or_equal, "greater than or equal cond3");

	struct dag_node* left3 = dag_get_child(cond3, 0);
	assert_ptr(left3, "ptr left3");
	expect_int_equal(left3->type, dag_type_id, "id left3");
	expect_str(&left3->value, "count", "count left3");

	struct dag_node* right3 = dag_get_child(cond3, 1);
	assert_ptr(right3, "ptr right3");
	expect_int_equal(right3->type, dag_type_number, "number right3");
	expect_str(&right3->value, "13", "13 right3");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_not()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var a::Bool; !a", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* not = dag_get_child(root, 1);
	assert_ptr(not, "ptr not");
	expect_int_equal(not->type, dag_type_not, "not not");

	struct dag_node* id = dag_get_child(not, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, dag_type_id, "id id");
	expect_str(&id->value, "a", "a id");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_and()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var a::Bool; var b::Bool; a && b", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");

	struct dag_node* and = dag_get_child(root, 2);
	assert_ptr(and, "ptr and");
	expect_int_equal(and->type, dag_type_and, "and and");

	struct dag_node* a = dag_get_child(and, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	struct dag_node* b = dag_get_child(and, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_id, "id b");
	expect_str(&b->value, "b", "b b");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_or()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var a::Int64; var b::Int64; a || b", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* or = dag_get_child(root, 2);
	assert_ptr(or, "ptr or");
	expect_int_equal(or->type, dag_type_or, "or or");

	struct dag_node* a = dag_get_child(or, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	struct dag_node* b = dag_get_child(or, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_id, "id b");
	expect_str(&b->value, "b", "b b");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_or_or()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var a::Int64; var b::Int64; var c::Int64; a || b || c", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* or0 = dag_get_child(root, 3);
	assert_ptr(or0 , "ptr or0");
	expect_int_equal(or0->type, dag_type_or, "or or0");

	struct dag_node* or1 = dag_get_child(or0, 0);
	assert_ptr(or1, "ptr or1");
	expect_int_equal(or1->type, dag_type_or, "or or1");

	struct dag_node* a = dag_get_child(or1, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	struct dag_node* b = dag_get_child(or1, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_id, "id b");
	expect_str(&b->value, "b", "b b");

	struct dag_node* c = dag_get_child(or0, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, dag_type_id, "id c");
	expect_str(&c->value, "c", "c c");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_array_literal()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("[1,2,3]", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	struct dag_node* a = check_stmts(root, "stmts root");
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_array_literal, "array-literal a");

	struct dag_node* a0 = dag_get_child(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, dag_type_number, "number a0");
	expect_str(&a0->value, "1", "1 a0");

	struct dag_node* a1 = dag_get_child(a, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, dag_type_number, "number a1");
	expect_str(&a1->value, "2", "2 a1");

	struct dag_node* a2 = dag_get_child(a, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, dag_type_number, "number a2");
	expect_str(&a2->value, "3", "3 a2");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_array_subscript()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Vector{Int64}; a[1]", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* as = dag_get_child(root, 1);
	assert_ptr(as, "ptr as");
	expect_int_equal(as->type, dag_type_array_subscript, "array-subscript as");

	struct dag_node* name = dag_get_child(as, 0);
	assert_ptr(name, "ptr name");
	expect_int_equal(name->type, dag_type_id, "id name");
	expect_str(&name->value, "a", "a name");

	struct dag_node* index = dag_get_child(as, 1);
	assert_ptr(index, "ptr index");
	expect_int_equal(index->type, dag_type_number, "number index");
	expect_str(&index->value, "1", "1 index");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_array_subscript2()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Vector{Int64}; var b::Vector{Int64}; a[b[1]]", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	struct dag_node* a = dag_get_child(root, 2);
	assert_ptr(a, "ptr a");
	assert_int_equal(a->type, dag_type_array_subscript, "array-subscript a");

	struct dag_node* a0 = dag_get_child(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, dag_type_id, "id a0");
	expect_str(&a0->value, "a", "a a0");

	struct dag_node* b = dag_get_child(a, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_array_subscript, "array-subscript b");

	struct dag_node* b0 = dag_get_child(b, 0);
	assert_ptr(b0, "ptr b0");
	expect_int_equal(b0->type, dag_type_id, "id b0");
	expect_str(&b0->value, "b", "b b0");

	struct dag_node* b1 = dag_get_child(b, 1);
	assert_ptr(b1, "ptr b1");
	expect_int_equal(b1->type, dag_type_number, "number b1");
	expect_str(&b1->value, "1", "1 b1");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_array_subscript3()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Vector{Int64}; a[1][2]", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	struct dag_node* a = dag_get_child(root, 1);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_array_subscript, "array-subscript a");

	struct dag_node* a0 = dag_get_child(a, 0);
	expect_int_equal(a0->type, dag_type_id, "id a0");
	expect_str(&a0->value, "a", "a a0");

	struct dag_node* a1 = dag_get_child(a, 1);
	expect_int_equal(a1->type, dag_type_number, "number a1");
	expect_str(&a1->value, "1", "1 a1");

	struct dag_node* a2 = dag_get_child(a, 2);
	expect_int_equal(a2->type, dag_type_number, "number a2");
	expect_str(&a2->value, "2", "2 a1");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

void test_parse_var()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Int32", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse valid");

	struct dag_node* var = check_stmts(root, "stmts root");
	assert_ptr(var, "ptr var");
	expect_int_equal(var->type, dag_type_var, "var");

	struct dag_node* dec = dag_get_child(var, 0);
	assert_ptr(dec, "ptr dec");
	expect_int_equal(dec->type, dag_type_declaration, "declaration");

	struct dag_node* id = dag_get_child(dec, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, dag_type_id, "id id");
	expect_str(&id->value, "a", "a");

	struct dag_node* name = dag_get_child(dec, 1);
	assert_ptr(name, "ptr name");
	expect_int_equal(name->type, dag_type_id, "id name");
	expect_str(&name->value, "Int32", "Int32");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

void test_parse_var2()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Int32 = 1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse valid");

	struct dag_node* let = check_stmts(root, "stmts root");
	assert_ptr(let, "ptr let");
	expect_int_equal(let->type, dag_type_var, "var");

	struct dag_node* dec = dag_get_child(let, 0);
	assert_ptr(dec, "ptr dec");
	expect_int_equal(dec->type, dag_type_declaration, "declaration");

	struct dag_node* id = dag_get_child(dec, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, dag_type_id, "id");
	expect_str(&id->value, "a", "a");

	struct dag_node* name = dag_get_child(dec, 1);
	assert_ptr(name, "ptr name");
	expect_int_equal(name->type, dag_type_id, "id name");
	expect_str(&name->value, "Int32", "Int32");

	struct dag_node* value = dag_get_child(let, 1);
	assert_ptr(value, "ptr value");
	expect_int_equal(value->type, dag_type_number, "number");
	expect_str(&value->value, "1", "1");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_assign_string()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::String; a = \"hello\"", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* assign = dag_get_child(root, 1);
	assert_ptr(assign, "ptr assign");
	expect_int_equal(assign->type, dag_type_assign, "assign assign");

	struct dag_node* lhv = dag_get_child(assign, 0);
	assert_ptr(lhv, "ptr lhv");
	expect_int_equal(lhv->type, dag_type_id, "id lhv");
	expect_str(&lhv->value, "a", "a lhv");

	struct dag_node* rhv = dag_get_child(assign, 1);
	assert_ptr(rhv, "ptr rhv");
	expect_int_equal(rhv->type, dag_type_string, "string rhv");
	expect_str(&rhv->value, "hello", "hello rhv");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_expression()
{
	test_parse_blank();
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
	test_parse_comparison();
	test_parse_not();
	test_parse_and();
	test_parse_or();
	test_parse_or_or();
	test_parse_array_literal();
	test_parse_array_subscript();
	test_parse_array_subscript2();
	test_parse_array_subscript3();
	test_parse_var();
	test_parse_var2();
	test_parse_assign_string();
}
