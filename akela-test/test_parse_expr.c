#include <stdbool.h>
#include "zinc/unit_test.h"
#include "test_parse.h"
#include "akela/ast.h"
#include "akela/parse_tools.h"
#include "akela/type_def.h"
#include "akela/comp_unit.h"
#include "zinc/error_unit_test.h"

void test_parse_blank()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* node = Ast_node_get(cu.root, 0);
	assert_null(node, "null node");

    parse_teardown(&cu);
}

void test_parse_add()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("let speed: i64 = 100; speed + 1", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* add = Ast_node_get(cu.root, 1);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, Ast_type_plus, "plus add");

	Type_use* tu = add->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

	Ast_node* left = Ast_node_get(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	Ast_node* right = Ast_node_get(add, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_number, "number");
	expect_str(&right->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_add_error_expected_term()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("1 +", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "expected term after additive operator");

    parse_teardown(&cu);
}

void test_parse_add_error_left_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; foo() + 1", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "addition operand has no value");

    parse_teardown(&cu);
}

void test_parse_add_error_left_not_numeric()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("true + 1", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "addition on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_add_error_right_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; 1 + foo()", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "addition operand has no value");

    parse_teardown(&cu);
}

void test_parse_add_error_right_not_numeric()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("1 + true", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "addition on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_add_mixed_types()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("1 + 5.0", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* add = Ast_node_get(cu.root, 0);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, Ast_type_plus, "plus add");

	Type_use* add_tu = add->tu;
	assert_ptr(add_tu, "ptr add_tu");

	struct type_def* add_td = add_tu->td;
	assert_ptr(add_td, "ptr add_td");
	expect_int_equal(add_td->type, type_float, "float add_td");
	expect_str(&add_td->name, "f64", "f64 add_td");

	Ast_node* left = Ast_node_get(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_number, "number left");
	expect_str(&left->value, "1", "1 left");

	Ast_node* right = Ast_node_get(add, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_number, "number");
	expect_str(&right->value, "5.0", "5.0 right");

    parse_teardown(&cu);
}

void test_parse_add_positive()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let speed: i64 = 100; speed + +1", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* add = Ast_node_get(cu.root, 1);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, Ast_type_plus, "plus add");

	Ast_node* left = Ast_node_get(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	Ast_node* right = Ast_node_get(add, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_sign, "sign");

	assert_null(Ast_node_get(cu.root, 2), "only 2 children");

	Ast_node* left2 = Ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, Ast_type_plus, "plus2");

	Ast_node* right2 = Ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_add_negative()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let speed: i64; speed + -1", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* add = Ast_node_get(cu.root, 1);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, Ast_type_plus, "plus add");

	Ast_node* left = Ast_node_get(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	Ast_node* right = Ast_node_get(add, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_sign, "sign");

	assert_null(Ast_node_get(add, 2), "only 2 children");

	Ast_node* left2 = Ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, Ast_type_minus, "minus");

	Ast_node* right2 = Ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_sub()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let delta: i64 = 3; 100 - delta", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* sub = Ast_node_get(cu.root, 1);
	assert_ptr(sub, "ptr sub");
	expect_int_equal(sub->type, Ast_type_minus, "minus sub");

	Ast_node* left = Ast_node_get(sub, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_number, "number");
	expect_str(&left->value, "100", "100");

	Ast_node* right = Ast_node_get(sub, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_id, "id");
	expect_str(&right->value, "delta", "delta");

	assert_null(Ast_node_get(cu.root, 2), "only 2 children");

    parse_teardown(&cu);
}

