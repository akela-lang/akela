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
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* node = Ast_node_get(cu.root, 0);
	Zinc_assert_null(node, "null node");

    parse_teardown(&cu);
}

void test_parse_add()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("let speed: i64 = 100; speed + 1", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* add = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(add, "ptr add");
	Zinc_expect_int_equal(add->type, Ake_ast_type_plus, "plus add");

	Ake_type_use* tu = add->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_integer, "integer td");
	Zinc_expect_str(&td->name, "i64", "i64 td");

	Ake_ast* left = Ast_node_get(add, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_id, "id");
	Zinc_expect_str(&left->value, "speed", "speed");

	Ake_ast* right = Ast_node_get(add, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&right->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_add_error_expected_term()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("1 +", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "expected term after additive operator");

    parse_teardown(&cu);
}

void test_parse_add_error_left_no_value()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; foo() + 1", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "addition operand has no value");

    parse_teardown(&cu);
}

void test_parse_add_error_left_not_numeric()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("true + 1", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "addition on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_add_error_right_no_value()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; 1 + foo()", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "addition operand has no value");

    parse_teardown(&cu);
}

void test_parse_add_error_right_not_numeric()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("1 + true", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "addition on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_add_mixed_types()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("1 + 5.0", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* add = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(add, "ptr add");
	Zinc_expect_int_equal(add->type, Ake_ast_type_plus, "plus add");

	Ake_type_use* add_tu = add->tu;
	Zinc_assert_ptr(add_tu, "ptr add_tu");

	struct Ake_type_def* add_td = add_tu->td;
	Zinc_assert_ptr(add_td, "ptr add_td");
	Zinc_expect_int_equal(add_td->type, Ake_type_float, "float add_td");
	Zinc_expect_str(&add_td->name, "f64", "f64 add_td");

	Ake_ast* left = Ast_node_get(add, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_number, "number left");
	Zinc_expect_str(&left->value, "1", "1 left");

	Ake_ast* right = Ast_node_get(add, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&right->value, "5.0", "5.0 right");

    parse_teardown(&cu);
}

