#include <stdbool.h>
#include "zinc/unit_test.h"
#include "test_parse.h"
#include "alba/ast.h"
#include "alba/parse_tools.h"
#include "alba/unit_test_compiler.h"
#include "alba/type_node.h"
#include "alba/type_info.h"

/* dynamic-output-none */
void test_parse_blank()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	root = check_stmts(root, "stmts root");

	assert_null(root, "null root");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_add()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var speed::Int64 = 100; speed + 1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* add = ast_node_get(root, 1);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, ast_type_plus, "plus add");

	struct type_node* tn = add->tn;
	assert_ptr(tn, "ptr tn");

	struct type_info* ti = tn->ti;
	assert_ptr(ti, "ptr ti");
	expect_int_equal(ti->type, type_integer, "integer ti");
	expect_str(&ti->name, "Int64", "Int64 ti");

	struct ast_node* left = ast_node_get(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct ast_node* right = ast_node_get(add, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "1", "1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_add_mixed_types()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("1 + 5.0", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* add = ast_node_get(root, 0);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, ast_type_plus, "plus add");

	struct type_node* add_tn = add->tn;
	assert_ptr(add_tn, "ptr add_tn");

	struct type_info* add_ti = add_tn->ti;
	assert_ptr(add_ti, "ptr add_ti");
	expect_int_equal(add_ti->type, type_float, "float add_ti");
	expect_str(&add_ti->name, "Float64", "Float64 add_ti");

	struct ast_node* left = ast_node_get(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_number, "number left");
	expect_str(&left->value, "1", "1 left");

	struct ast_node* right = ast_node_get(add, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "5.0", "5.0 right");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_add_positive()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var speed::Int64 = 100; speed + +1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* add = ast_node_get(root, 1);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, ast_type_plus, "plus add");

	struct ast_node* left = ast_node_get(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct ast_node* right = ast_node_get(add, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_sign, "sign");

	assert_null(ast_node_get(root, 2), "only 2 children");

	struct ast_node* left2 = ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_plus, "plus2");

	struct ast_node* right2 = ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_add_negative()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; speed + -1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* add = ast_node_get(root, 1);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, ast_type_plus, "plus add");

	struct ast_node* left = ast_node_get(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct ast_node* right = ast_node_get(add, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_sign, "sign");

	assert_null(ast_node_get(add, 2), "only 2 children");

	struct ast_node* left2 = ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_minus, "minus");

	struct ast_node* right2 = ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_sub()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var delta::Int64 = 3; 100 - delta", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* sub = ast_node_get(root, 1);
	assert_ptr(sub, "ptr sub");
	expect_int_equal(sub->type, ast_type_minus, "minus sub");

	struct ast_node* left = ast_node_get(sub, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_number, "number");
	expect_str(&left->value, "100", "100");

	struct ast_node* right = ast_node_get(sub, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_id, "id");
	expect_str(&right->value, "delta", "delta");

	assert_null(ast_node_get(root, 2), "only 2 children");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_sub_positive()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; speed - +1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* sub = ast_node_get(root, 1);
	assert_ptr(sub, "root");
	expect_int_equal(sub->type, ast_type_minus, "minus sub");

	struct ast_node* left = ast_node_get(sub, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct ast_node* right = ast_node_get(sub, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_sign, "sign");

	assert_null(ast_node_get(sub, 2), "only 2 children");

	struct ast_node* left2 = ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_plus, "plus");

	struct ast_node* right2 = ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_sub_negative()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; speed - -1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* sub = ast_node_get(root, 1);
	assert_ptr(sub, "ptr sub");
	expect_int_equal(sub->type, ast_type_minus, "minus sub");

	struct ast_node* left = ast_node_get(sub, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct ast_node* right = ast_node_get(sub, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_sign, "sign");

	assert_null(ast_node_get(sub, 2), "only 2 children");

	struct ast_node* left2 = ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_minus, "minus2");

	struct ast_node* right2 = ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}