void test_parse_sub_positive()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let speed: i64; speed - +1", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* sub = Ast_node_get(cu.root, 1);
	assert_ptr(sub, "cu.root");
	expect_int_equal(sub->type, Ast_type_minus, "minus sub");

	Ast_node* left = Ast_node_get(sub, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	Ast_node* right = Ast_node_get(sub, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_sign, "sign");

	assert_null(Ast_node_get(sub, 2), "only 2 children");

	Ast_node* left2 = Ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, Ast_type_plus, "plus");

	Ast_node* right2 = Ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_sub_negative()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("let speed: i64; speed - -1", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* sub = Ast_node_get(cu.root, 1);
	assert_ptr(sub, "ptr sub");
	expect_int_equal(sub->type, Ast_type_minus, "minus sub");

	Ast_node* left = Ast_node_get(sub, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	Ast_node* right = Ast_node_get(sub, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_sign, "sign");

	assert_null(Ast_node_get(sub, 2), "only 2 children");

	Ast_node* left2 = Ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, Ast_type_minus, "minus2");

	Ast_node* right2 = Ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_mult()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("5 * 2", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* mult = Ast_node_get(cu.root, 0);
	assert_ptr(mult, "cu.root");
	expect_int_equal(mult->type, Ast_type_mult, "mult mult");

	Type_use* tu = mult->tu;
	assert_ptr(tu, "ptr tu");
	
	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

	Ast_node* left = Ast_node_get(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_number, "number");
	expect_str(&left->value, "5", "5");

	Ast_node* right = Ast_node_get(mult, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_number, "number");
	expect_str(&right->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_mult_error_expected_term()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("5*", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "expected term after operator");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_mult_error_left_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; foo() * 1", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "multiplication operand has no value");

    parse_teardown(&cu);
}

void test_parse_mult_error_left_not_numeric()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("true * 1", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "multiplication on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_mult_error_right_no_value()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("fn foo() end; 1 * foo()", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "multiplication operand has no value");

    parse_teardown(&cu);
}

void test_parse_mult_error_right_not_numeric()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("1 * true", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "multiplication on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_mult_positive()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let speed: i64; speed * +1", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* mult = Ast_node_get(cu.root, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, Ast_type_mult, "mult mult");

	Ast_node* left = Ast_node_get(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	Ast_node* right = Ast_node_get(mult, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_sign, "sign");

	assert_null(Ast_node_get(mult, 2), "only 2 children");

	Ast_node* left2 = Ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, Ast_type_plus, "plus");

	Ast_node* right2 = Ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_mult_negative()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let speed: i64; speed * -1", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* mult = Ast_node_get(cu.root, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, Ast_type_mult, "mult mult");

	Ast_node* left = Ast_node_get(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	Ast_node* right = Ast_node_get(mult, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_sign, "sign");

	assert_null(Ast_node_get(mult, 2), "only 2 children");

	Ast_node* left2 = Ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, Ast_type_minus, "minus2");

	Ast_node* right2 = Ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_divide()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("52 / 2", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* divide = Ast_node_get(cu.root, 0);
	assert_ptr(divide, "ptr divide");
	assert_int_equal(divide->type, Ast_type_divide, "divide divide");

	Ast_node* left = Ast_node_get(divide, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_number, "number");
	expect_str(&left->value, "52", "52");

	Ast_node* right = Ast_node_get(divide, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_number, "number");
	expect_str(&right->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_add_add()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("1 + 2 + 3", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* plus0 = Ast_node_get(cu.root, 0);
	assert_ptr(plus0, "ptr plus0");
	assert_int_equal(plus0->type, Ast_type_plus, "plus plus0");

	Ast_node* left = Ast_node_get(plus0, 0);
	assert_ptr(left, "ptr left");
	expect_int_equal(left->type, Ast_type_plus, "plus left");

	Ast_node* left2 = Ast_node_get(left, 0);
	assert_ptr(left2, "ptr left2");
	expect_int_equal(left2->type, Ast_type_number, "number left2");
	expect_str(&left2->value, "1", "1 left2");

	Ast_node* right = Ast_node_get(left, 1);
	assert_ptr(right, "ptr right");
	expect_int_equal(right->type, Ast_type_number, "number right");
	expect_str(&right->value, "2", "2 right");

	Ast_node* right2 = Ast_node_get(plus0, 1);
	assert_ptr(right2, "ptr right2");
	expect_int_equal(right2->type, Ast_type_number, "number right2");
	expect_str(&right2->value, "3", "3 right2");

    parse_teardown(&cu);
}

void test_parse_mult_mult()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("1 * 2 * 3", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* mult = Ast_node_get(cu.root, 0);
	assert_ptr(mult, "ptr mult");
	assert_int_equal(mult->type, Ast_type_mult, "mult mult");

	Ast_node* left = Ast_node_get(mult, 0);
	assert_ptr(left, "ptr left");
	expect_int_equal(left->type, Ast_type_mult, "mult left");

	Ast_node* left2 = Ast_node_get(left, 0);
	assert_ptr(left2, "ptr left2");
	expect_int_equal(left2->type, Ast_type_number, "number left2");
	expect_str(&left2->value, "1", "1 left2");

	Ast_node* right = Ast_node_get(left, 1);
	assert_ptr(right, "ptr right");
	expect_int_equal(right->type, Ast_type_number, "number right");
	expect_str(&right->value, "2", "2 right");

	Ast_node* right2 = Ast_node_get(mult, 1);
	assert_ptr(right2, "ptr right2");
	expect_int_equal(right2->type, Ast_type_number, "number right2");
	expect_str(&right2->value, "3", "3 right2");

    parse_teardown(&cu);
}

void test_parse_add_mult()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("5 + 3 * 2", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* plus = Ast_node_get(cu.root, 0);
	assert_ptr(plus, "cu.root");
	assert_int_equal(plus->type, Ast_type_plus, "plus");

	Ast_node* left = Ast_node_get(plus, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_number, "number");
	expect_str(&left->value, "5", "5");

	Ast_node* right = Ast_node_get(plus, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_mult, "mult");

	Ast_node* left2 = Ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, Ast_type_number, "number 2");
	expect_str(&left2->value, "3", "3");

	Ast_node* right2 = Ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, Ast_type_number, "number 3");
	expect_str(&right2->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_mult_add()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("4 * 3 + 2", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* plus = Ast_node_get(cu.root, 0);
	assert_ptr(plus, "ptr plus");
	assert_int_equal(plus->type, Ast_type_plus, "plus plus");

	Ast_node* left = Ast_node_get(plus, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_mult, "mult");

	Ast_node* left2 = Ast_node_get(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, Ast_type_number, "number 4");
	expect_str(&left2->value, "4", "4");

	Ast_node* right2 = Ast_node_get(left, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, Ast_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	Ast_node* right = Ast_node_get(plus, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_number, "number 2");
	expect_str(&right->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_power()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("5 ^ 2", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "cu.valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* pow = Ast_node_get(cu.root, 0);
	assert_ptr(pow, "ptr pow");
	expect_int_equal(pow->type, Ast_type_power, "power pow");

	Type_use* tu = pow->tu;
	assert_ptr(tu, "ptr tu");
	
	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

	Ast_node* number0 = Ast_node_get(pow, 0);
	assert_ptr(number0, "ptr number0");
	expect_int_equal(number0->type, Ast_type_number, "number number0");
	expect_str(&number0->value, "5", "5 number0");

	Ast_node* number1 = Ast_node_get(pow, 1);
	assert_ptr(number1, "ptr number1");
	expect_int_equal(number1->type, Ast_type_number, "number number1");
	expect_str(&number1->value, "2", "2 number1");

    parse_teardown(&cu);
}

void test_parse_power_error_expected_term()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("5^", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "expected term after caret");

    parse_teardown(&cu);
}

void test_parse_power_error_left_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; 5 ^ foo()", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "power operand has no value");

    parse_teardown(&cu);
}

void test_parse_power_error_left_not_numeric()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("true ^ 2", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "power on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_power_error_right_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; 5 ^ foo()", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "power operand has no value");

    parse_teardown(&cu);
}

void test_parse_power_error_right_not_numeric()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("5 ^ true", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "power on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_paren_add()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let speed: i64; (speed + 1)", &cu);
	expect_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* paren = Ast_node_get(cu.root, 1);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, Ast_type_parenthesis, "parenthesis paren");

	Ast_node* plus = Ast_node_get(paren, 0);
	assert_ptr(plus, "cu.root");
	expect_int_equal(plus->type, Ast_type_plus, "plus");

	Ast_node* left = Ast_node_get(plus, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	Ast_node* right = Ast_node_get(plus, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(Ast_node_get(cu.root, 2), "only 2 children");

    parse_teardown(&cu);
}

void test_parse_paren_add2()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let speed: i64; (speed) + 1", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* plus = Ast_node_get(cu.root, 1);
	assert_ptr(plus, "cu.root");
	expect_int_equal(plus->type, Ast_type_plus, "plus");

	Ast_node* paren = Ast_node_get(plus, 0);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, Ast_type_parenthesis, "parenthesis paren");

	Ast_node* left = Ast_node_get(paren, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	Ast_node* right = Ast_node_get(plus, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(Ast_node_get(cu.root, 2), "only 2 children");

    parse_teardown(&cu);
}

void test_parse_paren_add3()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let speed: i64; speed + (1)", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* plus = Ast_node_get(cu.root, 1);
	assert_ptr(plus, "cu.root");
	expect_int_equal(plus->type, Ast_type_plus, "plus");

	Ast_node* left = Ast_node_get(plus, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	Ast_node* paren = Ast_node_get(plus, 1);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, Ast_type_parenthesis, "parenthesis paren");

	Ast_node* right = Ast_node_get(paren, 0);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_number, "number");
	expect_str(&right->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_paren_add_add()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("1 + (2 + 3)", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* plus = Ast_node_get(cu.root, 0);
	assert_ptr(plus, "ptr plus");
	assert_int_equal(plus->type, Ast_type_plus, "plus plus");

	Ast_node* left = Ast_node_get(plus, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_number, "number");
	expect_str(&left->value, "1", "1");

	Ast_node* paren = Ast_node_get(plus, 1);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, Ast_type_parenthesis, "parenthesis paren");

	Ast_node* right = Ast_node_get(paren, 0);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_plus, "plus 2");

	assert_null(Ast_node_get(cu.root, 2), "null");

	Ast_node* left2 = Ast_node_get(right, 0);
	assert_ptr(left2, "right");
	expect_int_equal(left2->type, Ast_type_number, "number 2");
	expect_str(&left2->value, "2", "2");

	Ast_node* right2 = Ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, Ast_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

    parse_teardown(&cu);
}

void test_parse_paren_add_add2()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("(1 + 2) + 3", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* plus = Ast_node_get(cu.root, 0);
	assert_ptr(plus, "ptr plus");
	assert_int_equal(plus->type, Ast_type_plus, "plus plus");

	Ast_node* paren = Ast_node_get(plus, 0);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, Ast_type_parenthesis, "parenthesis paren");

	Ast_node* left = Ast_node_get(paren, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_plus, "plus 2");

	Ast_node* left2 = Ast_node_get(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, Ast_type_number, "number");
	expect_str(&left2->value, "1", "1");

	Ast_node* right = Ast_node_get(left, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_number, "number 2");
	expect_str(&right->value, "2", "2");

	Ast_node* right2 = Ast_node_get(plus, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, Ast_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

    parse_teardown(&cu);
}

void test_parse_paren_mult()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("(5 * 2)", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* paren = Ast_node_get(cu.root, 0);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, Ast_type_parenthesis, "parenthesis paren");

	Ast_node* mult = Ast_node_get(paren, 0);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, Ast_type_mult, "mult mult");

	Ast_node* left = Ast_node_get(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_number, "number");
	expect_str(&left->value, "5", "5");

	Ast_node* right = Ast_node_get(mult, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_number, "number");
	expect_str(&right->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_paren_mult_mult()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("1 * (2 * 3)", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* mult = Ast_node_get(cu.root, 0);
	assert_ptr(mult, "ptr mult");
	assert_int_equal(mult->type, Ast_type_mult, "mult mult");

	Ast_node* left = Ast_node_get(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_number, "number");
	expect_str(&left->value, "1", "1");

	Ast_node* paren = Ast_node_get(mult, 1);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, Ast_type_parenthesis, "parenthesis paren");

	Ast_node* right = Ast_node_get(paren, 0);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_mult, "mult 2");

	Ast_node* left2 = Ast_node_get(right, 0);
	assert_ptr(left2, "left 2");
	expect_int_equal(left2->type, Ast_type_number, "number 2");
	expect_str(&left2->value, "2", "2");

	Ast_node* right2 = Ast_node_get(right, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, Ast_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

    parse_teardown(&cu);
}

void test_parse_paren_mult_mult2()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("(1 * 2) * 3", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* mult = Ast_node_get(cu.root, 0);
	assert_ptr(mult, "ptr mult");
	assert_int_equal(mult->type, Ast_type_mult, "mult mult");

	Ast_node* paren = Ast_node_get(mult, 0);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, Ast_type_parenthesis, "parenthesis paren");

	Ast_node* left = Ast_node_get(paren, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_mult, "mult 2");

	Ast_node* left2 = Ast_node_get(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, Ast_type_number, "number");
	expect_str(&left2->value, "1", "1");

	Ast_node* right = Ast_node_get(left, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_number, "number 2");
	expect_str(&right->value, "2", "2");

	Ast_node* right2 = Ast_node_get(mult, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, Ast_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

    parse_teardown(&cu);
}

void test_parse_comparison()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let count: i64 = 5; count == 10; count != 11.1; count <= 12; count >= 13", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	Ast_node* cond0 = Ast_node_get(cu.root, 1);
	assert_ptr(cond0, "ptr cond0");
	expect_int_equal(cond0->type, Ast_type_equality, "equality cond0");

	Type_use* cond0_tu = cond0->tu;
	assert_ptr(cond0_tu, "ptr cond0_tu");
	
	struct type_def* cond0_td = cond0_tu->td;
	assert_ptr(cond0_td, "ptr cond0_td");
	expect_int_equal(cond0_td->type, type_integer, "integer cond0_td");
	expect_str(&cond0_td->name, "i64", "i64 cond0_td");

	Ast_node* left0 = Ast_node_get(cond0, 0);
	assert_ptr(left0, "ptr left0");
	expect_int_equal(left0->type, Ast_type_id, "id left0");
	expect_str(&left0->value, "count", "count left0");

	Ast_node* right0 = Ast_node_get(cond0, 1);
	assert_ptr(right0, "ptr right0");
	expect_int_equal(right0->type, Ast_type_number, "number right0");
	expect_str(&right0->value, "10", "10 right0");

	Ast_node* cond1 = Ast_node_get(cu.root, 2);
	assert_ptr(cond1, "ptr cond1");
	expect_int_equal(cond1->type, Ast_type_not_equal, "not equal cond1");

	Type_use* cond1_tu = cond1->tu;
	assert_ptr(cond1_tu, "ptr cond1_tu");

	struct type_def* cond1_td = cond1_tu->td;
	assert_ptr(cond1_td, "ptr cond0_td");
	expect_int_equal(cond1_td->type, type_float, "float cond1_td");
	expect_str(&cond1_td->name, "f64", "f64 cond1_td");

	Ast_node* left1 = Ast_node_get(cond1, 0);
	assert_ptr(left1, "ptr left1");
	expect_int_equal(left1->type, Ast_type_id, "id left1");
	expect_str(&left1->value, "count", "count left1");

	Ast_node* right1 = Ast_node_get(cond1, 1);
	assert_ptr(right1, "ptr right1");
	expect_int_equal(right1->type, Ast_type_number, "number right1");
	expect_str(&right1->value, "11.1", "11.1 right1");

	Ast_node* cond2 = Ast_node_get(cu.root, 3);
	assert_ptr(cond2, "ptr cond2");
	expect_int_equal(cond2->type, Ast_type_less_than_or_equal, "less than or equal cond2");

	Ast_node* left2 = Ast_node_get(cond2, 0);
	assert_ptr(left2, "ptr left2");
	expect_int_equal(left2->type, Ast_type_id, "id left2");
	expect_str(&left2->value, "count", "count left2");

	Ast_node* right2 = Ast_node_get(cond2, 1);
	assert_ptr(right2, "ptr right2");
	expect_int_equal(right2->type, Ast_type_number, "number right2");
	expect_str(&right2->value, "12", "12 right2");

	Ast_node* cond3 = Ast_node_get(cu.root, 4);
	assert_ptr(cond3, "ptr cond3");
	expect_int_equal(cond3->type, Ast_type_greater_than_or_equal, "greater than or equal cond3");

	Ast_node* left3 = Ast_node_get(cond3, 0);
	assert_ptr(left3, "ptr left3");
	expect_int_equal(left3->type, Ast_type_id, "id left3");
	expect_str(&left3->value, "count", "count left3");

	Ast_node* right3 = Ast_node_get(cond3, 1);
	assert_ptr(right3, "ptr right3");
	expect_int_equal(right3->type, Ast_type_number, "number right3");
	expect_str(&right3->value, "13", "13 right3");

    parse_teardown(&cu);
}

void test_parse_comparison_identity()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("true == true; true != true", &cu);
	expect_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");

	Ast_node* comp0 = Ast_node_get(cu.root, 0);
	assert_ptr(comp0, "ptr comp0");
	assert_ptr(comp0->tu, "ptr comp0->tu");
	assert_ptr(comp0->tu->td, "ptr comp0->tu->td");
	expect_int_equal(comp0->tu->td->type, type_boolean, "boolean comp0->tu->td->type");
	expect_str(&comp0->tu->td->name, "bool", "bool comp0->tu->td->name");

	Ast_node* comp1 = Ast_node_get(cu.root, 1);
	assert_ptr(comp1, "ptr comp1");
	assert_ptr(comp1->tu, "ptr comp->tu1");
	assert_ptr(comp1->tu->td, "ptr comp1->tu->td");
	expect_int_equal(comp1->tu->td->type, type_boolean, "boolean comp1->tu->td->type");
	expect_str(&comp1->tu->td->name, "bool", "bool comp1->tu->td->name");

    parse_teardown(&cu);
}