void test_parse_add_positive()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let speed: i64 = 100; speed + +1", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* add = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(add, "ptr add");
	Zinc_expect_int_equal(add->type, Ake_ast_type_plus, "plus add");

	Ake_ast* left = Ast_node_get(add, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_id, "id");
	Zinc_expect_str(&left->value, "speed", "speed");

	Ake_ast* right = Ast_node_get(add, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_sign, "sign");

	Zinc_assert_null(Ast_node_get(cu.root, 2), "only 2 children");

	Ake_ast* left2 = Ast_node_get(right, 0);
	Zinc_assert_ptr(left2, "left2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_plus, "plus2");

	Ake_ast* right2 = Ast_node_get(right, 1);
	Zinc_assert_ptr(right2, "right2");
	Zinc_expect_str(&right2->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_add_negative()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let speed: i64; speed + -1", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* add = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(add, "ptr add");
	Zinc_expect_int_equal(add->type, Ake_ast_type_plus, "plus add");

	Ake_ast* left = Ast_node_get(add, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_id, "id");
	Zinc_expect_str(&left->value, "speed", "speed");

	Ake_ast* right = Ast_node_get(add, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_sign, "sign");

	Zinc_assert_null(Ast_node_get(add, 2), "only 2 children");

	Ake_ast* left2 = Ast_node_get(right, 0);
	Zinc_assert_ptr(left2, "left2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_minus, "minus");

	Ake_ast* right2 = Ast_node_get(right, 1);
	Zinc_assert_ptr(right2, "right2");
	Zinc_expect_str(&right2->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_sub()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let delta: i64 = 3; 100 - delta", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* sub = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(sub, "ptr sub");
	Zinc_expect_int_equal(sub->type, Ake_ast_type_minus, "minus sub");

	Ake_ast* left = Ast_node_get(sub, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&left->value, "100", "100");

	Ake_ast* right = Ast_node_get(sub, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_id, "id");
	Zinc_expect_str(&right->value, "delta", "delta");

	Zinc_assert_null(Ast_node_get(cu.root, 2), "only 2 children");

    parse_teardown(&cu);
}

void test_parse_sub_positive()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let speed: i64; speed - +1", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* sub = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(sub, "cu.root");
	Zinc_expect_int_equal(sub->type, Ake_ast_type_minus, "minus sub");

	Ake_ast* left = Ast_node_get(sub, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_id, "id");
	Zinc_expect_str(&left->value, "speed", "speed");

	Ake_ast* right = Ast_node_get(sub, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_sign, "sign");

	Zinc_assert_null(Ast_node_get(sub, 2), "only 2 children");

	Ake_ast* left2 = Ast_node_get(right, 0);
	Zinc_assert_ptr(left2, "left2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_plus, "plus");

	Ake_ast* right2 = Ast_node_get(right, 1);
	Zinc_assert_ptr(right2, "right2");
	Zinc_expect_str(&right2->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_sub_negative()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("let speed: i64; speed - -1", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* sub = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(sub, "ptr sub");
	Zinc_expect_int_equal(sub->type, Ake_ast_type_minus, "minus sub");

	Ake_ast* left = Ast_node_get(sub, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_id, "id");
	Zinc_expect_str(&left->value, "speed", "speed");

	Ake_ast* right = Ast_node_get(sub, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_sign, "sign");

	Zinc_assert_null(Ast_node_get(sub, 2), "only 2 children");

	Ake_ast* left2 = Ast_node_get(right, 0);
	Zinc_assert_ptr(left2, "left2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_minus, "minus2");

	Ake_ast* right2 = Ast_node_get(right, 1);
	Zinc_assert_ptr(right2, "right2");
	Zinc_expect_str(&right2->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_mult()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("5 * 2", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* mult = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(mult, "cu.root");
	Zinc_expect_int_equal(mult->type, Ake_ast_type_mult, "mult mult");

	Ake_type_use* tu = mult->tu;
	Zinc_assert_ptr(tu, "ptr tu");
	
	struct Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_integer, "integer td");
	Zinc_expect_str(&td->name, "i64", "i64 td");

	Ake_ast* left = Ast_node_get(mult, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&left->value, "5", "5");

	Ake_ast* right = Ast_node_get(mult, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&right->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_mult_error_expected_term()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("5*", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "expected term after operator");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_mult_error_left_no_value()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; foo() * 1", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "multiplication operand has no value");

    parse_teardown(&cu);
}

void test_parse_mult_error_left_not_numeric()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("true * 1", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "multiplication on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_mult_error_right_no_value()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; 1 * foo()", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "multiplication operand has no value");

    parse_teardown(&cu);
}

void test_parse_mult_error_right_not_numeric()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("1 * true", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "multiplication on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_mult_positive()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let speed: i64; speed * +1", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* mult = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(mult, "ptr mult");
	Zinc_expect_int_equal(mult->type, Ake_ast_type_mult, "mult mult");

	Ake_ast* left = Ast_node_get(mult, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_id, "id");
	Zinc_expect_str(&left->value, "speed", "speed");

	Ake_ast* right = Ast_node_get(mult, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_sign, "sign");

	Zinc_assert_null(Ast_node_get(mult, 2), "only 2 children");

	Ake_ast* left2 = Ast_node_get(right, 0);
	Zinc_assert_ptr(left2, "left2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_plus, "plus");

	Ake_ast* right2 = Ast_node_get(right, 1);
	Zinc_assert_ptr(right2, "right2");
	Zinc_expect_str(&right2->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_mult_negative()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let speed: i64; speed * -1", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* mult = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(mult, "ptr mult");
	Zinc_expect_int_equal(mult->type, Ake_ast_type_mult, "mult mult");

	Ake_ast* left = Ast_node_get(mult, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_id, "id");
	Zinc_expect_str(&left->value, "speed", "speed");

	Ake_ast* right = Ast_node_get(mult, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_sign, "sign");

	Zinc_assert_null(Ast_node_get(mult, 2), "only 2 children");

	Ake_ast* left2 = Ast_node_get(right, 0);
	Zinc_assert_ptr(left2, "left2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_minus, "minus2");

	Ake_ast* right2 = Ast_node_get(right, 1);
	Zinc_assert_ptr(right2, "right2");
	Zinc_expect_str(&right2->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_divide()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("52 / 2", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* divide = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(divide, "ptr divide");
	Zinc_assert_int_equal(divide->type, Ake_ast_type_divide, "divide divide");

	Ake_ast* left = Ast_node_get(divide, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&left->value, "52", "52");

	Ake_ast* right = Ast_node_get(divide, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&right->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_add_add()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("1 + 2 + 3", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* plus0 = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(plus0, "ptr plus0");
	Zinc_assert_int_equal(plus0->type, Ake_ast_type_plus, "plus plus0");

	Ake_ast* left = Ast_node_get(plus0, 0);
	Zinc_assert_ptr(left, "ptr left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_plus, "plus left");

	Ake_ast* left2 = Ast_node_get(left, 0);
	Zinc_assert_ptr(left2, "ptr left2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_number, "number left2");
	Zinc_expect_str(&left2->value, "1", "1 left2");

	Ake_ast* right = Ast_node_get(left, 1);
	Zinc_assert_ptr(right, "ptr right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number right");
	Zinc_expect_str(&right->value, "2", "2 right");

	Ake_ast* right2 = Ast_node_get(plus0, 1);
	Zinc_assert_ptr(right2, "ptr right2");
	Zinc_expect_int_equal(right2->type, Ake_ast_type_number, "number right2");
	Zinc_expect_str(&right2->value, "3", "3 right2");

    parse_teardown(&cu);
}

void test_parse_mult_mult()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("1 * 2 * 3", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* mult = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(mult, "ptr mult");
	Zinc_assert_int_equal(mult->type, Ake_ast_type_mult, "mult mult");

	Ake_ast* left = Ast_node_get(mult, 0);
	Zinc_assert_ptr(left, "ptr left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_mult, "mult left");

	Ake_ast* left2 = Ast_node_get(left, 0);
	Zinc_assert_ptr(left2, "ptr left2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_number, "number left2");
	Zinc_expect_str(&left2->value, "1", "1 left2");

	Ake_ast* right = Ast_node_get(left, 1);
	Zinc_assert_ptr(right, "ptr right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number right");
	Zinc_expect_str(&right->value, "2", "2 right");

	Ake_ast* right2 = Ast_node_get(mult, 1);
	Zinc_assert_ptr(right2, "ptr right2");
	Zinc_expect_int_equal(right2->type, Ake_ast_type_number, "number right2");
	Zinc_expect_str(&right2->value, "3", "3 right2");

    parse_teardown(&cu);
}

void test_parse_add_mult()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("5 + 3 * 2", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* plus = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(plus, "cu.root");
	Zinc_assert_int_equal(plus->type, Ake_ast_type_plus, "plus");

	Ake_ast* left = Ast_node_get(plus, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&left->value, "5", "5");

	Ake_ast* right = Ast_node_get(plus, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_mult, "mult");

	Ake_ast* left2 = Ast_node_get(right, 0);
	Zinc_assert_ptr(left2, "left2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_number, "number 2");
	Zinc_expect_str(&left2->value, "3", "3");

	Ake_ast* right2 = Ast_node_get(right, 1);
	Zinc_assert_ptr(right2, "right2");
	Zinc_expect_int_equal(right2->type, Ake_ast_type_number, "number 3");
	Zinc_expect_str(&right2->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_mult_add()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("4 * 3 + 2", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* plus = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(plus, "ptr plus");
	Zinc_assert_int_equal(plus->type, Ake_ast_type_plus, "plus plus");

	Ake_ast* left = Ast_node_get(plus, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_mult, "mult");

	Ake_ast* left2 = Ast_node_get(left, 0);
	Zinc_assert_ptr(left2, "left2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_number, "number 4");
	Zinc_expect_str(&left2->value, "4", "4");

	Ake_ast* right2 = Ast_node_get(left, 1);
	Zinc_assert_ptr(right2, "right2");
	Zinc_expect_int_equal(right2->type, Ake_ast_type_number, "number 3");
	Zinc_expect_str(&right2->value, "3", "3");

	Ake_ast* right = Ast_node_get(plus, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number 2");
	Zinc_expect_str(&right->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_power()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("5 ^ 2", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "cu.valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* pow = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(pow, "ptr pow");
	Zinc_expect_int_equal(pow->type, Ake_ast_type_power, "power pow");

	Ake_type_use* tu = pow->tu;
	Zinc_assert_ptr(tu, "ptr tu");
	
	struct Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_integer, "integer td");
	Zinc_expect_str(&td->name, "i64", "i64 td");

	Ake_ast* number0 = Ast_node_get(pow, 0);
	Zinc_assert_ptr(number0, "ptr number0");
	Zinc_expect_int_equal(number0->type, Ake_ast_type_number, "number number0");
	Zinc_expect_str(&number0->value, "5", "5 number0");

	Ake_ast* number1 = Ast_node_get(pow, 1);
	Zinc_assert_ptr(number1, "ptr number1");
	Zinc_expect_int_equal(number1->type, Ake_ast_type_number, "number number1");
	Zinc_expect_str(&number1->value, "2", "2 number1");

    parse_teardown(&cu);
}

void test_parse_power_error_expected_term()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("5^", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "expected term after caret");

    parse_teardown(&cu);
}

void test_parse_power_error_left_no_value()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; 5 ^ foo()", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "power operand has no value");

    parse_teardown(&cu);
}

void test_parse_power_error_left_not_numeric()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("true ^ 2", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "power on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_power_error_right_no_value()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; 5 ^ foo()", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "power operand has no value");

    parse_teardown(&cu);
}

void test_parse_power_error_right_not_numeric()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("5 ^ true", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "power on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_paren_add()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let speed: i64; (speed + 1)", &cu);
	Zinc_expect_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* paren = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(paren, "ptr paren");
	Zinc_expect_int_equal(paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

	Ake_ast* plus = Ast_node_get(paren, 0);
	Zinc_assert_ptr(plus, "cu.root");
	Zinc_expect_int_equal(plus->type, Ake_ast_type_plus, "plus");

	Ake_ast* left = Ast_node_get(plus, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_id, "id");
	Zinc_expect_str(&left->value, "speed", "speed");

	Ake_ast* right = Ast_node_get(plus, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&right->value, "1", "1");

	Zinc_assert_null(Ast_node_get(cu.root, 2), "only 2 children");

    parse_teardown(&cu);
}

void test_parse_paren_add2()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let speed: i64; (speed) + 1", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* plus = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(plus, "cu.root");
	Zinc_expect_int_equal(plus->type, Ake_ast_type_plus, "plus");

	Ake_ast* paren = Ast_node_get(plus, 0);
	Zinc_assert_ptr(paren, "ptr paren");
	Zinc_expect_int_equal(paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

	Ake_ast* left = Ast_node_get(paren, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_id, "id");
	Zinc_expect_str(&left->value, "speed", "speed");

	Ake_ast* right = Ast_node_get(plus, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&right->value, "1", "1");

	Zinc_assert_null(Ast_node_get(cu.root, 2), "only 2 children");

    parse_teardown(&cu);
}

void test_parse_paren_add3()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let speed: i64; speed + (1)", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* plus = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(plus, "cu.root");
	Zinc_expect_int_equal(plus->type, Ake_ast_type_plus, "plus");

	Ake_ast* left = Ast_node_get(plus, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_id, "id");
	Zinc_expect_str(&left->value, "speed", "speed");

	Ake_ast* paren = Ast_node_get(plus, 1);
	Zinc_assert_ptr(paren, "ptr paren");
	Zinc_expect_int_equal(paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

	Ake_ast* right = Ast_node_get(paren, 0);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&right->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_paren_add_add()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("1 + (2 + 3)", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* plus = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(plus, "ptr plus");
	Zinc_assert_int_equal(plus->type, Ake_ast_type_plus, "plus plus");

	Ake_ast* left = Ast_node_get(plus, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&left->value, "1", "1");

	Ake_ast* paren = Ast_node_get(plus, 1);
	Zinc_assert_ptr(paren, "ptr paren");
	Zinc_expect_int_equal(paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

	Ake_ast* right = Ast_node_get(paren, 0);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_plus, "plus 2");

	Zinc_assert_null(Ast_node_get(cu.root, 2), "null");

	Ake_ast* left2 = Ast_node_get(right, 0);
	Zinc_assert_ptr(left2, "right");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_number, "number 2");
	Zinc_expect_str(&left2->value, "2", "2");

	Ake_ast* right2 = Ast_node_get(right, 1);
	Zinc_assert_ptr(right2, "right2");
	Zinc_expect_int_equal(right2->type, Ake_ast_type_number, "number 3");
	Zinc_expect_str(&right2->value, "3", "3");

    parse_teardown(&cu);
}

void test_parse_paren_add_add2()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("(1 + 2) + 3", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* plus = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(plus, "ptr plus");
	Zinc_assert_int_equal(plus->type, Ake_ast_type_plus, "plus plus");

	Ake_ast* paren = Ast_node_get(plus, 0);
	Zinc_assert_ptr(paren, "ptr paren");
	Zinc_expect_int_equal(paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

	Ake_ast* left = Ast_node_get(paren, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_plus, "plus 2");

	Ake_ast* left2 = Ast_node_get(left, 0);
	Zinc_assert_ptr(left2, "left2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&left2->value, "1", "1");

	Ake_ast* right = Ast_node_get(left, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number 2");
	Zinc_expect_str(&right->value, "2", "2");

	Ake_ast* right2 = Ast_node_get(plus, 1);
	Zinc_assert_ptr(right2, "right2");
	Zinc_expect_int_equal(right2->type, Ake_ast_type_number, "number 3");
	Zinc_expect_str(&right2->value, "3", "3");

    parse_teardown(&cu);
}

void test_parse_paren_mult()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("(5 * 2)", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* paren = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(paren, "ptr paren");
	Zinc_expect_int_equal(paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

	Ake_ast* mult = Ast_node_get(paren, 0);
	Zinc_assert_ptr(mult, "ptr mult");
	Zinc_expect_int_equal(mult->type, Ake_ast_type_mult, "mult mult");

	Ake_ast* left = Ast_node_get(mult, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&left->value, "5", "5");

	Ake_ast* right = Ast_node_get(mult, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&right->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_paren_mult_mult()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("1 * (2 * 3)", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* mult = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(mult, "ptr mult");
	Zinc_assert_int_equal(mult->type, Ake_ast_type_mult, "mult mult");

	Ake_ast* left = Ast_node_get(mult, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&left->value, "1", "1");

	Ake_ast* paren = Ast_node_get(mult, 1);
	Zinc_assert_ptr(paren, "ptr paren");
	Zinc_expect_int_equal(paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

	Ake_ast* right = Ast_node_get(paren, 0);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_mult, "mult 2");

	Ake_ast* left2 = Ast_node_get(right, 0);
	Zinc_assert_ptr(left2, "left 2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_number, "number 2");
	Zinc_expect_str(&left2->value, "2", "2");

	Ake_ast* right2 = Ast_node_get(right, 1);
	Zinc_assert_ptr(right2, "right2");
	Zinc_expect_int_equal(right2->type, Ake_ast_type_number, "number 3");
	Zinc_expect_str(&right2->value, "3", "3");

    parse_teardown(&cu);
}

void test_parse_paren_mult_mult2()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("(1 * 2) * 3", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* mult = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(mult, "ptr mult");
	Zinc_assert_int_equal(mult->type, Ake_ast_type_mult, "mult mult");

	Ake_ast* paren = Ast_node_get(mult, 0);
	Zinc_assert_ptr(paren, "ptr paren");
	Zinc_expect_int_equal(paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

	Ake_ast* left = Ast_node_get(paren, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_mult, "mult 2");

	Ake_ast* left2 = Ast_node_get(left, 0);
	Zinc_assert_ptr(left2, "left2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&left2->value, "1", "1");

	Ake_ast* right = Ast_node_get(left, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number 2");
	Zinc_expect_str(&right->value, "2", "2");

	Ake_ast* right2 = Ast_node_get(mult, 1);
	Zinc_assert_ptr(right2, "right2");
	Zinc_expect_int_equal(right2->type, Ake_ast_type_number, "number 3");
	Zinc_expect_str(&right2->value, "3", "3");

    parse_teardown(&cu);
}

void test_parse_comparison()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let count: i64 = 5; count == 10; count != 11.1; count <= 12; count >= 13", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Ake_ast* cond0 = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(cond0, "ptr cond0");
	Zinc_expect_int_equal(cond0->type, Ake_ast_type_equality, "equality cond0");

	Ake_type_use* cond0_tu = cond0->tu;
	Zinc_assert_ptr(cond0_tu, "ptr cond0_tu");
	
	struct Ake_type_def* cond0_td = cond0_tu->td;
	Zinc_assert_ptr(cond0_td, "ptr cond0_td");
	Zinc_expect_int_equal(cond0_td->type, Ake_type_integer, "integer cond0_td");
	Zinc_expect_str(&cond0_td->name, "i64", "i64 cond0_td");

	Ake_ast* left0 = Ast_node_get(cond0, 0);
	Zinc_assert_ptr(left0, "ptr left0");
	Zinc_expect_int_equal(left0->type, Ake_ast_type_id, "id left0");
	Zinc_expect_str(&left0->value, "count", "count left0");

	Ake_ast* right0 = Ast_node_get(cond0, 1);
	Zinc_assert_ptr(right0, "ptr right0");
	Zinc_expect_int_equal(right0->type, Ake_ast_type_number, "number right0");
	Zinc_expect_str(&right0->value, "10", "10 right0");

	Ake_ast* cond1 = Ast_node_get(cu.root, 2);
	Zinc_assert_ptr(cond1, "ptr cond1");
	Zinc_expect_int_equal(cond1->type, Ake_ast_type_not_equal, "not equal cond1");

	Ake_type_use* cond1_tu = cond1->tu;
	Zinc_assert_ptr(cond1_tu, "ptr cond1_tu");

	struct Ake_type_def* cond1_td = cond1_tu->td;
	Zinc_assert_ptr(cond1_td, "ptr cond0_td");
	Zinc_expect_int_equal(cond1_td->type, Ake_type_float, "float cond1_td");
	Zinc_expect_str(&cond1_td->name, "f64", "f64 cond1_td");

	Ake_ast* left1 = Ast_node_get(cond1, 0);
	Zinc_assert_ptr(left1, "ptr left1");
	Zinc_expect_int_equal(left1->type, Ake_ast_type_id, "id left1");
	Zinc_expect_str(&left1->value, "count", "count left1");

	Ake_ast* right1 = Ast_node_get(cond1, 1);
	Zinc_assert_ptr(right1, "ptr right1");
	Zinc_expect_int_equal(right1->type, Ake_ast_type_number, "number right1");
	Zinc_expect_str(&right1->value, "11.1", "11.1 right1");

	Ake_ast* cond2 = Ast_node_get(cu.root, 3);
	Zinc_assert_ptr(cond2, "ptr cond2");
	Zinc_expect_int_equal(cond2->type, Ake_ast_type_less_than_or_equal, "less than or equal cond2");

	Ake_ast* left2 = Ast_node_get(cond2, 0);
	Zinc_assert_ptr(left2, "ptr left2");
	Zinc_expect_int_equal(left2->type, Ake_ast_type_id, "id left2");
	Zinc_expect_str(&left2->value, "count", "count left2");

	Ake_ast* right2 = Ast_node_get(cond2, 1);
	Zinc_assert_ptr(right2, "ptr right2");
	Zinc_expect_int_equal(right2->type, Ake_ast_type_number, "number right2");
	Zinc_expect_str(&right2->value, "12", "12 right2");

	Ake_ast* cond3 = Ast_node_get(cu.root, 4);
	Zinc_assert_ptr(cond3, "ptr cond3");
	Zinc_expect_int_equal(cond3->type, Ake_ast_type_greater_than_or_equal, "greater than or equal cond3");

	Ake_ast* left3 = Ast_node_get(cond3, 0);
	Zinc_assert_ptr(left3, "ptr left3");
	Zinc_expect_int_equal(left3->type, Ake_ast_type_id, "id left3");
	Zinc_expect_str(&left3->value, "count", "count left3");

	Ake_ast* right3 = Ast_node_get(cond3, 1);
	Zinc_assert_ptr(right3, "ptr right3");
	Zinc_expect_int_equal(right3->type, Ake_ast_type_number, "number right3");
	Zinc_expect_str(&right3->value, "13", "13 right3");

    parse_teardown(&cu);
}

void test_parse_comparison_identity()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("true == true; true != true", &cu);
	Zinc_expect_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");

	Ake_ast* comp0 = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(comp0, "ptr comp0");
	Zinc_assert_ptr(comp0->tu, "ptr comp0->tu");
	Zinc_assert_ptr(comp0->tu->td, "ptr comp0->tu->td");
	Zinc_expect_int_equal(comp0->tu->td->type, Ake_type_boolean, "boolean comp0->tu->td->type");
	Zinc_expect_str(&comp0->tu->td->name, "bool", "bool comp0->tu->td->name");

	Ake_ast* comp1 = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(comp1, "ptr comp1");
	Zinc_assert_ptr(comp1->tu, "ptr comp->tu1");
	Zinc_assert_ptr(comp1->tu->td, "ptr comp1->tu->td");
	Zinc_expect_int_equal(comp1->tu->td->type, Ake_type_boolean, "boolean comp1->tu->td->type");
	Zinc_expect_str(&comp1->tu->td->name, "bool", "bool comp1->tu->td->name");

    parse_teardown(&cu);
}

void test_parse_comparison_error_no_term()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("100 <", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "valid");
	Zinc_expect_source_error(&cu.el, "expected term after comparison operator");

    parse_teardown(&cu);
}

void test_parse_comparison_error_left_no_value()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; foo() < 100", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "valid");
	Zinc_expect_source_error(&cu.el, "operand has no value");

    parse_teardown(&cu);
}

void test_parse_comparison_error_right_no_value()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; 100 < foo()", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "valid");
	Zinc_expect_source_error(&cu.el, "operand has no value");

    parse_teardown(&cu);
}

void test_parse_comparison_error_left_not_numeric()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("true < 100", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "valid");
	Zinc_expect_source_error(&cu.el, "comparison operand is not numeric");

    parse_teardown(&cu);
}

void test_parse_comparison_error_right_not_numeric()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("true < 100", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "valid");
	Zinc_expect_source_error(&cu.el, "comparison operand is not numeric");

    parse_teardown(&cu);
}

void test_parse_and()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let a: bool; let b: bool; a && b", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");

	Ake_ast* and = Ast_node_get(cu.root, 2);
	Zinc_assert_ptr(and, "ptr and");
	Zinc_expect_int_equal(and->type, Ake_ast_type_and, "and and");

	Ake_ast* a = Ast_node_get(and, 0);
	Zinc_assert_ptr(a, "ptr a");
	Zinc_expect_int_equal(a->type, Ake_ast_type_id, "id a");
	Zinc_expect_str(&a->value, "a", "a a");

	Ake_ast* b = Ast_node_get(and, 1);
	Zinc_assert_ptr(b, "ptr b");
	Zinc_expect_int_equal(b->type, Ake_ast_type_id, "id b");
	Zinc_expect_str(&b->value, "b", "b b");

    parse_teardown(&cu);
}

void test_parse_or()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let a: bool; let b: bool; a || b", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* or = Ast_node_get(cu.root, 2);
	Zinc_assert_ptr(or, "ptr or");
	Zinc_expect_int_equal(or->type, Ake_ast_type_or, "or or");

	Ake_type_use* tu = or->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_boolean, "boolean td");
	Zinc_expect_str(&td->name, "bool", "bool td");

	Ake_ast* a = Ast_node_get(or, 0);
	Zinc_assert_ptr(a, "ptr a");
	Zinc_expect_int_equal(a->type, Ake_ast_type_id, "id a");
	Zinc_expect_str(&a->value, "a", "a a");

	Ake_ast* b = Ast_node_get(or, 1);
	Zinc_assert_ptr(b, "ptr b");
	Zinc_expect_int_equal(b->type, Ake_ast_type_id, "id b");
	Zinc_expect_str(&b->value, "b", "b b");

    parse_teardown(&cu);
}

void test_parse_or_or()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("let a: bool; let b: bool; let c: bool; a || b || c", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* or0 = Ast_node_get(cu.root, 3);
	Zinc_assert_ptr(or0 , "ptr or0");
	Zinc_expect_int_equal(or0->type, Ake_ast_type_or, "or or0");

	Ake_ast* or1 = Ast_node_get(or0, 0);
	Zinc_assert_ptr(or1, "ptr or1");
	Zinc_expect_int_equal(or1->type, Ake_ast_type_or, "or or1");

	Ake_ast* a = Ast_node_get(or1, 0);
	Zinc_assert_ptr(a, "ptr a");
	Zinc_expect_int_equal(a->type, Ake_ast_type_id, "id a");
	Zinc_expect_str(&a->value, "a", "a a");

	Ake_ast* b = Ast_node_get(or1, 1);
	Zinc_assert_ptr(b, "ptr b");
	Zinc_expect_int_equal(b->type, Ake_ast_type_id, "id b");
	Zinc_expect_str(&b->value, "b", "b b");

	Ake_ast* c = Ast_node_get(or0, 1);
	Zinc_assert_ptr(c, "ptr c");
	Zinc_expect_int_equal(c->type, Ake_ast_type_id, "id c");
	Zinc_expect_str(&c->value, "c", "c c");

    parse_teardown(&cu);
}

void test_parse_boolean_error_expected_term()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("true &&", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "expected term after && or ||");

    parse_teardown(&cu);
}

void test_parse_boolean_error_left_no_value()
{
	Zinc_test_name(__func__);

	
	struct Ake_comp_unit cu;


    parse_setup("fn foo() end; foo() && true", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "left-side operand of boolean operator has no type");

    parse_teardown(&cu);
}

void test_parse_boolean_error_right_no_value()
{
	Zinc_test_name(__func__);

	
	struct Ake_comp_unit cu;


    parse_setup("fn foo() end; true && foo()", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "operand of boolean operator has no type");

    parse_teardown(&cu);
}

void test_parse_boolean_error_left_not_boolean()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("1 && true", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "left-side expression of boolean operator is not boolean");

    parse_teardown(&cu);
}

void test_parse_boolean_error_right_not_boolean()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("true && 1", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "cu.valid");
	Zinc_expect_source_error(&cu.el, "expression of boolean operator is not boolean");

    parse_teardown(&cu);
}

void test_parse_array_declare()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let a: [5]i64\n"
                "a[0]\n",
                &cu);
    Zinc_expect_no_errors(&cu.el);
    Zinc_expect_true(cu.valid, "valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_array_subscript()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let a: [10]i64; a[1]", &cu);
	Zinc_expect_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* as = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(as, "ptr as");
	Zinc_expect_int_equal(as->type, Ake_ast_type_array_subscript, "array-subscript as");

	Ake_type_use* tu = as->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_integer, "integer td");
	Zinc_expect_str(&td->name, "i64", "i64 td");

	Ake_ast* name = Ast_node_get(as, 0);
	Zinc_assert_ptr(name, "ptr name");
	Zinc_expect_int_equal(name->type, Ake_ast_type_id, "id name");
	Zinc_expect_str(&name->value, "a", "a name");

	Ake_ast* index = Ast_node_get(as, 1);
	Zinc_assert_ptr(index, "ptr index");
	Zinc_expect_int_equal(index->type, Ake_ast_type_number, "number index");
	Zinc_expect_str(&index->value, "1", "1 index");

    parse_teardown(&cu);
}

void test_parse_array_subscript2()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("let a: [10][10]i64; a[0][1]", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

    Ake_ast* a = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(a, "ptr a");
	Zinc_assert_int_equal(a->type, Ake_ast_type_array_subscript, "array-subscript a");

	Ake_type_use* a_tu = a->tu;
	Zinc_assert_ptr(a_tu, "ptr tu");
    Zinc_expect_false(a_tu->is_array, "is array to");

	struct Ake_type_def* td = a_tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_integer, "integer td");
	Zinc_expect_str(&td->name, "i64", "i64 td");

	Ake_ast* b = Ast_node_get(a, 0);
	Zinc_assert_ptr(b, "ptr b");
	Zinc_expect_int_equal(b->type, Ake_ast_type_array_subscript, "array_subscript b");

    Ake_ast* c = Ast_node_get(b, 0);
    Zinc_assert_ptr(c, "ptr c");
    Zinc_expect_int_equal(c->type, Ake_ast_type_id, "id c");
    Zinc_expect_str(&c->value, "a", "value a");

    Ake_ast* d = Ast_node_get(b, 1);
    Zinc_assert_ptr(d, "ptr d");
    Zinc_expect_int_equal(d->type, Ake_ast_type_number, "type d");
    Zinc_expect_str(&d->value, "0", "value d");

	Ake_ast* e = Ast_node_get(a, 1);
	Zinc_assert_ptr(e, "ptr e");
	Zinc_expect_int_equal(d->type, Ake_ast_type_number, "number e");
    Zinc_expect_str(&e->value, "1", "value e");

    parse_teardown(&cu);
}

void test_parse_array_subscript3()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let a: [5]i64; let b: i64; a[b]", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Ake_ast* a = Ast_node_get(cu.root, 2);
	Zinc_assert_ptr(a, "ptr a");
	Zinc_expect_int_equal(a->type, Ake_ast_type_array_subscript, "type a");

	Ake_type_use* tu = a->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_integer, "integer td");
	Zinc_expect_str(&td->name, "i64", "i64 td");

	Ake_ast* b = Ast_node_get(a, 0);
	Zinc_expect_int_equal(b->type, Ake_ast_type_id, "type b");
	Zinc_expect_str(&b->value, "a", "a b");

	Ake_ast* c = Ast_node_get(a, 1);
	Zinc_expect_int_equal(c->type, Ake_ast_type_id, "type c");
	Zinc_expect_str(&c->value, "b", "value c");

    parse_teardown(&cu);
}

void test_parse_expr_array_subscript_3d()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

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
    Zinc_assert_no_errors(&cu.el);
    Zinc_expect_true(cu.valid, "valid");

    Ake_ast* let = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(let, "per let");
    Zinc_expect_int_equal(let->type, Ake_ast_type_let, "type let");

    Ake_ast* let_type = Ast_node_get(let, 1);
    Zinc_assert_ptr(let_type, "ptr let_type");
    Zinc_expect_int_equal(let_type->type, Ake_ast_type_type, "type let_type");

    Ake_ast* a = Ast_node_get(cu.root, 1);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_assert_int_equal(a->type, Ake_ast_type_array_subscript, "type a");

    Ake_type_use* a_tu = a->tu;
    Zinc_assert_ptr(a_tu, "ptr a_tu");
    Zinc_expect_false(a_tu->is_mut, "is_mut a_tu");
    Zinc_expect_false(a_tu->is_array, "is_array a_tu");
    Zinc_expect_size_t_equal(a_tu->dim.count, 0, "dim.count a_tu");

    struct Ake_type_def* a_td = a_tu->td;
    Zinc_assert_ptr(a_td, "ptr a_td");
    Zinc_expect_int_equal(a_td->type, Ake_type_integer, "type a_td");
    Zinc_expect_str(&a_td->name, "i64", "name a_td");

    Ake_ast* b = Ast_node_get(a, 0);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Ake_ast_type_array_subscript, "type b");

    Ake_type_use* b_tu = b->tu;
    Zinc_assert_ptr(b_tu, "ptr b_tu");
    Zinc_expect_true(b_tu->is_array, "is_array b_tu");
    Zinc_expect_size_t_equal(b_tu->dim.count, 1, "dim.count b_tu");

    Ake_type_dimension* b_dim0 = (Ake_type_dimension*)ZINC_VECTOR_PTR(&b_tu->dim, 0);
    Zinc_assert_ptr(b_dim0, "ptr b_dim0");
    Zinc_expect_size_t_equal(b_dim0->size, 4, "size b_dim0");
    Zinc_expect_size_t_equal(b_dim0->option, Ake_array_element_const, "option b_dim0");

    Ake_ast* c = Ast_node_get(b, 0);
    Zinc_assert_ptr(c, "ptr c");
    Zinc_expect_int_equal(c->type, Ake_ast_type_array_subscript, "type c");

    Ake_type_use* c_tu = c->tu;
    Zinc_assert_ptr(c_tu, "ptr c_tu");
    Zinc_expect_true(c_tu->is_mut, "is_mut c_tu");
    Zinc_expect_true(c_tu->is_array, "is_array c_tu");
    Zinc_expect_size_t_equal(c_tu->dim.count, 2, "dim.count c_tu");

    Ake_type_dimension* c_dim0 = (Ake_type_dimension*)ZINC_VECTOR_PTR(&c_tu->dim, 0);
    Zinc_assert_ptr(c_dim0, "ptr c_dim0");
    Zinc_expect_size_t_equal(c_dim0->size, 3, "size c_dim0");
    Zinc_expect_int_equal(c_dim0->option, Ake_array_element_default, "option c_dim0");

    Ake_type_dimension* c_dim1 = (Ake_type_dimension*)ZINC_VECTOR_PTR(&c_tu->dim, 1);
    Zinc_assert_ptr(c_dim1, "ptr c_dim1");
    Zinc_expect_size_t_equal(c_dim1->size, 4, "size c_dim1");
    Zinc_expect_int_equal(c_dim1->option, Ake_array_element_const, "option c_dim1");

    Ake_ast* d = Ast_node_get(c, 0);
    Zinc_assert_ptr(d, "ptr d");
    Zinc_expect_int_equal(d->type, Ake_ast_type_id, "type d");

    Ake_type_use* d_tu = d->tu;
    Zinc_assert_ptr(d_tu, "ptr d_tu");
    Zinc_expect_true(d_tu->is_mut, "is_mut d_tu");
    Zinc_expect_true(d_tu->is_array, "is_array d_tu");
    Zinc_expect_size_t_equal(d_tu->dim.count, 3, "dim.count d_tu");

    Ake_type_dimension* d_dim0 = (Ake_type_dimension*)ZINC_VECTOR_PTR(&d_tu->dim, 0);
    Zinc_assert_ptr(d_dim0, "ptr d_dim0");
    Zinc_expect_size_t_equal(d_dim0->size, 2, "size d_dim0");
    Zinc_expect_int_equal(d_dim0->option, Ake_array_element_default, "option d_dim0");

    Ake_type_dimension* d_dim1 = (Ake_type_dimension*)ZINC_VECTOR_PTR(&d_tu->dim, 1);
    Zinc_assert_ptr(d_dim1, "ptr d_dim1");
    Zinc_expect_size_t_equal(d_dim1->size, 3, "size d_dim1");
    Zinc_expect_int_equal(d_dim1->option, Ake_array_element_default, "option d_dim1");

    Ake_type_dimension* d_dim2 = (Ake_type_dimension*)ZINC_VECTOR_PTR(&d_tu->dim, 2);
    Zinc_assert_ptr(d_dim2, "ptr d_dim2");
    Zinc_expect_size_t_equal(d_dim2->size, 4, "size d_dim2");
    Zinc_expect_int_equal(d_dim2->option, Ake_array_element_const, "option d_dim2");

    Ake_ast* e = Ast_node_get(c, 1);
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_int_equal(e->type, Ake_ast_type_number, "type e");

    parse_teardown(&cu);
}

void test_parse_subscript_error_no_type()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; foo()[1]", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "valid");
	Zinc_expect_source_error(&cu.el, "expression has subscript but has no value");

    parse_teardown(&cu);
}

void test_parse_subscript_error_not_array()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let a: i64; a[1]", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "valid");
	Zinc_expect_source_error(&cu.el, "expression has subscript but is not an array or slice");

    parse_teardown(&cu);
}

void test_parse_subscript_error_expected_right_square_bracket()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let a: [10]i64; a[1", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "valid");
	Zinc_expect_source_error(&cu.el, "expected right-square-bracket");

    parse_teardown(&cu);
}

void test_parse_assign_string()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let mut a: [6 const]u8; a = \"hello\"", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts root");

	Ake_ast* assign = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(assign, "ptr assign");
	Zinc_expect_int_equal(assign->type, Ake_ast_type_assign, "assign assign");

	Ake_type_use* tu = assign->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_integer, "type td");
	Zinc_expect_str(&td->name, "u8", "name td");
		
	Ake_ast* lhv = Ast_node_get(assign, 0);
	Zinc_assert_ptr(lhv, "ptr lhv");
	Zinc_expect_int_equal(lhv->type, Ake_ast_type_id, "id lhv");
	Zinc_expect_str(&lhv->value, "a", "a lhv");

	Ake_ast* rhv = Ast_node_get(assign, 1);
	Zinc_assert_ptr(rhv, "ptr rhv");
	Zinc_expect_int_equal(rhv->type, Ake_ast_type_string, "string rhv");
	Zinc_expect_str(&rhv->value, "hello", "hello rhv");

    parse_teardown(&cu);
}

void test_parse_assign_multiple()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let mut a: i64; let mut b: i64; let mut c: i64; a = b = c = 0", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* assign = Ast_node_get(cu.root, 3);
	Zinc_assert_ptr(assign, "ptr assign");
	Zinc_expect_int_equal(assign->type, Ake_ast_type_assign, "assign assign");

	Ake_type_use* assign_tu = assign->tu;
	Zinc_assert_ptr(assign_tu, "ptr assign_tu");

	struct Ake_type_def* assign_td = assign_tu->td;
	Zinc_assert_ptr(assign_td, "ptr assign_td");
	Zinc_expect_int_equal(assign_td->type, Ake_type_integer, "integer assign_td");
	Zinc_expect_str(&assign_td->name, "i64", "i64 assign_td");

	Ake_ast* assign0 = Ast_node_get(assign, 0);
	Zinc_assert_ptr(assign0, "ptr lhv0");
	Zinc_expect_int_equal(assign0->type, Ake_ast_type_id, "id assign0");
	Zinc_expect_str(&assign0->value, "a", "assign0 value");

	Ake_ast* assign1 = Ast_node_get(assign, 1);
	Zinc_assert_ptr(assign1, "ptr assign1");
	Zinc_expect_int_equal(assign1->type, Ake_ast_type_id, "assign assign1");
    Zinc_expect_str(&assign1->value, "b", "assign1 value");

    Ake_ast* assign2 = Ast_node_get(assign, 2);
    Zinc_assert_ptr(assign2, "ptr assign2");
    Zinc_expect_int_equal(assign2->type, Ake_ast_type_id, "assign assign2");
    Zinc_expect_str(&assign2->value, "c", "assign2 value");

    Ake_ast* assign3 = Ast_node_get(assign, 3);
    Zinc_assert_ptr(assign3, "ptr assign3");
    Zinc_expect_int_equal(assign3->type, Ake_ast_type_number, "assign assign3");
    Zinc_expect_str(&assign3->value, "0", "assign3 value");

    parse_teardown(&cu);
}

void test_parse_expr_assignment_eseq_error_eseq_count()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let mut a: i64; let mut b: i64; let mut c: i64; a, b, c = 1, 2", &cu);
    Zinc_expect_has_errors(&cu.el);
    Zinc_expect_false(cu.valid, "valid");
    Zinc_expect_source_error(&cu.el, "assignment sequence counts do not match");

    parse_teardown(&cu);
}

void test_parse_let_assign_error_term()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("let a: bool =", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "valid");
	Zinc_expect_source_error(&cu.el, "expected expression");

    parse_teardown(&cu);
}

void test_parse_assign_error_no_value_right()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; let a: [10]u8 = foo()", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "valid");
	Zinc_expect_source_error(&cu.el, "cannot assign with operand that has no value");

    parse_teardown(&cu);
}

void test_parse_assign_error_not_compatible()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let x: i64 = true", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "valid");
	Zinc_expect_source_error(&cu.el, "values in assignment are not compatible");

    parse_teardown(&cu);
}

