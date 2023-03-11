#include <stdbool.h>
#include "zinc/unit_test.h"
#include "alba/parse_tools.h"
#include "alba/unit_test_compiler.h"
#include "alba/ast.h"
#include "test_parse.h"
#include "alba/type_def.h"
#include "alba/comp_unit.h"
#include "zinc/error_unit_test.h"

void test_parse_var()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var a::Int32", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* var = ast_node_get(cu.root, 0);
	assert_ptr(var, "ptr var");
	expect_int_equal(var->type, ast_type_var, "var");

	struct ast_node* dec = ast_node_get(var, 0);
	assert_ptr(dec, "ptr dec");
	expect_int_equal(dec->type, ast_type_declaration, "declaration");

	struct ast_node* id = ast_node_get(dec, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "a", "a");

	struct ast_node* tu = ast_node_get(dec, 1);
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_str(&td->name, "Int32", "Int32 td");

	/* destroy ps{} cu.root cu.root{} */
	parse_teardown2(&cu);
}

void test_parse_var2()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var a::Int32 = 1", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* assign = ast_node_get(cu.root, 0);
	assert_ptr(assign, "ptr assign");
	expect_int_equal(assign->type, ast_type_assign, "assign assign");

	struct ast_node* var = ast_node_get(assign, 0);
	assert_ptr(var, "ptr var");
	expect_int_equal(var->type, ast_type_var, "var");

	struct ast_node* dec = ast_node_get(var, 0);
	assert_ptr(dec, "ptr dec");
	expect_int_equal(dec->type, ast_type_declaration, "declaration");

	struct ast_node* id = ast_node_get(dec, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id");
	expect_str(&id->value, "a", "a");

	struct ast_node* tu = ast_node_get(dec, 1);
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_str(&td->name, "Int32", "Int32 td");

	struct ast_node* value = ast_node_get(assign, 1);
	assert_ptr(value, "ptr value");
	expect_int_equal(value->type, ast_type_number, "number");
	expect_str(&value->value, "1", "1");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_var_expected_declaration()
{
	test_name(__func__);

	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse valid");
	expect_error(&cu.el, "expected declaration after var");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_number_integer()
{
	test_name(__func__);
	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("32", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* number = ast_node_get(cu.root, 0);
	assert_ptr(number, "ptr number");
	expect_int_equal(number->type, ast_type_number, "number num");
	expect_str(&number->value, "32", "32 num");

	struct ast_node* tu = number->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "Int64", "Int64 td");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_number_float()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("5.0e0", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* number = ast_node_get(cu.root, 0);
	assert_ptr(number, "ptr num");
	expect_int_equal(number->type, ast_type_number, "number number");
	expect_str(&number->value, "5.0e0", "5.0e0 number");

	struct ast_node* tu = number->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_float, "integer td");
	expect_str(&td->name, "Float64", "Float64 td");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_string()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("\"hello\"", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* string = ast_node_get(cu.root, 0);
	assert_ptr(string, "ptr string");
	expect_int_equal(string->type, ast_type_string, "string string");
	expect_str(&string->value, "hello", "hello string");

	struct ast_node* tu = string->tu;
	assert_ptr(tu, "ptr tu");
	
	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_string, "string td");
	expect_str(&td->name, "String", "String td");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_boolean_true()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("true", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* true_node = ast_node_get(cu.root, 0);
	assert_ptr(true_node, "ptr true_node");
	expect_int_equal(true_node->type, ast_type_boolean, "boolean true_node");
	expect_str(&true_node->value, "true", "true true_node");

	struct ast_node* tu = true_node->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "Bool", "Bool td");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_boolean_false()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("false", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* false_node = ast_node_get(cu.root, 0);
	assert_ptr(false_node, "ptr false_node");
	expect_int_equal(false_node->type, ast_type_boolean, "boolean false_node");
	expect_str(&false_node->value, "false", "false false_node");

	struct ast_node* tu = false_node->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "Bool", "Bool td");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_id()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var x::Int64; x", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* var = ast_node_get(cu.root, 0);
	assert_ptr(var, "ptr var");
	assert_int_equal(var->type, ast_type_var, "let let");

	struct ast_node* var_dec = ast_node_get(var, 0);
	assert_ptr(var_dec, "ptr var_dec");
	assert_int_equal(var_dec->type, ast_type_declaration, "declaration var_dec");

	struct ast_node* var_id = ast_node_get(var_dec, 0);
	assert_ptr(var_id, "ptr var_id");
	expect_int_equal(var_id->type, ast_type_id, "id var_id");
	expect_str(&var_id->value, "x", "x");

	struct ast_node* var_type = ast_node_get(var_dec, 1);
	assert_ptr(var_type, "ptr var_type");
	expect_int_equal(var_type->type, ast_type_type, "type_type var_type");

	struct ast_node* id = ast_node_get(cu.root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "x", "x id");

	struct ast_node* tu = id->tu;
	assert_ptr(tu, "ptr tu");
	
	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "Int64", "Int64 td");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_id2()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var _a23::Int64; _a23", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* id = ast_node_get(cu.root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "_a23", "_a23 id");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_id3()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var a2::Int64; a2", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* id = ast_node_get(cu.root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "a2", "a2 id");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_sign_negative()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("-30", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* sign = ast_node_get(cu.root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, ast_type_sign, "sign sign");

	struct ast_node* tu = sign->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "Int64", "Int64 td");

	struct ast_node* left = ast_node_get(sign, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_minus, "minus");

	struct ast_node* right = ast_node_get(sign, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "30", "30");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_sign_positive()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("+30", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* sign = ast_node_get(cu.root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, ast_type_sign, "sign sign");

	struct ast_node* tu = sign->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "Int64", "Int64 td");

	struct ast_node* left = ast_node_get(sign, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, ast_type_plus, "plus");

	struct ast_node* right = ast_node_get(sign, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "30", "30");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_sign_error_no_value()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo() end\n-foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "negative operator was used on expression with no value");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_sign_expected_factor()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("-", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "expected factor after sign");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_not_id()
{
	test_name(__func__);

	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var a::Bool; !a", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* not = ast_node_get(cu.root, 1);
	assert_ptr(not, "ptr not");
	expect_int_equal(not->type, ast_type_not, "not not");

	struct ast_node* tu = not->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "Bool", "Bool td");

	struct ast_node* id = ast_node_get(not, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "a", "a id");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_not_literal()
{
	test_name(__func__);

	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("!true", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* not = ast_node_get(cu.root, 0);
	assert_ptr(not, "ptr not");
	expect_int_equal(not->type, ast_type_not, "not not");

	struct ast_node* tu = not->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "Bool", "Bool td");

	struct ast_node* lit_bool = ast_node_get(not, 0);
	assert_ptr(lit_bool, "ptr lit_bool");
	expect_int_equal(lit_bool->type, ast_type_boolean, "boolean true");
	expect_str(&lit_bool->value, "true", "true lit_bool");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_not_error()
{
	test_name(__func__);

	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo() end; !foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "! operator used on factor with no value");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_array_literal_integer()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("[1,2,3]", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* a = ast_node_get(cu.root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_array_literal, "array-literal a");

	struct ast_node* array_tu = a->tu;
	assert_ptr(array_tu, "ptr array_tu");

	struct type_def* array_td = array_tu->td;
	assert_ptr(array_td, "ptr array_td");
	expect_int_equal(array_td->type, type_integer, "integer array_td");
	expect_str(&array_td->name, "Int64", "Int64 array_td");

	struct ast_node* a0 = ast_node_get(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, ast_type_number, "number a0");
	expect_str(&a0->value, "1", "1 a0");

	struct ast_node* a1 = ast_node_get(a, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, ast_type_number, "number a1");
	expect_str(&a1->value, "2", "2 a1");

	struct ast_node* a2 = ast_node_get(a, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, ast_type_number, "number a2");
	expect_str(&a2->value, "3", "3 a2");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_array_literal_float()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("[1.0,2.5,3.2]", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* a = ast_node_get(cu.root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_array_literal, "array-literal a");

	struct ast_node* array_tu = a->tu;
	assert_ptr(array_tu, "ptr array_tu");

	struct type_def* array_td = array_tu->td;
	assert_ptr(array_td, "ptr array_td");
	expect_int_equal(array_td->type, type_float, "float array_td");
	expect_str(&array_td->name, "Float64", "Float64 array_td");

	struct ast_node* a0 = ast_node_get(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, ast_type_number, "number a0");
	expect_str(&a0->value, "1.0", "1.0 a0");

	struct ast_node* a1 = ast_node_get(a, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, ast_type_number, "number a1");
	expect_str(&a1->value, "2.5", "2.5 a1");

	struct ast_node* a2 = ast_node_get(a, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, ast_type_number, "number a2");
	expect_str(&a2->value, "3.2", "3.2 a2");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_array_literal_numeric()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("[1, 2.5, 3]", &cu);
	expect_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* a = ast_node_get(cu.root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_array_literal, "array-literal a");

	struct ast_node* array_tu = a->tu;
	assert_ptr(array_tu, "ptr array_tu");

	struct type_def* array_td = array_tu->td;
	assert_ptr(array_td, "ptr array_td");
	expect_int_equal(array_td->type, type_float, "float array_td");
	expect_str(&array_td->name, "Float64", "Float64 array_td");

	struct ast_node* a0 = ast_node_get(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, ast_type_number, "number a0");
	expect_str(&a0->value, "1", "1 a0");

	struct ast_node* a1 = ast_node_get(a, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, ast_type_number, "number a1");
	expect_str(&a1->value, "2.5", "2.5 a1");

	struct ast_node* a2 = ast_node_get(a, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, ast_type_number, "number a2");
	expect_str(&a2->value, "3", "3 a2");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_array_literal_mixed_error()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("[1,true,3]", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "array elements not the same type");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_array_literal_empty_error()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("[]", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "array literal has no elements");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_array_literal_error_right_square_bracket()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("[1,2", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "expected right square bracket");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_array_literal_error_expected_expr()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("[1,]", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "expected expr after comma");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_anonymous_function()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var a::Function{Input{Int32, Int32, Int32}}; a = function(x::Int32,y::Int32,z::Int32) 1 end", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* assign = ast_node_get(cu.root, 1);
	assert_ptr(assign, "ptr assign");
	expect_int_equal(assign->type, ast_type_assign, "assign assign");

	struct ast_node* a = ast_node_get(assign, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	struct ast_node* f = ast_node_get(assign, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, ast_type_anonymous_function, "anonymous-function f");

	struct ast_node* dseq = ast_node_get(f, 0);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, ast_type_dseq, "dseq dseq");

	struct ast_node* dec_x = ast_node_get(dseq, 0);
	assert_ptr(dec_x, "ptr dec_x");
	expect_int_equal(dec_x->type, ast_type_declaration, "declaration dec_x");

	struct ast_node* x = ast_node_get(dec_x, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x x");

	struct ast_node* type_x = ast_node_get(dec_x, 1);
	assert_ptr(type_x, "ptr type_x");
	expect_int_equal(type_x->type, ast_type_type, "type type_x");

	struct ast_node* dec_y = ast_node_get(dseq, 1);
	assert_ptr(dec_y, "ptr dec_y");
	expect_int_equal(dec_y->type, ast_type_declaration, "declaration dec_y");

	struct ast_node* y = ast_node_get(dec_y, 0);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y y");

	struct ast_node* type_y = ast_node_get(dec_y, 1);
	assert_ptr(type_y, "ptr type_y");
	expect_int_equal(type_y->type, ast_type_type, "type type_y");

	struct ast_node* dec_z = ast_node_get(dseq, 2);
	assert_ptr(dec_z, "ptr dec_z");
	expect_int_equal(dec_z->type, ast_type_declaration, "declaration dec_z");

	struct ast_node* z = ast_node_get(dec_z, 0);
	assert_ptr(z, "ptr z");
	expect_int_equal(z->type, ast_type_id, "id z");
	expect_str(&z->value, "z", "z z");

	struct ast_node* type_z = ast_node_get(dec_z, 1);
	assert_ptr(type_z, "ptr type_z");
	expect_int_equal(type_z->type, ast_type_type, "type type_z");

	struct ast_node* dret = ast_node_get(f, 1);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* stmts = ast_node_get(f, 2);
	assert_ptr(stmts, "ptr stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "stmts stmts");

	struct ast_node* one = ast_node_get(stmts, 0);
	assert_ptr(one, "ptr one");
	expect_int_equal(one->type, ast_type_number, "number one");
	expect_str(&one->value, "1", "1 one");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_anonymous_function2()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var a::Function{Input{Int32, Int32, Int32}, Output{Int32}}; a = function(x::Int32,y::Int32,z::Int32)::Int32 1 end", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* assign = ast_node_get(cu.root, 1);
	assert_ptr(assign, "ptr assign");
	expect_int_equal(assign->type, ast_type_assign, "assign assign");

	struct ast_node* a = ast_node_get(assign, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	struct ast_node* f = ast_node_get(assign, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, ast_type_anonymous_function, "anonymous-function f");

	struct ast_node* dseq = ast_node_get(f, 0);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, ast_type_dseq, "dseq dseq");

	struct ast_node* dec_x = ast_node_get(dseq, 0);
	assert_ptr(dec_x, "ptr dec_x");
	expect_int_equal(dec_x->type, ast_type_declaration, "declaration dec_x");

	struct ast_node* x = ast_node_get(dec_x, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x x");

	struct ast_node* type_x = ast_node_get(dec_x, 1);
	assert_ptr(type_x, "ptr type_x");
	expect_int_equal(type_x->type, ast_type_type, "type_type type_x");

	struct ast_node* dec_y = ast_node_get(dseq, 1);
	assert_ptr(dec_y, "ptr dec_y");
	expect_int_equal(dec_y->type, ast_type_declaration, "declaration dec_y");

	struct ast_node* y = ast_node_get(dec_y, 0);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y y");

	struct ast_node* type_y = ast_node_get(dec_y, 1);
	assert_ptr(type_y, "ptr name_y");
	expect_int_equal(type_y->type, ast_type_type, "type type_y");

	struct ast_node* dec_z = ast_node_get(dseq, 2);
	assert_ptr(dec_z, "ptr dec_z");
	expect_int_equal(dec_z->type, ast_type_declaration, "declaration dec_z");

	struct ast_node* z = ast_node_get(dec_z, 0);
	assert_ptr(z, "ptr z");
	expect_int_equal(z->type, ast_type_id, "id z");
	expect_str(&z->value, "z", "z z");

	struct ast_node* type_z = ast_node_get(dec_z, 1);
	assert_ptr(type_z, "ptr type_z");
	expect_int_equal(type_z->type, ast_type_type, "type type_z");

	struct ast_node* dret = ast_node_get(f, 1);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* dret_type = ast_node_get(dret, 0);
	assert_ptr(dret_type, "ptr dret_type");
	expect_int_equal(dret_type->type, ast_type_type, "id dret_type");

	struct ast_node* stmts = ast_node_get(f, 2);
	assert_ptr(stmts, "ptr stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "stmts stmts");

	struct ast_node* one = ast_node_get(stmts, 0);
	assert_ptr(one, "ptr one");
	expect_int_equal(one->type, ast_type_number, "number one");
	expect_str(&one->value, "1", "1 one");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_anonymous_function3()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var a::Function; a = function(x::Int64) var x::Int64 = 1 end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse valid");
	expect_error(&cu.el, "duplicate declaration in same scope: x");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_anonymous_function_assignment_error()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var a::Function = function(x::Int64) end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse valid");
	expect_error(&cu.el, "values in assignment not compatible");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_anonymous_function_return_error()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var f::Function{Output{Int64}} = function()::Int64 true end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse valid");
	expect_error(&cu.el, "returned type does not match function return type");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_anonymous_function_expected_right_paren()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function(", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse valid");
	expect_error(&cu.el, "expected right parenthesis");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_anonymous_function_expected_end()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse valid");
	expect_error(&cu.el, "expected end");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_paren_num()
{
	test_name(__func__);

	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("(32)", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* paren = ast_node_get(cu.root, 0);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, ast_type_parenthesis, "parenthesis paren");

	struct ast_node* number = ast_node_get(paren, 0);
	expect_int_equal(number->type, ast_type_number, "number number");
	expect_str(&number->value, "32", "32 number");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_paren_error_empty()
{
	test_name(__func__);

	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("()", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "empty parenthesis");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_paren_error_right_parenthesis()
{
	test_name(__func__);

	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("(1", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "expected right parenthesis");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_paren_error_no_value()
{
	test_name(__func__);

	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo() end; (foo())", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "parenthesis on expression that has no value");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_call()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo() 1 end; foo()", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* fd = ast_node_get(cu.root, 0);
	assert_ptr(fd, "ptr fd");
	expect_int_equal(fd->type, ast_type_function, "function fd");

	struct ast_node* f_id = ast_node_get(fd, 0);
	assert_ptr(f_id, "ptr f_id");
	expect_int_equal(f_id->type, ast_type_id, "id f_id");
	expect_str(&f_id->value, "foo", "foo");

	struct ast_node* dseq = ast_node_get(fd, 1);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, ast_type_dseq, "dret dseq");

	struct ast_node* dret = ast_node_get(fd, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* a = ast_node_get(cu.root, 1);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_call, "call");

	struct ast_node* b = ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id");
	expect_str(&b->value, "foo", "foo");

	struct ast_node* c = ast_node_get(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, ast_type_cseq, "cseq");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_call_return_type()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo()::Int64 1 end; foo() + 2", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* f = ast_node_get(cu.root, 0);
	assert_ptr(f, "ptr f");

	struct ast_node* add = ast_node_get(cu.root, 1);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, ast_type_plus, "plus add");

	struct ast_node* add_tu = add->tu;
	assert_ptr(add, "patr add_tu");

	struct type_def* add_td = add_tu->td;
	assert_ptr(add_td, "ptr add_td");
	expect_int_equal(add_td->type, type_integer, "integer add_td");
	expect_str(&add_td->name, "Int64", "Int64 add_td");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_call_return_type_error()
{
	test_name(__func__);
	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo()::Bool true end; foo() + 2", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "addition on non-numeric operand");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_call2()
{
	test_name(__func__);
	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo(arg1::Int64) arg1 end; foo(2)", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* fd = ast_node_get(cu.root, 0);
	assert_ptr(fd, "ptr fd");
	expect_int_equal(fd->type, ast_type_function, "function fd");

	struct ast_node* fname = ast_node_get(fd, 0);
	assert_ptr(fname, "ptr fname");
	expect_int_equal(fname->type, ast_type_id, "id fname");
	expect_str(&fname->value, "foo", "foo fname");

	struct ast_node* dseq = ast_node_get(fd, 1);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, ast_type_dseq, "dseq dseq");

	struct ast_node* dparam0 = ast_node_get(dseq, 0);
	assert_ptr(dparam0, "ptr dparam0");
	expect_int_equal(dparam0->type, ast_type_declaration, "declaration dparam0");

	struct ast_node* param0_name = ast_node_get(dparam0, 0);
	assert_ptr(param0_name, "ptr param0_name");
	expect_int_equal(param0_name->type, ast_type_id, "id param0_name");
	expect_str(&param0_name->value, "arg1", "arg1");

	struct ast_node* tu = ast_node_get(dparam0, 1);
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "Int64", "Int64 td");

	struct ast_node* dret = ast_node_get(fd, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* f_stmts = ast_node_get(fd, 3);
	assert_ptr(f_stmts, "ptr f_stmts");
	expect_int_equal(f_stmts->type, ast_type_stmts, "stmts f_stmts");

	struct ast_node* a = ast_node_get(cu.root, 1);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_call, "call");

	struct ast_node* b = ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id");
	expect_str(&b->value, "foo", "foo");

	struct ast_node* cseq = ast_node_get(a, 1);
	assert_ptr(cseq, "ptr cseq");
	expect_int_equal(cseq->type, ast_type_cseq, "cseq");

	struct ast_node* cseq_a = ast_node_get(cseq, 0);
	assert_ptr(cseq_a, "ptr cseq_a");
	expect_int_equal(cseq_a->type, ast_type_number, "cseq_a");
	expect_str(&cseq_a->value, "2", "2 cseq_a");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_call3()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo(arg1::Int64, arg2::Int64)::Int64 1 end; var x::Int64; var y::Int64; foo(x,y)", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* fd = ast_node_get(cu.root, 0);
	assert_ptr(fd, "ptr fd");
	expect_int_equal(fd->type, ast_type_function, "function fd");

	struct ast_node* fname = ast_node_get(fd, 0);
	assert_ptr(fname, "ptr fname");
	expect_int_equal(fname->type, ast_type_id, "id fname");
	expect_str(&fname->value, "foo", "foo fname");

	struct ast_node* fd_seq = ast_node_get(fd, 1);
	assert_ptr(fd_seq, "ptr fdseq");
	expect_int_equal(fd_seq->type, ast_type_dseq, "dseq fd_seq");

	struct ast_node* fd_param0 = ast_node_get(fd_seq, 0);
	assert_ptr(fd_param0, "ptr fd_param0");
	expect_int_equal(fd_param0->type, ast_type_declaration, "declaration fd_param0");

	struct ast_node* param0_name = ast_node_get(fd_param0, 0);
	assert_ptr(param0_name, "ptr param0_name");
	expect_int_equal(param0_name->type, ast_type_id, "id param0_name");
	expect_str(&param0_name->value, "arg1", "arg1");

	struct ast_node* param0_type = ast_node_get(fd_param0, 1);
	assert_ptr(param0_type, "ptr param0_id");
	expect_int_equal(param0_type->type, ast_type_type, "type param0_id");

	struct ast_node* dret = ast_node_get(fd, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* a = ast_node_get(cu.root, 3);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_call, "call");

	struct ast_node* b = ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id");
	expect_str(&b->value, "foo", "foo");

	struct ast_node* cseq = ast_node_get(a, 1);
	assert_ptr(cseq, "ptr cseq");
	expect_int_equal(cseq->type, ast_type_cseq, "cseq");

	struct ast_node* cseq_a = ast_node_get(cseq, 0);
	assert_ptr(cseq_a, "ptr cseq_a");
	expect_int_equal(cseq_a->type, ast_type_id, "cseq_a");
	expect_str(&cseq_a->value, "x", "x cseq_a");

	struct ast_node* cseq_b = ast_node_get(cseq, 1);
	assert_ptr(cseq_b, "ptr cseq_b");
	expect_int_equal(cseq_b->type, ast_type_id, "cseq_b");
	expect_str(&cseq_b->value, "y", "y cseq_b");

	struct ast_node* cseq_c = ast_node_get(cseq, 2);
	assert_null(cseq_c, "null cseq_c");

	struct ast_node* c = ast_node_get(a, 2);
	assert_null(c, "null c");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

/* dynamic-output-none */
void test_parse_call4()
{
	test_name(__func__);
	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo(arg0::Int64, arg1::Int64, arg2::Int64)::Int64 100 end; var x::Int64; var y::Int64; foo(x, y, 1)", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "stmts cu.root");

	struct ast_node* fd = ast_node_get(cu.root, 0);
	assert_ptr(fd, "ptr fd");
	expect_int_equal(fd->type, ast_type_function, "function fd");

	struct ast_node* fd_id = ast_node_get(fd, 0);
	assert_ptr(fd_id, "ptr fd");
	expect_int_equal(fd_id->type, ast_type_id, "id fd_id");
	expect_str(&fd_id->value, "foo", "foo fd_id");

	struct ast_node* dseq = ast_node_get(fd, 1);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, ast_type_dseq, "dseq dseq");

	struct ast_node* dseq_param0 = ast_node_get(dseq, 0);
	assert_ptr(dseq_param0, "ptr desq_param0");
	expect_int_equal(dseq_param0->type, ast_type_declaration, "declaration dseq_param0");

	struct ast_node* dseq_param0_id = ast_node_get(dseq_param0, 0);
	assert_ptr(dseq_param0_id, "ptr dseq_param0_id");
	expect_int_equal(dseq_param0_id->type, ast_type_id, "type dseq_param0_id");
	expect_str(&dseq_param0_id->value, "arg0", "arg0 dseq_param0_id");

	struct ast_node* dseq_param0_type = ast_node_get(dseq_param0, 1);
	assert_ptr(dseq_param0_type, "ptr dseq_param0_type");
	expect_int_equal(dseq_param0_type->type, ast_type_type, "type dseq_param0_type");

	struct ast_node* dseq_param1 = ast_node_get(dseq, 1);
	assert_ptr(dseq_param1, "ptr desq_param1");
	expect_int_equal(dseq_param1->type, ast_type_declaration, "declaration dseq_param1");

	struct ast_node* dseq_param1_id = ast_node_get(dseq_param1, 0);
	assert_ptr(dseq_param1_id, "ptr dseq_param1_id");
	expect_int_equal(dseq_param1_id->type, ast_type_id, "type dseq_param1_id");
	expect_str(&dseq_param1_id->value, "arg1", "arg1 dseq_param1_id");

	struct ast_node* dseq_param1_type_id = ast_node_get(dseq_param1, 1);
	assert_ptr(dseq_param1_type_id, "ptr dseq_param1_type_id");
	expect_int_equal(dseq_param1_type_id->type, ast_type_type, "type dseq_param1_type_id");

	struct ast_node* dseq_param2 = ast_node_get(dseq, 2);
	assert_ptr(dseq_param2, "ptr desq_param2");
	expect_int_equal(dseq_param2->type, ast_type_declaration, "declaration dseq_param2");

	struct ast_node* dseq_param2_id = ast_node_get(dseq_param2, 0);
	assert_ptr(dseq_param2_id, "ptr dseq_param2_id");
	expect_int_equal(dseq_param2_id->type, ast_type_id, "type dseq_param2_id");
	expect_str(&dseq_param2_id->value, "arg2", "arg2 dseq_param2_id");

	struct ast_node* dseq_param2_type_id = ast_node_get(dseq_param2, 1);
	assert_ptr(dseq_param2_type_id, "ptr dseq_param2_type_id");
	expect_int_equal(dseq_param2_type_id->type, ast_type_type, "type dseq_param2_type_id");

	struct ast_node* dret = ast_node_get(fd, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* dret_type_id = ast_node_get(dret, 0);
	assert_ptr(dret_type_id, "ptr dret_type_id");
	expect_int_equal(dret_type_id->type, ast_type_type, "type dret_type_id");

	struct ast_node* call = ast_node_get(cu.root, 3);
	assert_ptr(call, "ptr call");
	expect_int_equal(call->type, ast_type_call, "call call");

	struct ast_node* call_id = ast_node_get(call, 0);
	assert_ptr(call_id, "ptr call_id");
	expect_int_equal(call_id->type, ast_type_id, "id call_id");
	expect_str(&call_id->value, "foo", "foo call_id");

	struct ast_node* cseq = ast_node_get(call, 1);
	assert_ptr(cseq, "ptr cseq");
	expect_int_equal(cseq->type, ast_type_cseq, "cseq");

	struct ast_node* cseq_param0 = ast_node_get(cseq, 0);
	assert_ptr(cseq_param0, "ptr cseq_param0");
	expect_int_equal(cseq_param0->type, ast_type_id, "id cseq_param0");
	expect_str(&cseq_param0->value, "x", "x cseq_param0");

	struct ast_node* cseq_param1 = ast_node_get(cseq, 1);
	assert_ptr(cseq_param1, "ptr cseq_param1");
	expect_int_equal(cseq_param1->type, ast_type_id, "id cseq_param1");
	expect_str(&cseq_param1->value, "y", "y cseq_param1");

	struct ast_node* cseq_param2 = ast_node_get(cseq, 2);
	assert_ptr(cseq_param2, "ptr cseq_param2");
	expect_int_equal(cseq_param2->type, ast_type_number, "cseq_param2");
	expect_str(&cseq_param2->value, "1", "1 cseq_param2");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_call_missing_arguments()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo(a::Int64, b::Int64) end; foo(1)", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "not enough arguments in function call");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_call_too_many_arguments()
{
	test_name(__func__);
	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo(a::Int64) end; foo(1, 2)", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "too many arguments in function call");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_call_type_error()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo(a::Int64) end; foo(true)", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "parameter and aguments types do not match");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_call_anonymous_function_type_error()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var foo::Function{Input{Int64}} = function (a::Int64) end; foo(true)", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "parameter and aguments types do not match");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_call_error_right_paren()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo() end; foo(", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "expected right parenthesis");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_call_error_function_not_declared()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "variable not declared: foo");

	/* destroy ps{} cu.root cu.root{} */
	
	parse_teardown2(&cu);
}

void test_parse_call_error_not_function()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("var foo::Int64; foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "not a function type");

	/* destroy ps{} cu.root cu.root{} */
	parse_teardown2(&cu);
}

void test_parse_call_error_not_enough_arguments()
{
	test_name(__func__);
	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo(x::Int64) end; foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "not enough arguments in function call");

	parse_teardown2(&cu);
}

void test_parse_call_error_too_many_arguments()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo() end; foo(1)", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "too many arguments in function call");

	parse_teardown2(&cu);
}

void test_parse_call_error_expected_expression()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo(x::Int64) end; foo(1,)", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "expected expression after comma");

	/* destroy ps{} cu.root cu.root{} */
	parse_teardown2(&cu);
}

void test_parse_not_error_expected_factor()
{
	test_name(__func__);

	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("!", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "expected factor after !");

	/* destroy ps{} cu.root cu.root{} */
	parse_teardown2(&cu);
}

void test_parse_not_error_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("function foo() end; !foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "! operator used on factor with no value");

	/* destroy ps{} cu.root cu.root{} */
	parse_teardown2(&cu);
}

void test_parse_not_error_not_boolean()
{
	test_name(__func__);
	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
	parse_setup2("!1", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_error(&cu.el, "not operator used on non-boolean");

	/* destroy ps{} cu.root cu.root{} */	
	parse_teardown2(&cu);
}

void test_parse_factor_newline_var()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup2("var\nx::Int64", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    /* destroy ps{} cu.root cu.root{} */
    parse_teardown2(&cu);
}

void test_parse_factor_newline_var_assign()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup2("var\nx::Int64 =\n1", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    /* destroy ps{} cu.root cu.root{} */
    parse_teardown2(&cu);
}

void test_parse_factor_newline_anonymous_function()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup2("function\n(\na::Int64,\nb::Int64,\nc::Int64\n)\n::\nInt64\na+b+c\nend", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    /* destroy ps{} cu.root cu.root{} */
    parse_teardown2(&cu);
}

void test_parse_factor_newline_anonymous_function_var()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup2(
        "var foo::Function{Input{Int64,Int64,Int64},Output{Int64}} = function\n"
        "(\n"
        "a::Int64,\n"
        "b::Int64,\n"
        "c::Int64\n"
        ")\n"
        "::\n"
        "Int64\n"
        "a+b+c\n"
        "end", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    /* destroy ps{} cu.root cu.root{} */
    parse_teardown2(&cu);
}

void test_parse_factor()
{
	test_parse_var();
	test_parse_var2();
	test_parse_var_expected_declaration();
	test_parse_number_integer();
	test_parse_number_float();
	test_parse_string();
	test_parse_boolean_true();
	test_parse_boolean_false();
	test_parse_id();
	test_parse_id2();
	test_parse_id3();
	test_parse_sign_negative();
	test_parse_sign_positive();
	test_parse_sign_error_no_value();
	test_parse_sign_expected_factor();
	test_parse_anonymous_function();
	test_parse_anonymous_function2();
	test_parse_anonymous_function3();
	test_parse_anonymous_function_return_error();
	test_parse_anonymous_function_assignment_error();
	test_parse_anonymous_function_expected_right_paren();
	test_parse_not_id();
	test_parse_not_literal();
	test_parse_not_error();
	test_parse_array_literal_integer();
	test_parse_array_literal_float();
	test_parse_array_literal_numeric();
	test_parse_array_literal_mixed_error();
	test_parse_array_literal_empty_error();
	test_parse_array_literal_error_right_square_bracket();
	test_parse_array_literal_error_expected_expr();
	test_parse_paren_num();
	test_parse_paren_error_empty();
	test_parse_paren_error_right_parenthesis();
	test_parse_paren_error_no_value();
	test_parse_call();
	test_parse_call_return_type();
	test_parse_call_return_type_error();
	test_parse_call2();
	test_parse_call3();
	test_parse_call4();
	test_parse_call_missing_arguments();
	test_parse_call_too_many_arguments();
	test_parse_call_type_error();
	test_parse_call_anonymous_function_type_error();
	test_parse_anonymous_function_expected_end();
	test_parse_call_error_right_paren();
	test_parse_call_error_function_not_declared();
	test_parse_call_error_not_function();
	test_parse_call_error_not_enough_arguments();
	test_parse_call_error_too_many_arguments();
	test_parse_call_error_expected_expression();
	test_parse_not_error_expected_factor();
	test_parse_not_error_no_value();
	test_parse_not_error_not_boolean();
    test_parse_factor_newline_var();
    test_parse_factor_newline_var_assign();
    test_parse_factor_newline_anonymous_function();
    test_parse_factor_newline_anonymous_function_var();
}