void test_parse_comparison_error_no_term()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("100 <", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expected term after comparison operator");

    parse_teardown(&cu);
}

void test_parse_comparison_error_left_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; foo() < 100", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "operand has no value");

    parse_teardown(&cu);
}

void test_parse_comparison_error_right_no_value()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("fn foo() end; 100 < foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "operand has no value");

    parse_teardown(&cu);
}

void test_parse_comparison_error_left_not_numeric()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("true < 100", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "comparison operand is not numeric");

    parse_teardown(&cu);
}

void test_parse_comparison_error_right_not_numeric()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("true < 100", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "comparison operand is not numeric");

    parse_teardown(&cu);
}

void test_parse_and()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a: bool; let b: bool; a && b", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");

	Ast_node* and = Ast_node_get(cu.root, 2);
	assert_ptr(and, "ptr and");
	expect_int_equal(and->type, Ast_type_and, "and and");

	Ast_node* a = Ast_node_get(and, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ast_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	Ast_node* b = Ast_node_get(and, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, Ast_type_id, "id b");
	expect_str(&b->value, "b", "b b");

    parse_teardown(&cu);
}

void test_parse_or()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a: bool; let b: bool; a || b", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* or = Ast_node_get(cu.root, 2);
	assert_ptr(or, "ptr or");
	expect_int_equal(or->type, Ast_type_or, "or or");

	Type_use* tu = or->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "bool", "bool td");

	Ast_node* a = Ast_node_get(or, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ast_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	Ast_node* b = Ast_node_get(or, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, Ast_type_id, "id b");
	expect_str(&b->value, "b", "b b");

    parse_teardown(&cu);
}

