#include <stdbool.h>
#include "zinc/unit_test.h"
#include "alba/parse_tools.h"
#include "alba/unit_test_compiler.h"
#include "alba/ast.h"
#include "test_parse.h"
#include "alba/type_use.h"

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

	struct ast_node* type_use = ast_node_get(dec, 1);
	assert_ptr(type_use, "ptr type_use");
	expect_int_equal(type_use->type, ast_type_type, "type type");

	struct type_use* tu = type_use->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_str(&td->name, "Int32", "Int32 td");

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

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* assign = ast_node_get(root, 0);
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

	struct ast_node* type_use = ast_node_get(dec, 1);
	assert_ptr(type_use, "ptr type_use");
	expect_int_equal(type_use->type, ast_type_type, "type type_use");

	struct type_use* tu = type_use->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_str(&td->name, "Int32", "Int32 td");

	struct ast_node* value = ast_node_get(assign, 1);
	assert_ptr(value, "ptr value");
	expect_int_equal(value->type, ast_type_number, "number");
	expect_str(&value->value, "1", "1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_number_integer()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("32", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* number = ast_node_get(root, 0);
	assert_ptr(number, "ptr number");
	expect_int_equal(number->type, ast_type_number, "number num");
	expect_str(&number->value, "32", "32 num");

	struct type_use* tu = number->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "Int64", "Int64 td");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_number_float()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("5.0e0", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* number = ast_node_get(root, 0);
	assert_ptr(number, "ptr num");
	expect_int_equal(number->type, ast_type_number, "number number");
	expect_str(&number->value, "5.0e0", "5.0e0 number");

	struct type_use* tu = number->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_float, "integer td");
	expect_str(&td->name, "Float64", "Float64 td");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_string()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("\"hello\"", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* string = ast_node_get(root, 0);
	assert_ptr(string, "ptr string");
	expect_int_equal(string->type, ast_type_string, "string string");
	expect_str(&string->value, "hello", "hello string");

	struct type_use* tu = string->tu;
	assert_ptr(tu, "ptr tu");
	
	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_string, "string td");
	expect_str(&td->name, "String", "String td");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_boolean_true()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("true", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* true_node = ast_node_get(root, 0);
	assert_ptr(true_node, "ptr true_node");
	expect_int_equal(true_node->type, ast_type_boolean, "boolean true_node");
	expect_str(&true_node->value, "true", "true true_node");

	struct type_use* tu = true_node->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "Bool", "Bool td");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_boolean_false()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("false", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* false_node = ast_node_get(root, 0);
	assert_ptr(false_node, "ptr false_node");
	expect_int_equal(false_node->type, ast_type_boolean, "boolean false_node");
	expect_str(&false_node->value, "false", "false false_node");

	struct type_use* tu = false_node->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "Bool", "Bool td");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_id()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var x::Int64; x", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* var = ast_node_get(root, 0);
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

	struct ast_node* id = ast_node_get(root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "x", "x id");

	struct type_use* tu = id->tu;
	assert_ptr(tu, "ptr tu");
	
	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "Int64", "Int64 td");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_id2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var _a23::Int64; _a23", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* id = ast_node_get(root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "_a23", "_a23 id");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_id3()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a2::Int64; a2", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* id = ast_node_get(root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "a2", "a2 id");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_sign_negative()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("-30", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* sign = ast_node_get(root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, ast_type_sign, "sign sign");

	struct type_use* tu = sign->tu;
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

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_sign_positive()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("+30", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* sign = ast_node_get(root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, ast_type_sign, "sign sign");

	struct type_use* tu = sign->tu;
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

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_sign_error()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("function foo() end\n-foo()", &ps, &root);
	expect_has_errors(ps.el);
	expect_false(valid, "parse_setup valid");
	expect_compile_error(ps.el, "negative operator was used on expression with no value");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_not_id()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var a::Bool; !a", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* not = ast_node_get(root, 1);
	assert_ptr(not, "ptr not");
	expect_int_equal(not->type, ast_type_not, "not not");

	struct type_use* tu = not->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "Bool", "Bool td");

	struct ast_node* id = ast_node_get(not, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "a", "a id");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_not_literal()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("!true", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* not = ast_node_get(root, 0);
	assert_ptr(not, "ptr not");
	expect_int_equal(not->type, ast_type_not, "not not");

	struct type_use* tu = not->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "Bool", "Bool td");

	struct ast_node* lit_bool = ast_node_get(not, 0);
	assert_ptr(lit_bool, "ptr lit_bool");
	expect_int_equal(lit_bool->type, ast_type_boolean, "boolean true");
	expect_str(&lit_bool->value, "true", "true lit_bool");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_not_error()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("function foo() end; !foo()", &ps, &root);
	expect_has_errors(ps.el);
	expect_false(valid, "parse_setup valid");
	expect_compile_error(ps.el, "! operator used on factor with no value");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_array_literal_integer()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("[1,2,3]", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* a = ast_node_get(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_array_literal, "array-literal a");

	struct type_use* array_tu = a->tu;
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

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_array_literal_float()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("[1.0,2.5,3.2]", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* a = ast_node_get(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_array_literal, "array-literal a");

	struct type_use* array_tu = a->tu;
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

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_array_literal_numeric()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("[1, 2.5, 3]", &ps, &root);
	expect_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* a = ast_node_get(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_array_literal, "array-literal a");

	struct type_use* array_tu = a->tu;
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

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_array_literal_mixed_error()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("[1,true,3]", &ps, &root);
	expect_has_errors(ps.el);
	expect_false(valid, "parse_setup valid");
	expect_compile_error(ps.el, "array elements not the same type");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_array_literal_empty_error()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("[]", &ps, &root);
	expect_has_errors(ps.el);
	expect_false(valid, "parse_setup valid");
	expect_compile_error(ps.el, "array literal has no elements");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_anonymous_function()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Function; a = function(x::Int32,y::Int32,z::Int32) 1 end", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* assign = ast_node_get(root, 1);
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

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_anonymous_function2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Function; a = function(x::Int32,y::Int32,z::Int32)::Int32 1 end", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* assign = ast_node_get(root, 1);
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

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_anonymous_function3()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Function; a = function(x::Int64) var x::Int64 = 1 end", &ps, &root);
	expect_has_errors(ps.el);
	expect_false(valid, "parse valid");
	expect_compile_error(ps.el, "duplicate declaration in same scope: x");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_paren_num()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("(32)", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* paren = ast_node_get(root, 0);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, ast_type_parenthesis, "parenthesis paren");

	struct ast_node* number = ast_node_get(paren, 0);
	expect_int_equal(number->type, ast_type_number, "number number");
	expect_str(&number->value, "32", "32 number");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_factor()
{
	test_parse_var();
	test_parse_var2();
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
	test_parse_sign_error();
	test_parse_anonymous_function();
	test_parse_anonymous_function2();
	test_parse_anonymous_function3();
	test_parse_not_id();
	test_parse_not_literal();
	test_parse_not_error();
	test_parse_array_literal_integer();
	test_parse_array_literal_float();
	test_parse_array_literal_numeric();
	test_parse_array_literal_mixed_error();
	test_parse_array_literal_empty_error();
	test_parse_paren_num();
}
