#include "zinc/unit_test.h"
#include "test_parse.h"
#include "alba/unit_test_compiler.h"
#include "alba/type_def.h"

void test_parse_types_missing_declaration()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("x + 1", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "variable not declared: x");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_missing_declaration2()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("foo() + 1", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "variable not declared: foo");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_missing_declaration3()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("x = function() end", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "variable not declared: x");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_double_function()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("function foo() end; function foo() end", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "duplicate declaration in same scope: foo");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_reserved_type()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("var Int64::Int64", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "identifier reserved as a type: Int64");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_reserved_type2()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("function Int64() end", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "identifier reserved as a type: Int64");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_reserved_type3()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("for Int64::Int64 = 1:10 end", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "identifier reserved as a type: Int64");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_reserved_type4()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("var list::Vector{Int64}; for Int64::Int64 in list end", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "identifier reserved as a type: Int64");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_exists()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("var x::SuperInt; x + 1", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "expected a type");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_types_array()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Vector{Int64}; a[1]", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* var = ast_node_get(root, 0);
	assert_ptr(var, "ptr var");
	expect_int_equal(var->type, ast_type_var, "var dec");

	struct ast_node* dec = ast_node_get(var, 0);
	assert_ptr(dec, "ptr dec");
	expect_int_equal(dec->type, ast_type_declaration, "declaration dec");

	struct ast_node* dec_id = ast_node_get(dec, 0);
	assert_ptr(dec_id, "ptr dec_id");
	expect_int_equal(dec_id->type, ast_type_id, "id dec_id");
	expect_str(&dec_id->value, "a", "a dec_id");

	struct ast_node* array_node = ast_node_get(dec, 1);
	assert_ptr(array_node, "ptr array_node");
	expect_int_equal(array_node->type, ast_type_type, "type array_node");

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
void test_parse_types_array_error()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Int64{Int64}; a[1]", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "subtype was specified for non-generic type: Int64");
	expect_false(valid, "valid");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_types_array_error2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Vector{Int64, Float64}; a[1]", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "generic type (Vector) should have 1 subtype but has 2 subtypes");
	expect_false(valid, "valid");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_error_dseq_comma()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("function foo(a::Int64,) end", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "expected declaration after comma");
	expect_false(valid, "valid");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_error_declaration_double_colon()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "expected double colon");
	expect_false(valid, "valid");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_error_declaration_type()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "expected a type");
	expect_false(valid, "valid");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_error_type_right_curly_brace()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Vector{Int64", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "expected right curly brace");
	expect_false(valid, "valid");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_error_type_not_defined()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Foo", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "type not defined: Foo");
	expect_false(valid, "valid");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_error_not_a_type()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var foo::Int64; var a::foo", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "identifier is not a type: foo");
	expect_false(valid, "valid");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_error_not_generic()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Int64{Int64}", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "subtype was specified for non-generic type: Int64");
	expect_false(valid, "valid");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_error_subtype_count()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("var a::Vector{Int64, Int64}", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "generic type (Vector) should have 1 subtype but has 2 subtypes");
	expect_false(valid, "valid");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_error_duplicate_declarations()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("var x::Int64; var x::Int64", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "duplicate declaration in same scope: x");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_error_type_name()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("var x::Vector{}", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "expected a type name");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_error_comma_type_name()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("var x::Vector{Int64,}", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "expected a type name after comma");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_error_return_type()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("function foo()::Int64 true end", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "returned type does not match function return type");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_error_param()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("function foo(a::Int64) true end; foo(true)", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "parameter and aguments types do not match");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_error_param_no_value()
{
	test_name(__func__);

	struct parse_state ps;
	struct ast_node* root;

	bool valid = parse_setup("function foo(a::Int64) true end; foo(foo(1))", &ps, &root);
	expect_has_errors(ps.el);
	expect_compile_error(ps.el, "argument expression has no value");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types()
{
	test_parse_types_missing_declaration();
	test_parse_types_missing_declaration2();
	test_parse_types_missing_declaration3();
	test_parse_types_double_function();
	test_parse_types_reserved_type();
	test_parse_types_reserved_type2();
	test_parse_types_reserved_type3();
	test_parse_types_reserved_type4();
	test_parse_types_exists();
	test_parse_types_array();
	test_parse_types_array_error();
	test_parse_types_array_error2();
	test_parse_error_dseq_comma();
	test_parse_error_declaration_double_colon();
	test_parse_error_declaration_type();
	test_parse_error_type_right_curly_brace();
	test_parse_error_type_not_defined();
	test_parse_error_not_a_type();
	test_parse_error_not_generic();
	test_parse_error_subtype_count();
	test_parse_error_duplicate_declarations();
	test_parse_error_type_name();
	test_parse_error_comma_type_name();
	test_parse_error_return_type();
	test_parse_types_error_param();
	test_parse_types_error_param_no_value();
}