void test_parse_or_or()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("let a: bool; let b: bool; let c: bool; a || b || c", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* or0 = Ast_node_get(cu.root, 3);
	assert_ptr(or0 , "ptr or0");
	expect_int_equal(or0->type, Ast_type_or, "or or0");

	Ast_node* or1 = Ast_node_get(or0, 0);
	assert_ptr(or1, "ptr or1");
	expect_int_equal(or1->type, Ast_type_or, "or or1");

	Ast_node* a = Ast_node_get(or1, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ast_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	Ast_node* b = Ast_node_get(or1, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, Ast_type_id, "id b");
	expect_str(&b->value, "b", "b b");

	Ast_node* c = Ast_node_get(or0, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, Ast_type_id, "id c");
	expect_str(&c->value, "c", "c c");

    parse_teardown(&cu);
}

void test_parse_boolean_error_expected_term()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("true &&", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "expected term after && or ||");

    parse_teardown(&cu);
}

void test_parse_boolean_error_left_no_value()
{
	test_name(__func__);

	
	struct comp_unit cu;


    parse_setup("fn foo() end; foo() && true", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "left-side operand of boolean operator has no type");

    parse_teardown(&cu);
}

void test_parse_boolean_error_right_no_value()
{
	test_name(__func__);

	
	struct comp_unit cu;


    parse_setup("fn foo() end; true && foo()", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "operand of boolean operator has no type");

    parse_teardown(&cu);
}