/* dynamic-output-none */
void test_parse_mult()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("5 * 2", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* mult = ast_node_get(root, 0);
	assert_ptr(mult, "root");
	expect_int_equal(mult->type, ast_type_mult, "mult mult");

	struct type_node* tn = mult->tn;
	assert_ptr(tn, "ptr tn");
	
	struct type_info* ti = tn->ti;
	assert_ptr(ti, "ptr ti");
	expect_int_equal(ti->type, type_integer, "integer ti");
	expect_str(&ti->name, "Int64", "Int64 ti");

	struct ast_node* left = ast_node_get(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_number, "number");
	expect_str(&left->value, "5", "5");

	struct ast_node* right = ast_node_get(mult, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "2", "2");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_mult_positive()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; speed * +1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* mult = ast_node_get(root, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, ast_type_mult, "mult mult");

	struct ast_node* left = ast_node_get(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct ast_node* right = ast_node_get(mult, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_sign, "sign");

	assert_null(ast_node_get(mult, 2), "only 2 children");

	struct ast_node* left2 = ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_plus, "plus");

	struct ast_node* right2 = ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_mult_negative()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; speed * -1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* mult = ast_node_get(root, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, ast_type_mult, "mult mult");

	struct ast_node* left = ast_node_get(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct ast_node* right = ast_node_get(mult, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_sign, "sign");

	assert_null(ast_node_get(mult, 2), "only 2 children");

	struct ast_node* left2 = ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_minus, "minus2");

	struct ast_node* right2 = ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_divide()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("52 / 2", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* divide = ast_node_get(root, 0);
	assert_ptr(divide, "ptr divide");
	assert_int_equal(divide->type, ast_type_divide, "divide divide");

	struct ast_node* left = ast_node_get(divide, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_number, "number");
	expect_str(&left->value, "52", "52");

	struct ast_node* right = ast_node_get(divide, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "2", "2");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_add_add()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("1 + 2 + 3", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* plus0 = ast_node_get(root, 0);
	assert_ptr(plus0, "ptr plus0");
	assert_int_equal(plus0->type, ast_type_plus, "plus plus0");

	struct ast_node* left = ast_node_get(plus0, 0);
	assert_ptr(left, "ptr left");
	expect_int_equal(left->type, ast_type_plus, "plus left");

	struct ast_node* left2 = ast_node_get(left, 0);
	assert_ptr(left2, "ptr left2");
	expect_int_equal(left2->type, ast_type_number, "number left2");
	expect_str(&left2->value, "1", "1 left2");

	struct ast_node* right = ast_node_get(left, 1);
	assert_ptr(right, "ptr right");
	expect_int_equal(right->type, ast_type_number, "number right");
	expect_str(&right->value, "2", "2 right");

	struct ast_node* right2 = ast_node_get(plus0, 1);
	assert_ptr(right2, "ptr right2");
	expect_int_equal(right2->type, ast_type_number, "number right2");
	expect_str(&right2->value, "3", "3 right2");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_mult_mult()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("1 * 2 * 3", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* mult = ast_node_get(root, 0);
	assert_ptr(mult, "ptr mult");
	assert_int_equal(mult->type, ast_type_mult, "mult mult");

	struct ast_node* left = ast_node_get(mult, 0);
	assert_ptr(left, "ptr left");
	expect_int_equal(left->type, ast_type_mult, "mult left");

	struct ast_node* left2 = ast_node_get(left, 0);
	assert_ptr(left2, "ptr left2");
	expect_int_equal(left2->type, ast_type_number, "number left2");
	expect_str(&left2->value, "1", "1 left2");

	struct ast_node* right = ast_node_get(left, 1);
	assert_ptr(right, "ptr right");
	expect_int_equal(right->type, ast_type_number, "number right");
	expect_str(&right->value, "2", "2 right");

	struct ast_node* right2 = ast_node_get(mult, 1);
	assert_ptr(right2, "ptr right2");
	expect_int_equal(right2->type, ast_type_number, "number right2");
	expect_str(&right2->value, "3", "3 right2");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_add_mult()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("5 + 3 * 2", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "root");
	assert_int_equal(root->type, ast_type_plus, "plus");

	struct ast_node* left = ast_node_get(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_number, "number");
	expect_str(&left->value, "5", "5");

	struct ast_node* right = ast_node_get(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_mult, "mult");

	struct ast_node* left2 = ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_number, "number 2");
	expect_str(&left2->value, "3", "3");

	struct ast_node* right2 = ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, ast_type_number, "number 3");
	expect_str(&right2->value, "2", "2");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_mult_add()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("4 * 3 + 2", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* plus = ast_node_get(root, 0);
	assert_ptr(plus, "ptr plus");
	assert_int_equal(plus->type, ast_type_plus, "plus plus");

	struct ast_node* left = ast_node_get(plus, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_mult, "mult");

	struct ast_node* left2 = ast_node_get(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_number, "number 4");
	expect_str(&left2->value, "4", "4");

	struct ast_node* right2 = ast_node_get(left, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, ast_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	struct ast_node* right = ast_node_get(plus, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number 2");
	expect_str(&right->value, "2", "2");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_add()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; (speed + 1)", &ps, &root);
	expect_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* paren = ast_node_get(root, 1);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, ast_type_parenthesis, "parenthesis paren");

	struct ast_node* plus = ast_node_get(paren, 0);
	assert_ptr(plus, "root");
	expect_int_equal(plus->type, ast_type_plus, "plus");

	struct ast_node* left = ast_node_get(plus, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct ast_node* right = ast_node_get(plus, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(ast_node_get(root, 2), "only 2 children");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_add2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; (speed) + 1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* plus = ast_node_get(root, 1);
	assert_ptr(plus, "root");
	expect_int_equal(plus->type, ast_type_plus, "plus");

	struct ast_node* paren = ast_node_get(plus, 0);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, ast_type_parenthesis, "parenthesis paren");

	struct ast_node* left = ast_node_get(paren, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct ast_node* right = ast_node_get(plus, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(ast_node_get(root, 2), "only 2 children");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_add3()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var speed::Int64; speed + (1)", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* plus = ast_node_get(root, 1);
	assert_ptr(plus, "root");
	expect_int_equal(plus->type, ast_type_plus, "plus");

	struct ast_node* left = ast_node_get(plus, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct ast_node* paren = ast_node_get(plus, 1);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, ast_type_parenthesis, "parenthesis paren");

	struct ast_node* right = ast_node_get(paren, 0);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "1", "1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_add_add()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("1 + (2 + 3)", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* plus = ast_node_get(root, 0);
	assert_ptr(plus, "ptr plus");
	assert_int_equal(plus->type, ast_type_plus, "plus plus");

	struct ast_node* left = ast_node_get(plus, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_number, "number");
	expect_str(&left->value, "1", "1");

	struct ast_node* paren = ast_node_get(plus, 1);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, ast_type_parenthesis, "parenthesis paren");

	struct ast_node* right = ast_node_get(paren, 0);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_plus, "plus 2");

	assert_null(ast_node_get(root, 2), "null");

	struct ast_node* left2 = ast_node_get(right, 0);
	assert_ptr(left2, "right");
	expect_int_equal(left2->type, ast_type_number, "number 2");
	expect_str(&left2->value, "2", "2");

	struct ast_node* right2 = ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, ast_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_add_add2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("(1 + 2) + 3", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* plus = ast_node_get(root, 0);
	assert_ptr(plus, "ptr plus");
	assert_int_equal(plus->type, ast_type_plus, "plus plus");

	struct ast_node* paren = ast_node_get(plus, 0);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, ast_type_parenthesis, "parenthesis paren");

	struct ast_node* left = ast_node_get(paren, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_plus, "plus 2");

	struct ast_node* left2 = ast_node_get(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_number, "number");
	expect_str(&left2->value, "1", "1");

	struct ast_node* right = ast_node_get(left, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number 2");
	expect_str(&right->value, "2", "2");

	struct ast_node* right2 = ast_node_get(plus, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, ast_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_mult()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("(5 * 2)", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* paren = ast_node_get(root, 0);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, ast_type_parenthesis, "parenthesis paren");

	struct ast_node* mult = ast_node_get(paren, 0);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, ast_type_mult, "mult mult");

	struct ast_node* left = ast_node_get(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_number, "number");
	expect_str(&left->value, "5", "5");

	struct ast_node* right = ast_node_get(mult, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "2", "2");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_mult_mult()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("1 * (2 * 3)", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* mult = ast_node_get(root, 0);
	assert_ptr(mult, "ptr mult");
	assert_int_equal(mult->type, ast_type_mult, "mult mult");

	struct ast_node* left = ast_node_get(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_number, "number");
	expect_str(&left->value, "1", "1");

	struct ast_node* paren = ast_node_get(mult, 1);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, ast_type_parenthesis, "parenthesis paren");

	struct ast_node* right = ast_node_get(paren, 0);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_mult, "mult 2");

	struct ast_node* left2 = ast_node_get(right, 0);
	assert_ptr(left2, "left 2");
	expect_int_equal(left2->type, ast_type_number, "number 2");
	expect_str(&left2->value, "2", "2");

	struct ast_node* right2 = ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, ast_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_mult_mult2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("(1 * 2) * 3", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* mult = ast_node_get(root, 0);
	assert_ptr(mult, "ptr mult");
	assert_int_equal(mult->type, ast_type_mult, "mult mult");

	struct ast_node* paren = ast_node_get(mult, 0);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, ast_type_parenthesis, "parenthesis paren");

	struct ast_node* left = ast_node_get(paren, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_mult, "mult 2");

	struct ast_node* left2 = ast_node_get(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_number, "number");
	expect_str(&left2->value, "1", "1");

	struct ast_node* right = ast_node_get(left, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number 2");
	expect_str(&right->value, "2", "2");

	struct ast_node* right2 = ast_node_get(mult, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, ast_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_comparison()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var count::Int64\ncount == 10\ncount != 11\ncount <= 12\ncount >= 13", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	struct ast_node* cond0 = ast_node_get(root, 1);
	assert_ptr(cond0, "ptr cond0");
	expect_int_equal(cond0->type, ast_type_equality, "equality cond0");

	struct ast_node* left0 = ast_node_get(cond0, 0);
	assert_ptr(left0, "ptr left0");
	expect_int_equal(left0->type, ast_type_id, "id left0");
	expect_str(&left0->value, "count", "count left0");

	struct ast_node* right0 = ast_node_get(cond0, 1);
	assert_ptr(right0, "ptr right0");
	expect_int_equal(right0->type, ast_type_number, "number right0");
	expect_str(&right0->value, "10", "10 right0");

	struct ast_node* cond1 = ast_node_get(root, 2);
	assert_ptr(cond1, "ptr cond1");
	expect_int_equal(cond1->type, ast_type_not_equal, "not equal cond1");

	struct ast_node* left1 = ast_node_get(cond1, 0);
	assert_ptr(left1, "ptr left1");
	expect_int_equal(left1->type, ast_type_id, "id left1");
	expect_str(&left1->value, "count", "count left1");

	struct ast_node* right1 = ast_node_get(cond1, 1);
	assert_ptr(right1, "ptr right1");
	expect_int_equal(right1->type, ast_type_number, "number right1");
	expect_str(&right1->value, "11", "11 right1");

	struct ast_node* cond2 = ast_node_get(root, 3);
	assert_ptr(cond2, "ptr cond2");
	expect_int_equal(cond2->type, ast_type_less_than_or_equal, "less than or equal cond2");

	struct ast_node* left2 = ast_node_get(cond2, 0);
	assert_ptr(left2, "ptr left2");
	expect_int_equal(left2->type, ast_type_id, "id left2");
	expect_str(&left2->value, "count", "count left2");

	struct ast_node* right2 = ast_node_get(cond2, 1);
	assert_ptr(right2, "ptr right2");
	expect_int_equal(right2->type, ast_type_number, "number right2");
	expect_str(&right2->value, "12", "12 right2");

	struct ast_node* cond3 = ast_node_get(root, 4);
	assert_ptr(cond3, "ptr cond3");
	expect_int_equal(cond3->type, ast_type_greater_than_or_equal, "greater than or equal cond3");

	struct ast_node* left3 = ast_node_get(cond3, 0);
	assert_ptr(left3, "ptr left3");
	expect_int_equal(left3->type, ast_type_id, "id left3");
	expect_str(&left3->value, "count", "count left3");

	struct ast_node* right3 = ast_node_get(cond3, 1);
	assert_ptr(right3, "ptr right3");
	expect_int_equal(right3->type, ast_type_number, "number right3");
	expect_str(&right3->value, "13", "13 right3");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_and()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var a::Bool; var b::Bool; a && b", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");

	struct ast_node* and = ast_node_get(root, 2);
	assert_ptr(and, "ptr and");
	expect_int_equal(and->type, ast_type_and, "and and");

	struct ast_node* a = ast_node_get(and, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	struct ast_node* b = ast_node_get(and, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id b");
	expect_str(&b->value, "b", "b b");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_or()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var a::Int64; var b::Int64; a || b", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* or = ast_node_get(root, 2);
	assert_ptr(or, "ptr or");
	expect_int_equal(or->type, ast_type_or, "or or");

	struct ast_node* a = ast_node_get(or, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	struct ast_node* b = ast_node_get(or, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id b");
	expect_str(&b->value, "b", "b b");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_or_or()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var a::Int64; var b::Int64; var c::Int64; a || b || c", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* or0 = ast_node_get(root, 3);
	assert_ptr(or0 , "ptr or0");
	expect_int_equal(or0->type, ast_type_or, "or or0");

	struct ast_node* or1 = ast_node_get(or0, 0);
	assert_ptr(or1, "ptr or1");
	expect_int_equal(or1->type, ast_type_or, "or or1");

	struct ast_node* a = ast_node_get(or1, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	struct ast_node* b = ast_node_get(or1, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id b");
	expect_str(&b->value, "b", "b b");

	struct ast_node* c = ast_node_get(or0, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, ast_type_id, "id c");
	expect_str(&c->value, "c", "c c");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_array_subscript()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Vector{Int64}; a[1]", &ps, &root);
	expect_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* as = ast_node_get(root, 1);
	assert_ptr(as, "ptr as");
	expect_int_equal(as->type, ast_type_array_subscript, "array-subscript as");

	struct ast_node* name = ast_node_get(as, 0);
	assert_ptr(name, "ptr name");
	expect_int_equal(name->type, ast_type_id, "id name");
	expect_str(&name->value, "a", "a name");

	struct ast_node* index = ast_node_get(as, 1);
	assert_ptr(index, "ptr index");
	expect_int_equal(index->type, ast_type_number, "number index");
	expect_str(&index->value, "1", "1 index");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_array_subscript2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Vector{Int64}; var b::Vector{Int64}; a[b[1]]", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	struct ast_node* a = ast_node_get(root, 2);
	assert_ptr(a, "ptr a");
	assert_int_equal(a->type, ast_type_array_subscript, "array-subscript a");

	struct ast_node* a0 = ast_node_get(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, ast_type_id, "id a0");
	expect_str(&a0->value, "a", "a a0");

	struct ast_node* b = ast_node_get(a, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_array_subscript, "array-subscript b");

	struct ast_node* b0 = ast_node_get(b, 0);
	assert_ptr(b0, "ptr b0");
	expect_int_equal(b0->type, ast_type_id, "id b0");
	expect_str(&b0->value, "b", "b b0");

	struct ast_node* b1 = ast_node_get(b, 1);
	assert_ptr(b1, "ptr b1");
	expect_int_equal(b1->type, ast_type_number, "number b1");
	expect_str(&b1->value, "1", "1 b1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_array_subscript3()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Vector{Int64}; a[1][2]", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	struct ast_node* a = ast_node_get(root, 1);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_array_subscript, "array-subscript a");

	struct ast_node* a0 = ast_node_get(a, 0);
	expect_int_equal(a0->type, ast_type_id, "id a0");
	expect_str(&a0->value, "a", "a a0");

	struct ast_node* a1 = ast_node_get(a, 1);
	expect_int_equal(a1->type, ast_type_number, "number a1");
	expect_str(&a1->value, "1", "1 a1");

	struct ast_node* a2 = ast_node_get(a, 2);
	expect_int_equal(a2->type, ast_type_number, "number a2");
	expect_str(&a2->value, "2", "2 a1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_var()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Int32", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse valid");

	struct ast_node* var = check_stmts(root, "stmts root");
	assert_ptr(var, "ptr var");
	expect_int_equal(var->type, ast_type_var, "var");

	struct ast_node* dec = ast_node_get(var, 0);
	assert_ptr(dec, "ptr dec");
	expect_int_equal(dec->type, ast_type_declaration, "declaration");

	struct ast_node* id = ast_node_get(dec, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "a", "a");

	struct ast_node* type_node = ast_node_get(dec, 1);
	assert_ptr(type_node, "ptr type_node");
	expect_int_equal(type_node->type, ast_type_type, "type type");

	struct type_node* tn = type_node->tn;
	assert_ptr(tn, "ptr tn");

	struct type_info* ti = tn->ti;
	assert_ptr(ti, "ptrt ti");
	expect_str(&ti->name, "Int32", "Int32 ti");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_var2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Int32 = 1", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse valid");

	struct ast_node* var = check_stmts(root, "stmts root");
	assert_ptr(var, "ptr var");
	expect_int_equal(var->type, ast_type_var, "var");

	struct ast_node* dec = ast_node_get(var, 0);
	assert_ptr(dec, "ptr dec");
	expect_int_equal(dec->type, ast_type_declaration, "declaration");

	struct ast_node* id = ast_node_get(dec, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id");
	expect_str(&id->value, "a", "a");

	struct ast_node* type_node = ast_node_get(dec, 1);
	assert_ptr(type_node, "ptr type_node");
	expect_int_equal(type_node->type, ast_type_type, "type type_node");

	struct type_node* tn = type_node->tn;
	assert_ptr(tn, "ptr tn");

	struct type_info* ti = tn->ti;
	assert_ptr(ti, "ptrt ti");
	expect_str(&ti->name, "Int32", "Int32 ti");

	struct ast_node* value = ast_node_get(var, 1);
	assert_ptr(value, "ptr value");
	expect_int_equal(value->type, ast_type_number, "number");
	expect_str(&value->value, "1", "1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_assign_string()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::String; a = \"hello\"", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* assign = ast_node_get(root, 1);
	assert_ptr(assign, "ptr assign");
	expect_int_equal(assign->type, ast_type_assign, "assign assign");

	struct ast_node* lhv = ast_node_get(assign, 0);
	assert_ptr(lhv, "ptr lhv");
	expect_int_equal(lhv->type, ast_type_id, "id lhv");
	expect_str(&lhv->value, "a", "a lhv");

	struct ast_node* rhv = ast_node_get(assign, 1);
	assert_ptr(rhv, "ptr rhv");
	expect_int_equal(rhv->type, ast_type_string, "string rhv");
	expect_str(&rhv->value, "hello", "hello rhv");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_expression()
{
	test_parse_blank();
	test_parse_add();
	test_parse_add_mixed_types();
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
	test_parse_paren_add();
	test_parse_paren_add2();
	test_parse_paren_add3();
	test_parse_paren_add_add();
	test_parse_paren_add_add2();
	test_parse_paren_mult();
	test_parse_paren_mult_mult();
	test_parse_paren_mult_mult2();
	test_parse_comparison();
	test_parse_and();
	test_parse_or();
	test_parse_or_or();
	test_parse_array_subscript();
	test_parse_array_subscript2();
	test_parse_array_subscript3();
	test_parse_var();
	test_parse_var2();
	test_parse_assign_string();
}
