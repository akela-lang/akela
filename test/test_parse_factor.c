#include <stdbool.h>
#include "zinc/unit_test.h"
#include "alba/parse_tools.h"
#include "assert_compiler.h"
#include "alba/dag.h"
#include "test_parse.h"

/* dynamic-output-none */
void test_parse_number_integer()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("32", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* num = dag_get_child(root, 0);
	assert_ptr(num, "ptr num");
	expect_int_equal(num->type, dag_type_number, "number num");
	expect_str(&num->value, "32", "32 num");

	struct dag_node* etype = num->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, dag_type_type_name, "type_name etype");
	expect_str(&etype->value, "Int64", "Int64 etype");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_number_float()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("5.0e0", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* number = dag_get_child(root, 0);
	assert_ptr(number, "ptr num");
	expect_int_equal(number->type, dag_type_number, "number number");
	expect_str(&number->value, "5.0e0", "5.0e0 number");

	struct dag_node* etype = number->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, dag_type_type_name, "type_name etype");
	expect_str(&etype->value, "Float64", "Float64 etype");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_string()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("\"hello\"", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* string = dag_get_child(root, 0);
	assert_ptr(string, "ptr string");
	expect_int_equal(string->type, dag_type_string, "string string");
	expect_str(&string->value, "hello", "hello string");

	struct dag_node* etype = string->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, dag_type_string, "string etype");
	expect_str(&etype->value, "String", "String etype");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

void test_parse_boolean_true()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("true", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* true_node = dag_get_child(root, 0);
	assert_ptr(true_node, "ptr true_node");
	expect_int_equal(true_node->type, dag_type_boolean, "boolean true_node");
	expect_str(&true_node->value, "true", "true true_node");

	struct dag_node* etype = true_node->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, dag_type_boolean, "boolean etype");
	expect_str(&etype->value, "Bool", "Bool etype");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