void test_parse_boolean_error_left_not_boolean()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("1 && true", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "left-side expression of boolean operator is not boolean");

    parse_teardown(&cu);
}

void test_parse_boolean_error_right_not_boolean()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("true && 1", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "cu.valid");
	expect_source_error(&cu.el, "expression of boolean operator is not boolean");

    parse_teardown(&cu);
}

void test_parse_array_declare()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a: [5]i64\n"
                "a[0]\n",
                &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_array_subscript()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a: [10]i64; a[1]", &cu);
	expect_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

    assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* as = Ast_node_get(cu.root, 1);
	assert_ptr(as, "ptr as");
	expect_int_equal(as->type, Ast_type_array_subscript, "array-subscript as");

	Type_use* tu = as->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

	Ast_node* name = Ast_node_get(as, 0);
	assert_ptr(name, "ptr name");
	expect_int_equal(name->type, Ast_type_id, "id name");
	expect_str(&name->value, "a", "a name");

	Ast_node* index = Ast_node_get(as, 1);
	assert_ptr(index, "ptr index");
	expect_int_equal(index->type, Ast_type_number, "number index");
	expect_str(&index->value, "1", "1 index");

    parse_teardown(&cu);
}

void test_parse_array_subscript2()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("let a: [10][10]i64; a[0][1]", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

    Ast_node* a = Ast_node_get(cu.root, 1);
	assert_ptr(a, "ptr a");
	assert_int_equal(a->type, Ast_type_array_subscript, "array-subscript a");

	Type_use* a_tu = a->tu;
	assert_ptr(a_tu, "ptr tu");
    expect_false(a_tu->is_array, "is array to");

	struct type_def* td = a_tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

	Ast_node* b = Ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, Ast_type_array_subscript, "array_subscript b");

    Ast_node* c = Ast_node_get(b, 0);
    assert_ptr(c, "ptr c");
    expect_int_equal(c->type, Ast_type_id, "id c");
    expect_str(&c->value, "a", "value a");

    Ast_node* d = Ast_node_get(b, 1);
    assert_ptr(d, "ptr d");
    expect_int_equal(d->type, Ast_type_number, "type d");
    expect_str(&d->value, "0", "value d");

	Ast_node* e = Ast_node_get(a, 1);
	assert_ptr(e, "ptr e");
	expect_int_equal(d->type, Ast_type_number, "number e");
    expect_str(&e->value, "1", "value e");

    parse_teardown(&cu);
}

void test_parse_array_subscript3()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a: [5]i64; let b: i64; a[b]", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	Ast_node* a = Ast_node_get(cu.root, 2);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ast_type_array_subscript, "type a");

	Type_use* tu = a->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

	Ast_node* b = Ast_node_get(a, 0);
	expect_int_equal(b->type, Ast_type_id, "type b");
	expect_str(&b->value, "a", "a b");

	Ast_node* c = Ast_node_get(a, 1);
	expect_int_equal(c->type, Ast_type_id, "type c");
	expect_str(&c->value, "b", "value c");

    parse_teardown(&cu);
}