void test_parse_assign_error_lvalue()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("true = true", &cu);
	Zinc_assert_has_errors(&cu.el);
	Zinc_expect_false(cu.valid, "valid");
	Zinc_expect_source_error(&cu.el, "invalid lvalue");

    parse_teardown(&cu);
}

void test_parse_expr_newline_assignment()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let mut a: i64; a =\n1", &cu);
    Zinc_assert_no_errors(&cu.el);
    Zinc_expect_true(cu.valid, "valid");

    Ake_ast* op = Ast_node_get(cu.root, 1);
    Zinc_assert_ptr(op, "ptr op");
    Zinc_expect_int_equal(op->type, Ake_ast_type_assign, "assign op");

    Ake_ast* one = Ast_node_get(op, 0);
    Zinc_assert_ptr(one, "one");
    Zinc_expect_int_equal(one->type, Ake_ast_type_id, "id one");
    Zinc_expect_str(&one->value, "a", "a one");

    Ake_ast* two = Ast_node_get(op, 1);
    Zinc_assert_ptr(two, "two");
    Zinc_expect_int_equal(two->type, Ake_ast_type_number, "number two");
    Zinc_expect_str(&two->value, "1", "1 two");

    parse_teardown(&cu);
}

void test_parse_expr_newline_boolean()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("true &&\nfalse", &cu);
    Zinc_assert_no_errors(&cu.el);
    Zinc_expect_true(cu.valid, "valid");

    Ake_ast* op = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(op, "ptr add");
    Zinc_expect_int_equal(op->type, Ake_ast_type_and, "less than op");

    Ake_ast* one = Ast_node_get(op, 0);
    Zinc_assert_ptr(one, "one");
    Zinc_expect_int_equal(one->type, Ake_ast_type_boolean, "boolean one");
    Zinc_expect_str(&one->value, "true", "true");

    Ake_ast* two = Ast_node_get(op, 1);
    Zinc_assert_ptr(two, "two");
    Zinc_expect_int_equal(two->type, Ake_ast_type_boolean, "boolean two");
    Zinc_expect_str(&two->value, "false", "false");

    parse_teardown(&cu);
}

