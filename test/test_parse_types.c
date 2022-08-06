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


void test_parse_types()
{
	test_parse_types_missing_declaration();
	test_parse_types_missing_declaration2();
	test_parse_types_missing_declaration3();
}