void test_parse_expr_array_subscript_3d()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let mut x: [2][3][4 const]i64 = \n"
                "[\n"
                "  [\n"
                "    [1, 2, 3, 4],\n"
                "    [5, 6, 7, 8],\n"
                "    [9, 10, 11, 12]\n"
                "  ],\n"
                "  [\n"
                "    [13, 14, 15, 16],\n"
                "    [17, 18, 19, 20],\n"
                "    [21, 22, 23, 24]\n"
                "  ]\n"
                "]\n"
                "x[1][1][3]\n",
                &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    Ast_node* let = Ast_node_get(cu.root, 0);
    assert_ptr(let, "per let");
    expect_int_equal(let->type, Ast_type_let, "type let");

    Ast_node* let_type = Ast_node_get(let, 1);
    assert_ptr(let_type, "ptr let_type");
    expect_int_equal(let_type->type, Ast_type_type, "type let_type");

    Ast_node* a = Ast_node_get(cu.root, 1);
    assert_ptr(a, "ptr a");
    assert_int_equal(a->type, Ast_type_array_subscript, "type a");

    Type_use* a_tu = a->tu;
    assert_ptr(a_tu, "ptr a_tu");
    expect_false(a_tu->is_mut, "is_mut a_tu");
    expect_false(a_tu->is_array, "is_array a_tu");
    expect_size_t_equal(a_tu->dim.count, 0, "dim.count a_tu");

    struct type_def* a_td = a_tu->td;
    assert_ptr(a_td, "ptr a_td");
    expect_int_equal(a_td->type, type_integer, "type a_td");
    expect_str(&a_td->name, "i64", "name a_td");

    Ast_node* b = Ast_node_get(a, 0);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_array_subscript, "type b");

    Type_use* b_tu = b->tu;
    assert_ptr(b_tu, "ptr b_tu");
    expect_true(b_tu->is_array, "is_array b_tu");
    expect_size_t_equal(b_tu->dim.count, 1, "dim.count b_tu");

    Type_dimension* b_dim0 = (Type_dimension*)VECTOR_PTR(&b_tu->dim, 0);
    assert_ptr(b_dim0, "ptr b_dim0");
    expect_size_t_equal(b_dim0->size, 4, "size b_dim0");
    expect_size_t_equal(b_dim0->option, Array_element_const, "option b_dim0");

    Ast_node* c = Ast_node_get(b, 0);
    assert_ptr(c, "ptr c");
    expect_int_equal(c->type, Ast_type_array_subscript, "type c");

    Type_use* c_tu = c->tu;
    assert_ptr(c_tu, "ptr c_tu");
    expect_true(c_tu->is_mut, "is_mut c_tu");
    expect_true(c_tu->is_array, "is_array c_tu");
    expect_size_t_equal(c_tu->dim.count, 2, "dim.count c_tu");

    Type_dimension* c_dim0 = (Type_dimension*)VECTOR_PTR(&c_tu->dim, 0);
    assert_ptr(c_dim0, "ptr c_dim0");
    expect_size_t_equal(c_dim0->size, 3, "size c_dim0");
    expect_int_equal(c_dim0->option, Array_element_default, "option c_dim0");

    Type_dimension* c_dim1 = (Type_dimension*)VECTOR_PTR(&c_tu->dim, 1);
    assert_ptr(c_dim1, "ptr c_dim1");
    expect_size_t_equal(c_dim1->size, 4, "size c_dim1");
    expect_int_equal(c_dim1->option, Array_element_const, "option c_dim1");

    Ast_node* d = Ast_node_get(c, 0);
    assert_ptr(d, "ptr d");
    expect_int_equal(d->type, Ast_type_id, "type d");

    Type_use* d_tu = d->tu;
    assert_ptr(d_tu, "ptr d_tu");
    expect_true(d_tu->is_mut, "is_mut d_tu");
    expect_true(d_tu->is_array, "is_array d_tu");
    expect_size_t_equal(d_tu->dim.count, 3, "dim.count d_tu");

    Type_dimension* d_dim0 = (Type_dimension*)VECTOR_PTR(&d_tu->dim, 0);
    assert_ptr(d_dim0, "ptr d_dim0");
    expect_size_t_equal(d_dim0->size, 2, "size d_dim0");
    expect_int_equal(d_dim0->option, Array_element_default, "option d_dim0");

    Type_dimension* d_dim1 = (Type_dimension*)VECTOR_PTR(&d_tu->dim, 1);
    assert_ptr(d_dim1, "ptr d_dim1");
    expect_size_t_equal(d_dim1->size, 3, "size d_dim1");
    expect_int_equal(d_dim1->option, Array_element_default, "option d_dim1");

    Type_dimension* d_dim2 = (Type_dimension*)VECTOR_PTR(&d_tu->dim, 2);
    assert_ptr(d_dim2, "ptr d_dim2");
    expect_size_t_equal(d_dim2->size, 4, "size d_dim2");
    expect_int_equal(d_dim2->option, Array_element_const, "option d_dim2");

    Ast_node* e = Ast_node_get(c, 1);
    assert_ptr(e, "ptr e");
    expect_int_equal(e->type, Ast_type_number, "type e");

    parse_teardown(&cu);
}

void test_parse_subscript_error_no_type()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; foo()[1]", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expression has subscript but has no value");

    parse_teardown(&cu);
}

void test_parse_subscript_error_not_array()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a: i64; a[1]", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expression has subscript but is not an array or slice");

    parse_teardown(&cu);
}

void test_parse_subscript_error_expected_right_square_bracket()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a: [10]i64; a[1", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expected right-square-bracket");

    parse_teardown(&cu);
}

void test_parse_assign_string()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let mut a: [6 const]u8; a = \"hello\"", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts root");

	Ast_node* assign = Ast_node_get(cu.root, 1);
	assert_ptr(assign, "ptr assign");
	expect_int_equal(assign->type, Ast_type_assign, "assign assign");

	Type_use* tu = assign->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "type td");
	expect_str(&td->name, "u8", "name td");
		
	Ast_node* lhv = Ast_node_get(assign, 0);
	assert_ptr(lhv, "ptr lhv");
	expect_int_equal(lhv->type, Ast_type_id, "id lhv");
	expect_str(&lhv->value, "a", "a lhv");

	Ast_node* rhv = Ast_node_get(assign, 1);
	assert_ptr(rhv, "ptr rhv");
	expect_int_equal(rhv->type, Ast_type_string, "string rhv");
	expect_str(&rhv->value, "hello", "hello rhv");

    parse_teardown(&cu);
}

void test_parse_assign_multiple()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let mut a: i64; let mut b: i64; let mut c: i64; a = b = c = 0", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* assign = Ast_node_get(cu.root, 3);
	assert_ptr(assign, "ptr assign");
	expect_int_equal(assign->type, Ast_type_assign, "assign assign");

	Type_use* assign_tu = assign->tu;
	assert_ptr(assign_tu, "ptr assign_tu");

	struct type_def* assign_td = assign_tu->td;
	assert_ptr(assign_td, "ptr assign_td");
	expect_int_equal(assign_td->type, type_integer, "integer assign_td");
	expect_str(&assign_td->name, "i64", "i64 assign_td");

	Ast_node* assign0 = Ast_node_get(assign, 0);
	assert_ptr(assign0, "ptr lhv0");
	expect_int_equal(assign0->type, Ast_type_id, "id assign0");
	expect_str(&assign0->value, "a", "assign0 value");

	Ast_node* assign1 = Ast_node_get(assign, 1);
	assert_ptr(assign1, "ptr assign1");
	expect_int_equal(assign1->type, Ast_type_id, "assign assign1");
    expect_str(&assign1->value, "b", "assign1 value");

    Ast_node* assign2 = Ast_node_get(assign, 2);
    assert_ptr(assign2, "ptr assign2");
    expect_int_equal(assign2->type, Ast_type_id, "assign assign2");
    expect_str(&assign2->value, "c", "assign2 value");

    Ast_node* assign3 = Ast_node_get(assign, 3);
    assert_ptr(assign3, "ptr assign3");
    expect_int_equal(assign3->type, Ast_type_number, "assign assign3");
    expect_str(&assign3->value, "0", "assign3 value");

    parse_teardown(&cu);
}

