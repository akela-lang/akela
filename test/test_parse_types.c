#include "zinc/unit_test.h"
#include "test_parse.h"
#include "assert_compiler.h"

void test_parse_types_missing_declaration()
{
	test_name(__func__);

	struct parse_state ps;
	struct dag_node* root;

	bool valid = parse_setup("x + 1", &ps, &root);
	assert_has_errors(ps.el);
	assert_compile_error(ps.el, "identifier not declared: x");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_missing_declaration2()
{
	test_name(__func__);

	struct parse_state ps;
	struct dag_node* root;

	bool valid = parse_setup("foo() + 1", &ps, &root);
	assert_has_errors(ps.el);
	assert_compile_error(ps.el, "function is not declared: foo");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_missing_declaration3()
{
	test_name(__func__);

	struct parse_state ps;
	struct dag_node* root;

	bool valid = parse_setup("x = function() end", &ps, &root);
	assert_has_errors(ps.el);
	assert_compile_error(ps.el, "identifier not declared: x");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_double()
{
	test_name(__func__);

	struct parse_state ps;
	struct dag_node* root;

	bool valid = parse_setup("var x::Int64; var x::Int64", &ps, &root);
	assert_has_errors(ps.el);
	assert_compile_error(ps.el, "duplicate declaration in same scope: x");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_double_function()
{
	test_name(__func__);

	struct parse_state ps;
	struct dag_node* root;

	bool valid = parse_setup("function foo() end; function foo() end", &ps, &root);
	assert_has_errors(ps.el);
	assert_compile_error(ps.el, "duplicate declaration in same scope: foo");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_reserved_type()
{
	test_name(__func__);

	struct parse_state ps;
	struct dag_node* root;

	bool valid = parse_setup("var Int64::Int64", &ps, &root);
	assert_has_errors(ps.el);
	assert_compile_error(ps.el, "expected declaration after var");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_reserved_type2()
{
	test_name(__func__);

	struct parse_state ps;
	struct dag_node* root;

	bool valid = parse_setup("function Int64() end", &ps, &root);
	assert_has_errors(ps.el);
	assert_compile_error(ps.el, "expecting identifier");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_reserved_type3()
{
	test_name(__func__);

	struct parse_state ps;
	struct dag_node* root;

	bool valid = parse_setup("var list::Vector{Int64}; for Int64 in list end", &ps, &root);
	assert_has_errors(ps.el);
	assert_compile_error(ps.el, "expected identifier after for");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types_exists()
{
	test_name(__func__);

	struct parse_state ps;
	struct dag_node* root;

	bool valid = parse_setup("var x::SuperInt; x + 1", &ps, &root);
	assert_has_errors(ps.el);
	assert_compile_error(ps.el, "expected a type");
	expect_false(valid, "valid");

	parse_teardown(&ps);
}

void test_parse_types()
{
	test_parse_types_missing_declaration();
	test_parse_types_missing_declaration2();
	test_parse_types_missing_declaration3();
	test_parse_types_double();
	test_parse_types_double_function();
	test_parse_types_reserved_type();
	test_parse_types_reserved_type2();
	test_parse_types_reserved_type3();
	test_parse_types_exists();
}