void test_parse_expr_newline_comparison()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("1 <\n2", &cu);
    Zinc_assert_no_errors(&cu.el);
    Zinc_expect_true(cu.valid, "valid");

    Ake_ast* op = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(op, "ptr add");
    Zinc_expect_int_equal(op->type, Ake_ast_type_less_than, "less than op");

    Ake_ast* one = Ast_node_get(op, 0);
    Zinc_assert_ptr(one, "one");
    Zinc_expect_int_equal(one->type, Ake_ast_type_number, "number one");
    Zinc_expect_str(&one->value, "1", "1");

    Ake_ast* two = Ast_node_get(op, 1);
    Zinc_assert_ptr(two, "two");
    Zinc_expect_int_equal(two->type, Ake_ast_type_number, "number two");
    Zinc_expect_str(&two->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_expr_newline_add()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("1 +\n2", &cu);
    Zinc_assert_no_errors(&cu.el);
    Zinc_expect_true(cu.valid, "valid");

    Ake_ast* add = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(add, "ptr add");
    Zinc_expect_int_equal(add->type, Ake_ast_type_plus, "plus add");

    Ake_ast* one = Ast_node_get(add, 0);
    Zinc_assert_ptr(one, "one");
    Zinc_expect_int_equal(one->type, Ake_ast_type_number, "number one");
    Zinc_expect_str(&one->value, "1", "1");

    Ake_ast* two = Ast_node_get(add, 1);
    Zinc_assert_ptr(two, "two");
    Zinc_expect_int_equal(two->type, Ake_ast_type_number, "number two");
    Zinc_expect_str(&two->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_expr_newline_mult()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("1 *\n2", &cu);
    Zinc_assert_no_errors(&cu.el);
    Zinc_expect_true(cu.valid, "valid");

    Ake_ast* op = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(op, "ptr op");
    Zinc_expect_int_equal(op->type, Ake_ast_type_mult, "plus op");

    Ake_ast* one = Ast_node_get(op, 0);
    Zinc_assert_ptr(one, "one");
    Zinc_expect_int_equal(one->type, Ake_ast_type_number, "number one");
    Zinc_expect_str(&one->value, "1", "1");

    Ake_ast* two = Ast_node_get(op, 1);
    Zinc_assert_ptr(two, "two");
    Zinc_expect_int_equal(two->type, Ake_ast_type_number, "number two");
    Zinc_expect_str(&two->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_expr_newline_power()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("1^\n2", &cu);
    Zinc_assert_no_errors(&cu.el);
    Zinc_expect_true(cu.valid, "valid");

    Ake_ast* op = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(op, "ptr op");
    Zinc_expect_int_equal(op->type, Ake_ast_type_power, "plus op");

    Ake_ast* one = Ast_node_get(op, 0);
    Zinc_assert_ptr(one, "one");
    Zinc_expect_int_equal(one->type, Ake_ast_type_number, "number one");
    Zinc_expect_str(&one->value, "1", "1");

    Ake_ast* two = Ast_node_get(op, 1);
    Zinc_assert_ptr(two, "two");
    Zinc_expect_int_equal(two->type, Ake_ast_type_number, "number two");
    Zinc_expect_str(&two->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_expr_newline_subscript()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let a: [1]i64; a[\n0\n]", &cu);
    Zinc_assert_no_errors(&cu.el);
    Zinc_expect_true(cu.valid, "valid");

    Ake_ast* subscript = Ast_node_get(cu.root, 1);
    Zinc_assert_ptr(subscript, "ptr subscript");
    Zinc_expect_int_equal(subscript->type, Ake_ast_type_array_subscript, "array_subscript subscript");

    Ake_ast* array = Ast_node_get(subscript, 0);
    Zinc_assert_ptr(array, "array");
    Zinc_expect_int_equal(array->type, Ake_ast_type_id, "id array");
    Zinc_expect_str(&array->value, "a", "a");

    Ake_ast* zero = Ast_node_get(subscript, 1);
    Zinc_assert_ptr(zero, "zero");
    Zinc_expect_int_equal(zero->type, Ake_ast_type_number, "number zero");
    Zinc_expect_str(&zero->value, "0", "0");

    parse_teardown(&cu);
}

void test_parse_expr_newline_function_call()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(a: i64, b: i64)->i64 a+b end; foo(\n1,\n2\n)", &cu);
    Zinc_assert_no_errors(&cu.el);
    Zinc_expect_true(cu.valid, "valid");

    Ake_ast* call = Ast_node_get(cu.root, 1);
    Zinc_assert_ptr(call, "ptr call");
    Zinc_expect_int_equal(call->type, Ake_ast_type_call, "call call");

    Ake_ast* foo = Ast_node_get(call, 0);
    Zinc_assert_ptr(foo, "ptr foo");
    Zinc_expect_int_equal(foo->type, Ake_ast_type_id, "id foo");
    Zinc_expect_str(&foo->value, "foo", "foo");

    Ake_ast* cseq = Ast_node_get(call, 1);
    Zinc_assert_ptr(cseq, "ptr cseq");
    Zinc_expect_int_equal(cseq->type, Ake_ast_type_cseq, "cseq cseq");

    Ake_ast* one = Ast_node_get(cseq, 0);
    Zinc_assert_ptr(one, "ptr one");
    Zinc_expect_int_equal(one->type, Ake_ast_type_number, "number one");
    Zinc_expect_str(&one->value, "1", "1");

    Ake_ast* two = Ast_node_get(cseq, 1);
    Zinc_assert_ptr(two, "ptr two");
    Zinc_expect_int_equal(two->type, Ake_ast_type_number, "number two");
    Zinc_expect_str(&two->value, "2", "2");

    parse_teardown(&cu);
}

void test_parse_expr_assign_eseq()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let mut a: i64\n"
                "let mut b: i64\n"
                "let mut c: i64\n"
                "a,b,c = 1,2,3", &cu);
    Zinc_expect_no_errors(&cu.el);
    Zinc_expect_true(cu.valid, "valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* assign = Ast_node_get(cu.root, 3);
    Zinc_assert_ptr(assign, "ptr assign");
    Zinc_expect_int_equal(assign->type, Ake_ast_type_assign, "assign assign");

    Ake_ast* lvalue = Ast_node_get(assign, 0);
    Zinc_assert_ptr(lvalue, "ptr lvalue");
    Zinc_expect_int_equal(lvalue->type, Ake_ast_type_eseq, "eseq lvalue");

    Ake_ast* a = Ast_node_get(lvalue, 0);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Ake_ast_type_id, "id a");
    Zinc_expect_str(&a->value, "a", "a");

    Ake_ast* b = Ast_node_get(lvalue, 1);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Ake_ast_type_id, "id b");
    Zinc_expect_str(&b->value, "b", "b");

    Ake_ast* c = Ast_node_get(lvalue, 2);
    Zinc_assert_ptr(c, "ptr c");
    Zinc_expect_int_equal(c->type, Ake_ast_type_id, "id c");
    Zinc_expect_str(&c->value, "c", "c");

    parse_teardown(&cu);
}

void test_parse_expr_error_lvalue()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let a: i64\n"
                "1 = 1", &cu);
    Zinc_expect_has_errors(&cu.el);
    Zinc_expect_source_error(&cu.el, "invalid lvalue");
    Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_expr_error_eseq_lvalue()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let a: i64\n"
                "let b: i64\n"
                "let c: i64\n"
                "1,2,3 = 1,2,3", &cu);
    Zinc_expect_has_errors(&cu.el);
    Zinc_expect_source_error(&cu.el, "invalid lvalue");
    Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_array_string_return()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(a: i64, b: i64, c: i64)->[6 const]u8\n"
                "  \"hello\"\n"
                "end\n"
                "foo(1, 2, 3)\n",
                &cu);
    Zinc_expect_no_errors(&cu.el);
    Zinc_expect_true(cu.valid, "valid");

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