void test_parse_expr_assignment_eseq_error_eseq_count()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let mut a: i64; let mut b: i64; let mut c: i64; a, b, c = 1, 2", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "assignment sequence counts do not match");

    parse_teardown(&cu);
}

void test_parse_let_assign_error_term()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("let a: bool =", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expected expression");

    parse_teardown(&cu);
}

void test_parse_assign_error_no_value_right()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("fn foo() end; let a: [10]u8 = foo()", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "cannot assign with operand that has no value");

    parse_teardown(&cu);
}

void test_parse_assign_error_not_compatible()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let x: i64 = true", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "values in assignment are not compatible");

    parse_teardown(&cu);
}

void test_parse_assign_error_lvalue()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("true = true", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "invalid lvalue");

    parse_teardown(&cu);
}

void test_parse_expr_newline_assignment()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let mut a: i64; a =\n1", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    Ast_node* op = Ast_node_get(cu.root, 1);
    assert_ptr(op, "ptr op");
    expect_int_equal(op->type, Ast_type_assign, "assign op");

    Ast_node* one = Ast_node_get(op, 0);
    assert_ptr(one, "one");
    expect_int_equal(one->type, Ast_type_id, "id one");
    expect_str(&one->value, "a", "a one");

    Ast_node* two = Ast_node_get(op, 1);
    assert_ptr(two, "two");
    expect_int_equal(two->type, Ast_type_number, "number two");
    expect_str(&two->value, "1", "1 two");

    parse_teardown(&cu);
}

void test_parse_expr_newline_boolean()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("true &&\nfalse", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    Ast_node* op = Ast_node_get(cu.root, 0);
    assert_ptr(op, "ptr add");
    expect_int_equal(op->type, Ast_type_and, "less than op");

    Ast_node* one = Ast_node_get(op, 0);
    assert_ptr(one, "one");
    expect_int_equal(one->type, Ast_type_boolean, "boolean one");
    expect_str(&one->value, "true", "true");

    Ast_node* two = Ast_node_get(op, 1);
    assert_ptr(two, "two");
    expect_int_equal(two->type, Ast_type_boolean, "boolean two");
    expect_str(&two->value, "false", "false");

    parse_teardown(&cu);
}