void test_parse_boolean_false()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("false", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* false_node = dag_get_child(root, 0);
	assert_ptr(false_node, "ptr false_node");
	expect_int_equal(false_node->type, dag_type_boolean, "boolean false_node");
	expect_str(&false_node->value, "false", "false false_node");

	struct dag_node* etype = false_node->etype;
	assert_ptr(etype, "ptr etype");
	expect_int_equal(etype->type, dag_type_boolean, "boolean etype");
	expect_str(&etype->value, "Bool", "Bool etype");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_id()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var x::Int64; x", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* var = dag_get_child(root, 0);
	assert_ptr(var, "ptr var");
	assert_int_equal(var->type, dag_type_var, "let let");

	struct dag_node* var_dec = dag_get_child(var, 0);
	assert_ptr(var_dec, "ptr var_dec");
	assert_int_equal(var_dec->type, dag_type_declaration, "declaration var_dec");

	struct dag_node* var_id = dag_get_child(var_dec, 0);
	assert_ptr(var_id, "ptr var_id");
	expect_int_equal(var_id->type, dag_type_id, "id var_id");
	expect_str(&var_id->value, "x", "x");

	struct dag_node* var_type = dag_get_child(var_dec, 1);
	assert_ptr(var_type, "ptr var_type");
	expect_int_equal(var_type->type, dag_type_id, "id var_type");
	expect_str(&var_type->value, "Int64", "Int64");

	struct dag_node* id = dag_get_child(root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, dag_type_id, "id id");
	expect_str(&id->value, "x", "x id");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_id2()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var _a23::Int64; _a23", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* id = dag_get_child(root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, dag_type_id, "id id");
	expect_str(&id->value, "_a23", "_a23 id");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_id3()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a2::Int64; a2", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* id = dag_get_child(root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, dag_type_id, "id id");
	expect_str(&id->value, "a2", "a2 id");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_num_negative()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("-30", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* sign = dag_get_child(root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, dag_type_sign, "sign sign");

	struct dag_node* left = dag_get_child(sign, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, dag_type_minus, "minus");

	struct dag_node* right = dag_get_child(sign, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "30", "30");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_num_positive()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("+30", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* sign = dag_get_child(root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, dag_type_sign, "sign sign");

	struct dag_node* left = dag_get_child(sign, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, dag_type_plus, "plus");

	struct dag_node* right = dag_get_child(sign, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "30", "30");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_call_negative()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("function foo() end\n-foo()", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* fd = dag_get_child(root, 0);
	assert_ptr(fd, "ptr fd");
	expect_int_equal(fd->type, dag_type_function, "function");

	struct dag_node* f_id = dag_get_child(fd, 0);
	assert_ptr(f_id, "ptr f_id");
	expect_int_equal(f_id->type, dag_type_id, "id f_id");
	expect_str(&f_id->value, "foo", "foo");

	struct dag_node* sign = dag_get_child(root, 1);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, dag_type_sign, "sign sign");

	struct dag_node* left = dag_get_child(sign, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, dag_type_minus, "minus");

	struct dag_node* right = dag_get_child(sign, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, dag_type_call, "call");

	struct dag_node* id = dag_get_child(right, 0);
	assert_ptr(id, "ptr id");
	assert_int_equal(id->type, dag_type_id, "id id");
	expect_str(&id->value, "foo", "foo id");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_anonymous_function()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Function; a = function(x::Int32,y::Int32,z::Int32) 1 end", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* assign = dag_get_child(root, 1);
	assert_ptr(assign, "ptr assign");
	expect_int_equal(assign->type, dag_type_assign, "assign assign");

	struct dag_node* a = dag_get_child(assign, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	struct dag_node* f = dag_get_child(assign, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_anonymous_function, "anonymous-function f");

	struct dag_node* dseq = dag_get_child(f, 0);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, dag_type_dseq, "dseq dseq");

	struct dag_node* dec_x = dag_get_child(dseq, 0);
	assert_ptr(dec_x, "ptr dec_x");
	expect_int_equal(dec_x->type, dag_type_declaration, "declaration dec_x");

	struct dag_node* x = dag_get_child(dec_x, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, dag_type_id, "id x");
	expect_str(&x->value, "x", "x x");

	struct dag_node* name_x = dag_get_child(dec_x, 1);
	assert_ptr(name_x, "ptr name_x");
	expect_int_equal(name_x->type, dag_type_id, "id name_x");
	expect_str(&name_x->value, "Int32", "Int32 name_x");

	struct dag_node* dec_y = dag_get_child(dseq, 1);
	assert_ptr(dec_y, "ptr dec_y");
	expect_int_equal(dec_y->type, dag_type_declaration, "declaration dec_y");

	struct dag_node* y = dag_get_child(dec_y, 0);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, dag_type_id, "id y");
	expect_str(&y->value, "y", "y y");

	struct dag_node* name_y = dag_get_child(dec_y, 1);
	assert_ptr(name_y, "ptr name_y");
	expect_int_equal(name_y->type, dag_type_id, "id name_y");
	expect_str(&name_y->value, "Int32", "Int32 name_y");

	struct dag_node* dec_z = dag_get_child(dseq, 2);
	assert_ptr(dec_z, "ptr dec_z");
	expect_int_equal(dec_z->type, dag_type_declaration, "declaration dec_z");

	struct dag_node* z = dag_get_child(dec_z, 0);
	assert_ptr(z, "ptr z");
	expect_int_equal(z->type, dag_type_id, "id z");
	expect_str(&z->value, "z", "z z");

	struct dag_node* name_z = dag_get_child(dec_z, 1);
	assert_ptr(name_z, "ptr name_z");
	expect_int_equal(name_z->type, dag_type_id, "id name_z");
	expect_str(&name_z->value, "Int32", "Int32 name_z");

	struct dag_node* dret = dag_get_child(f, 1);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, dag_type_dret, "dret dret");

	struct dag_node* stmts = dag_get_child(f, 2);
	assert_ptr(stmts, "ptr stmts");
	expect_int_equal(stmts->type, dag_type_stmts, "stmts stmts");

	struct dag_node* one = dag_get_child(stmts, 0);
	assert_ptr(one, "ptr one");
	expect_int_equal(one->type, dag_type_number, "number one");
	expect_str(&one->value, "1", "1 one");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_anonymous_function2()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Function; a = function(x::Int32,y::Int32,z::Int32)::Int32 1 end", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* assign = dag_get_child(root, 1);
	assert_ptr(assign, "ptr assign");
	expect_int_equal(assign->type, dag_type_assign, "assign assign");

	struct dag_node* a = dag_get_child(assign, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	struct dag_node* f = dag_get_child(assign, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_anonymous_function, "anonymous-function f");

	struct dag_node* dseq = dag_get_child(f, 0);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, dag_type_dseq, "dseq dseq");

	struct dag_node* dec_x = dag_get_child(dseq, 0);
	assert_ptr(dec_x, "ptr dec_x");
	expect_int_equal(dec_x->type, dag_type_declaration, "declaration dec_x");

	struct dag_node* x = dag_get_child(dec_x, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, dag_type_id, "id x");
	expect_str(&x->value, "x", "x x");

	struct dag_node* name_x = dag_get_child(dec_x, 1);
	assert_ptr(name_x, "ptr name_x");
	expect_int_equal(name_x->type, dag_type_id, "id name_x");
	expect_str(&name_x->value, "Int32", "Int32 name_x");

	struct dag_node* dec_y = dag_get_child(dseq, 1);
	assert_ptr(dec_y, "ptr dec_y");
	expect_int_equal(dec_y->type, dag_type_declaration, "declaration dec_y");

	struct dag_node* y = dag_get_child(dec_y, 0);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, dag_type_id, "id y");
	expect_str(&y->value, "y", "y y");

	struct dag_node* name_y = dag_get_child(dec_y, 1);
	assert_ptr(name_y, "ptr name_y");
	expect_int_equal(name_y->type, dag_type_id, "id name_y");
	expect_str(&name_y->value, "Int32", "Int32 name_y");

	struct dag_node* dec_z = dag_get_child(dseq, 2);
	assert_ptr(dec_z, "ptr dec_z");
	expect_int_equal(dec_z->type, dag_type_declaration, "declaration dec_z");

	struct dag_node* z = dag_get_child(dec_z, 0);
	assert_ptr(z, "ptr z");
	expect_int_equal(z->type, dag_type_id, "id z");
	expect_str(&z->value, "z", "z z");

	struct dag_node* name_z = dag_get_child(dec_z, 1);
	assert_ptr(name_z, "ptr name_z");
	expect_int_equal(name_z->type, dag_type_id, "id name_z");
	expect_str(&name_z->value, "Int32", "Int32 name_z");

	struct dag_node* dret = dag_get_child(f, 1);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, dag_type_dret, "dret dret");

	struct dag_node* dret_type_id = dag_get_child(dret, 0);
	assert_ptr(dret_type_id, "ptr dret_type_id");
	expect_int_equal(dret_type_id->type, dag_type_id, "id dret_type_id");
	expect_str(&dret_type_id->value, "Int32", "In32 dret_type_id");

	struct dag_node* stmts = dag_get_child(f, 2);
	assert_ptr(stmts, "ptr stmts");
	expect_int_equal(stmts->type, dag_type_stmts, "stmts stmts");

	struct dag_node* one = dag_get_child(stmts, 0);
	assert_ptr(one, "ptr one");
	expect_int_equal(one->type, dag_type_number, "number one");
	expect_str(&one->value, "1", "1 one");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

void test_parse_anonymous_function3()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Function; a = function(x::Int64) var x::Int64 = 1 end", &ps, &root);
	assert_has_errors(ps.el);
	assert_false(valid, "parse valid");
	assert_compile_error(ps.el, "duplicate declaration in same scope: x");

	/* destroy ps{} root root{} */
	dag_destroy(root);
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
	test_parse_num_negative();
	test_parse_num_positive();
	test_parse_call_negative();
	test_parse_anonymous_function();
	test_parse_anonymous_function2();
	test_parse_anonymous_function3();
}
