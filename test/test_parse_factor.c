#include <stdbool.h>
#include "zinc/unit_test.h"
#include "alba/parse_tools.h"
#include "unit_test_compiler.h"
#include "alba/ast.h"
#include "test_parse.h"

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

	struct ast_node* num = ast_node_get(root, 0);
	assert_ptr(num, "ptr num");
	expect_int_equal(num->type, ast_type_number, "number num");
	expect_str(&num->value, "32", "32 num");

	struct ast_node* etype = num->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_type_name, "type_name etype");
	expect_str(&etype->value, "Int64", "Int64 etype");

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

	struct ast_node* etype = number->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_type_name, "type_name etype");
	expect_str(&etype->value, "Float64", "Float64 etype");

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

	struct ast_node* etype = string->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_type_name, "string etype");
	expect_str(&etype->value, "String", "String etype");

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

	struct ast_node* etype = true_node->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_type_name, "boolean etype");
	expect_str(&etype->value, "Bool", "Bool etype");

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

	struct ast_node* etype = false_node->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_type_name, "boolean etype");
	expect_str(&etype->value, "Bool", "Bool etype");

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
	expect_int_equal(var_type->type, ast_type_type_name, "type_name var_type");
	expect_str(&var_type->value, "Int64", "Int64");

	struct ast_node* id = ast_node_get(root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "x", "x id");

	struct ast_node* etype = id->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_type_name, "type_name etype");
	expect_str(&etype->value, "Int64", "Int64 etype");

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

	struct ast_node* left = ast_node_get(sign, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_minus, "minus");

	struct ast_node* right = ast_node_get(sign, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "30", "30");

	struct ast_node* etype = sign->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_type_name, "type_name sign");
	expect_str(&etype->value, "Int64", "Int64 sign");

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

	struct ast_node* left = ast_node_get(sign, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, ast_type_plus, "plus");

	struct ast_node* right = ast_node_get(sign, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "30", "30");

	struct ast_node* etype = sign->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_type_name, "type_name sign");
	expect_str(&etype->value, "Int64", "Int64 sign");

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
	assert_has_errors(ps.el);
	expect_false(valid, "parse_setup valid");
	assert_compile_error(ps.el, "cannot perform operation on expression with no value");

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

	struct ast_node* name_x = ast_node_get(dec_x, 1);
	assert_ptr(name_x, "ptr name_x");
	expect_int_equal(name_x->type, ast_type_type_name, "type_name name_x");
	expect_str(&name_x->value, "Int32", "Int32 name_x");

	struct ast_node* dec_y = ast_node_get(dseq, 1);
	assert_ptr(dec_y, "ptr dec_y");
	expect_int_equal(dec_y->type, ast_type_declaration, "declaration dec_y");

	struct ast_node* y = ast_node_get(dec_y, 0);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y y");

	struct ast_node* name_y = ast_node_get(dec_y, 1);
	assert_ptr(name_y, "ptr name_y");
	expect_int_equal(name_y->type, ast_type_type_name, "type_name name_y");
	expect_str(&name_y->value, "Int32", "Int32 name_y");

	struct ast_node* dec_z = ast_node_get(dseq, 2);
	assert_ptr(dec_z, "ptr dec_z");
	expect_int_equal(dec_z->type, ast_type_declaration, "declaration dec_z");

	struct ast_node* z = ast_node_get(dec_z, 0);
	assert_ptr(z, "ptr z");
	expect_int_equal(z->type, ast_type_id, "id z");
	expect_str(&z->value, "z", "z z");

	struct ast_node* name_z = ast_node_get(dec_z, 1);
	assert_ptr(name_z, "ptr name_z");
	expect_int_equal(name_z->type, ast_type_type_name, "type_name name_z");
	expect_str(&name_z->value, "Int32", "Int32 name_z");

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

	struct ast_node* etype = f->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_type_function, "function_declaration etype");

	struct ast_node* type_dseq = ast_node_get(etype, 0);
	assert_ptr(type_dseq, "ptr type_desq");
	expect_int_equal(type_dseq->type, ast_type_type_dseq, "type_dseq type_dseq");

	struct ast_node* param0 = ast_node_get(type_dseq, 0);
	assert_ptr(param0, "ptr param0");
	expect_int_equal(param0->type, ast_type_type_name, "type_name param0");
	expect_str(&param0->value, "Int32", "Int32 param0");

	struct ast_node* param1 = ast_node_get(type_dseq, 1);
	assert_ptr(param1, "ptr param1");
	expect_int_equal(param1->type, ast_type_type_name, "type_name param1");
	expect_str(&param1->value, "Int32", "Int32 param1");

	struct ast_node* param2 = ast_node_get(type_dseq, 2);
	assert_ptr(param2, "ptr param2");
	expect_int_equal(param2->type, ast_type_type_name, "type_name param2");
	expect_str(&param2->value, "Int32", "Int32 param2");

	struct ast_node* type_dret = ast_node_get(etype, 1);
	assert_ptr(type_dret, "ptr type_dret");
	expect_int_equal(type_dret->type, ast_type_type_dret, "type_dret type_dret");

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

	struct ast_node* name_x = ast_node_get(dec_x, 1);
	assert_ptr(name_x, "ptr name_x");
	expect_int_equal(name_x->type, ast_type_type_name, "type_name name_x");
	expect_str(&name_x->value, "Int32", "Int32 name_x");

	struct ast_node* dec_y = ast_node_get(dseq, 1);
	assert_ptr(dec_y, "ptr dec_y");
	expect_int_equal(dec_y->type, ast_type_declaration, "declaration dec_y");

	struct ast_node* y = ast_node_get(dec_y, 0);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y y");

	struct ast_node* name_y = ast_node_get(dec_y, 1);
	assert_ptr(name_y, "ptr name_y");
	expect_int_equal(name_y->type, ast_type_type_name, "type_name name_y");
	expect_str(&name_y->value, "Int32", "Int32 name_y");

	struct ast_node* dec_z = ast_node_get(dseq, 2);
	assert_ptr(dec_z, "ptr dec_z");
	expect_int_equal(dec_z->type, ast_type_declaration, "declaration dec_z");

	struct ast_node* z = ast_node_get(dec_z, 0);
	assert_ptr(z, "ptr z");
	expect_int_equal(z->type, ast_type_id, "id z");
	expect_str(&z->value, "z", "z z");

	struct ast_node* name_z = ast_node_get(dec_z, 1);
	assert_ptr(name_z, "ptr name_z");
	expect_int_equal(name_z->type, ast_type_type_name, "type_name name_z");
	expect_str(&name_z->value, "Int32", "Int32 name_z");

	struct ast_node* dret = ast_node_get(f, 1);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* dret_type_id = ast_node_get(dret, 0);
	assert_ptr(dret_type_id, "ptr dret_type_id");
	expect_int_equal(dret_type_id->type, ast_type_type_name, "type_name dret_type_id");
	expect_str(&dret_type_id->value, "Int32", "In32 dret_type_id");

	struct ast_node* stmts = ast_node_get(f, 2);
	assert_ptr(stmts, "ptr stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "stmts stmts");

	struct ast_node* one = ast_node_get(stmts, 0);
	assert_ptr(one, "ptr one");
	expect_int_equal(one->type, ast_type_number, "number one");
	expect_str(&one->value, "1", "1 one");

	struct ast_node* etype = f->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_type_function, "function_declaration etype");

	struct ast_node* type_dseq = ast_node_get(etype, 0);
	assert_ptr(type_dseq, "ptr type_desq");
	expect_int_equal(type_dseq->type, ast_type_type_dseq, "type_dseq type_dseq");

	struct ast_node* param0 = ast_node_get(type_dseq, 0);
	assert_ptr(param0, "ptr param0");
	expect_int_equal(param0->type, ast_type_type_name, "type_name param0");
	expect_str(&param0->value, "Int32", "Int32 param0");

	struct ast_node* param1 = ast_node_get(type_dseq, 1);
	assert_ptr(param1, "ptr param1");
	expect_int_equal(param1->type, ast_type_type_name, "type_name param1");
	expect_str(&param1->value, "Int32", "Int32 param1");

	struct ast_node* param2 = ast_node_get(type_dseq, 2);
	assert_ptr(param2, "ptr param2");
	expect_int_equal(param2->type, ast_type_type_name, "type_name param2");
	expect_str(&param2->value, "Int32", "Int32 param2");

	struct ast_node* type_dret = ast_node_get(etype, 1);
	assert_ptr(type_dret, "ptr type_dret");
	expect_int_equal(type_dret->type, ast_type_type_dret, "type_dret type_dret");

	struct ast_node* type_return = ast_node_get(type_dret, 0);
	assert_ptr(type_return, "ptr type_return");
	expect_int_equal(type_return->type, ast_type_type_name, "type_name type_return");
	expect_str(&type_return->value, "Int32", "Int32 type_return");

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
	assert_has_errors(ps.el);
	assert_false(valid, "parse valid");
	assert_compile_error(ps.el, "duplicate declaration in same scope: x");

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

	struct ast_node* id = ast_node_get(not, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "a", "a id");

	struct ast_node* etype = not->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_type_name, "type_name etype");
	expect_str(&etype->value, "Bool", "Bool etype");

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

	struct ast_node* lit_bool = ast_node_get(not, 0);
	assert_ptr(lit_bool, "ptr lit_bool");
	expect_int_equal(lit_bool->type, ast_type_boolean, "boolean true");
	expect_str(&lit_bool->value, "true", "true lit_bool");

	struct ast_node* etype = not->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_type_name, "type_name etype");
	expect_str(&etype->value, "Bool", "Bool etype");

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
	assert_has_errors(ps.el);
	expect_false(valid, "parse_setup valid");
	assert_compile_error(ps.el, "cannot perform operation on expression with no value");

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

	struct ast_node* etype = a->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_array, "array etype");

	struct ast_node* type_vector = ast_node_get(etype, 0);
	assert_ptr(type_vector, "ptr type_vector");
	expect_int_equal(type_vector->type, ast_type_array_type_name, "array_type_name type_vector");
	expect_str(&type_vector->value, "Vector", "Vector type_vector");

	struct ast_node* type_element = ast_node_get(etype, 1);
	assert_ptr(type_element, "ptr type_element");
	expect_int_equal(type_element->type, ast_type_type_name, "type_name type_element");
	expect_str(&type_element->value, "Int64", "Int64 type_element");

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

	struct ast_node* etype = a->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_array, "array etype");

	struct ast_node* type_vector = ast_node_get(etype, 0);
	assert_ptr(type_vector, "ptr type_vector");
	expect_int_equal(type_vector->type, ast_type_array_type_name, "array_type_name type_vector");
	expect_str(&type_vector->value, "Vector", "Vector type_vector");

	struct ast_node* type_element = ast_node_get(etype, 1);
	assert_ptr(type_element, "ptr type_element");
	expect_int_equal(type_element->type, ast_type_type_name, "type_name type_element");
	expect_str(&type_element->value, "Float64", "Float64 type_element");

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
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* a = ast_node_get(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_array_literal, "array-literal a");

	struct ast_node* etype = a->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, ast_type_array, "array etype");

	struct ast_node* type_vector = ast_node_get(etype, 0);
	assert_ptr(type_vector, "ptr type_vector");
	expect_int_equal(type_vector->type, ast_type_array_type_name, "array_type_name type_vector");
	expect_str(&type_vector->value, "Vector", "Vector type_vector");

	struct ast_node* type_element = ast_node_get(etype, 1);
	assert_ptr(type_element, "ptr type_element");
	expect_int_equal(type_element->type, ast_type_type_name, "type_name type_element");
	expect_str(&type_element->value, "Float64", "Float64 type_element");

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
	assert_has_errors(ps.el);
	expect_false(valid, "parse_setup valid");
	assert_compile_error(ps.el, "array elements not one type");

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
	test_parse_paren_num();
}