void test_parse_expr_newline_comparison()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("1 <\n2", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    Ast_node* op = Ast_node_get(cu.root, 0);
    assert_ptr(op, "ptr add");
    expect_int_equal(op->type, Ast_type_less_than, "less than op");

    Ast_node* one = Ast_node_get(op, 0);
    assert_ptr(one, "one");
    expect_int_equal(one->type, Ast_type_number, "number one");
    expect_str(&one->value, "1", "1");

    Ast_node* two = Ast_node_get(op, 1);
    assert_ptr(two, "two");
    expect_int_equal(two->type, Ast_type_number, "number two");
    expect_str(&two->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_expr_newline_add()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("1 +\n2", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    Ast_node* add = Ast_node_get(cu.root, 0);
    assert_ptr(add, "ptr add");
    expect_int_equal(add->type, Ast_type_plus, "plus add");

    Ast_node* one = Ast_node_get(add, 0);
    assert_ptr(one, "one");
    expect_int_equal(one->type, Ast_type_number, "number one");
    expect_str(&one->value, "1", "1");

    Ast_node* two = Ast_node_get(add, 1);
    assert_ptr(two, "two");
    expect_int_equal(two->type, Ast_type_number, "number two");
    expect_str(&two->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_expr_newline_mult()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("1 *\n2", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    Ast_node* op = Ast_node_get(cu.root, 0);
    assert_ptr(op, "ptr op");
    expect_int_equal(op->type, Ast_type_mult, "plus op");

    Ast_node* one = Ast_node_get(op, 0);
    assert_ptr(one, "one");
    expect_int_equal(one->type, Ast_type_number, "number one");
    expect_str(&one->value, "1", "1");

    Ast_node* two = Ast_node_get(op, 1);
    assert_ptr(two, "two");
    expect_int_equal(two->type, Ast_type_number, "number two");
    expect_str(&two->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_expr_newline_power()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("1^\n2", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    Ast_node* op = Ast_node_get(cu.root, 0);
    assert_ptr(op, "ptr op");
    expect_int_equal(op->type, Ast_type_power, "plus op");

    Ast_node* one = Ast_node_get(op, 0);
    assert_ptr(one, "one");
    expect_int_equal(one->type, Ast_type_number, "number one");
    expect_str(&one->value, "1", "1");

    Ast_node* two = Ast_node_get(op, 1);
    assert_ptr(two, "two");
    expect_int_equal(two->type, Ast_type_number, "number two");
    expect_str(&two->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_expr_newline_subscript()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a: [1]i64; a[\n0\n]", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    Ast_node* subscript = Ast_node_get(cu.root, 1);
    assert_ptr(subscript, "ptr subscript");
    expect_int_equal(subscript->type, Ast_type_array_subscript, "array_subscript subscript");

    Ast_node* array = Ast_node_get(subscript, 0);
    assert_ptr(array, "array");
    expect_int_equal(array->type, Ast_type_id, "id array");
    expect_str(&array->value, "a", "a");

    Ast_node* zero = Ast_node_get(subscript, 1);
    assert_ptr(zero, "zero");
    expect_int_equal(zero->type, Ast_type_number, "number zero");
    expect_str(&zero->value, "0", "0");

    parse_teardown(&cu);
}

void test_parse_expr_newline_function_call()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(a: i64, b: i64)->i64 a+b end; foo(\n1,\n2\n)", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    Ast_node* call = Ast_node_get(cu.root, 1);
    assert_ptr(call, "ptr call");
    expect_int_equal(call->type, Ast_type_call, "call call");

    Ast_node* foo = Ast_node_get(call, 0);
    assert_ptr(foo, "ptr foo");
    expect_int_equal(foo->type, Ast_type_id, "id foo");
    expect_str(&foo->value, "foo", "foo");

    Ast_node* cseq = Ast_node_get(call, 1);
    assert_ptr(cseq, "ptr cseq");
    expect_int_equal(cseq->type, Ast_type_cseq, "cseq cseq");

    Ast_node* one = Ast_node_get(cseq, 0);
    assert_ptr(one, "ptr one");
    expect_int_equal(one->type, Ast_type_number, "number one");
    expect_str(&one->value, "1", "1");

    Ast_node* two = Ast_node_get(cseq, 1);
    assert_ptr(two, "ptr two");
    expect_int_equal(two->type, Ast_type_number, "number two");
    expect_str(&two->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_expr_assign_eseq()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let mut a: i64\n"
                "let mut b: i64\n"
                "let mut c: i64\n"
                "a,b,c = 1,2,3", &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* assign = Ast_node_get(cu.root, 3);
    assert_ptr(assign, "ptr assign");
    expect_int_equal(assign->type, Ast_type_assign, "assign assign");

    Ast_node* lvalue = Ast_node_get(assign, 0);
    assert_ptr(lvalue, "ptr lvalue");
    expect_int_equal(lvalue->type, Ast_type_eseq, "eseq lvalue");

    Ast_node* a = Ast_node_get(lvalue, 0);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_id, "id a");
    expect_str(&a->value, "a", "a");

    Ast_node* b = Ast_node_get(lvalue, 1);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_id, "id b");
    expect_str(&b->value, "b", "b");

    Ast_node* c = Ast_node_get(lvalue, 2);
    assert_ptr(c, "ptr c");
    expect_int_equal(c->type, Ast_type_id, "id c");
    expect_str(&c->value, "c", "c");

    parse_teardown(&cu);
}

void test_parse_expr_error_lvalue()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a: i64\n"
                "1 = 1", &cu);
    expect_has_errors(&cu.el);
    expect_source_error(&cu.el, "invalid lvalue");
    expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_expr_error_eseq_lvalue()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a: i64\n"
                "let b: i64\n"
                "let c: i64\n"
                "1,2,3 = 1,2,3", &cu);
    expect_has_errors(&cu.el);
    expect_source_error(&cu.el, "invalid lvalue");
    expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_array_string_return()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(a: i64, b: i64, c: i64)->[6 const]u8\n"
                "  \"hello\"\n"
                "end\n"
                "foo(1, 2, 3)\n",
                &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_expression()
{
	test_parse_blank();
	test_parse_add();
	test_parse_add_error_expected_term();
	test_parse_add_error_left_no_value();
	test_parse_add_error_left_not_numeric();
	test_parse_add_error_right_no_value();
    test_parse_add_error_right_not_numeric();
	test_parse_add_mixed_types();
	test_parse_add_positive();
	test_parse_add_negative();
	test_parse_sub();
	test_parse_sub_positive();
	test_parse_sub_negative();
	test_parse_mult();
	test_parse_mult_error_expected_term();
	test_parse_mult_error_left_no_value();
	test_parse_mult_error_left_not_numeric();
	test_parse_mult_error_right_no_value();
	test_parse_mult_error_right_not_numeric();
	test_parse_mult_positive();
	test_parse_mult_negative();
	test_parse_divide();
	test_parse_add_add();
	test_parse_mult_mult();
	test_parse_add_mult();
	test_parse_mult_add();
	test_parse_power();
	test_parse_power_error_expected_term();
	test_parse_power_error_left_no_value();
	test_parse_power_error_left_not_numeric();
	test_parse_power_error_right_no_value();
	test_parse_power_error_right_not_numeric();
	test_parse_paren_add();
	test_parse_paren_add2();
	test_parse_paren_add3();
	test_parse_paren_add_add();
	test_parse_paren_add_add2();
	test_parse_paren_mult();
	test_parse_paren_mult_mult();
	test_parse_paren_mult_mult2();
	test_parse_comparison();
	test_parse_comparison_identity();
	test_parse_comparison_error_no_term();
	test_parse_comparison_error_left_no_value();
	test_parse_comparison_error_right_no_value();
	test_parse_comparison_error_left_not_numeric();
	test_parse_comparison_error_right_not_numeric();
	test_parse_and();
	test_parse_or();
	test_parse_or_or();
	test_parse_boolean_error_expected_term();
	test_parse_boolean_error_left_no_value();
	test_parse_boolean_error_right_no_value();
	test_parse_boolean_error_left_not_boolean();
	test_parse_boolean_error_right_not_boolean();
    test_parse_array_declare();
	test_parse_array_subscript();
	test_parse_array_subscript2();
	test_parse_array_subscript3();
    test_parse_expr_array_subscript_3d();
	test_parse_subscript_error_no_type();
	test_parse_subscript_error_not_array();
	test_parse_subscript_error_expected_right_square_bracket();
	test_parse_assign_string();
	test_parse_assign_multiple();
    test_parse_expr_assignment_eseq_error_eseq_count();
    test_parse_let_assign_error_term();
	test_parse_assign_error_no_value_right();
	test_parse_assign_error_not_compatible();
	test_parse_assign_error_lvalue();
    test_parse_expr_newline_assignment();
    test_parse_expr_newline_boolean();
    test_parse_expr_newline_comparison();
    test_parse_expr_newline_add();
    test_parse_expr_newline_mult();
    test_parse_expr_newline_power();
    test_parse_expr_newline_subscript();
    test_parse_expr_newline_function_call();
    test_parse_expr_assign_eseq();
    test_parse_expr_error_lvalue();
    test_parse_expr_error_eseq_lvalue();
    test_parse_array_string_return();
}