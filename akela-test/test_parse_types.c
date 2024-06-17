#include "zinc/unit_test.h"
#include "test_parse.h"
#include "akela/type_def.h"
#include "akela/comp_unit.h"
#include "zinc/error_unit_test.h"

void test_parse_types_missing_declaration()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("x + 1", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "variable not declared: x");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_missing_declaration2()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("foo() + 1", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "variable not declared: foo");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_missing_declaration3()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("x = fn() end", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "variable not declared: x");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_double_function()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; fn foo() end", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "duplicate declaration in same scope: foo");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_reserved_type()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let i64: i64", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "identifier reserved as a type: i64");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_reserved_type2()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn i64() end", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "identifier reserved as a type: i64");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_reserved_type3()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("for i64: i64 = 1:10 end", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "identifier reserved as a type: i64");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_reserved_type4()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let list: [10]i64; for i64: i64 in list end", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "identifier reserved as a type: i64");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_exists()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let x: SuperInt; x + 1", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "type not defined: SuperInt");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_array()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a: [10]i64; a[1]", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	Ast_node* let = Ast_node_get(cu.root, 0);
	assert_ptr(let, "ptr let");
	expect_int_equal(let->type, ast_type_let, "type let");

	Ast_node* let_lseq = Ast_node_get(let, 0);
	assert_ptr(let_lseq, "ptr let_lseq");
	expect_int_equal(let_lseq->type, ast_type_let_lseq, "type let_lseq");

	Ast_node* name = Ast_node_get(let_lseq, 0);
	assert_ptr(name, "ptr name");
	expect_int_equal(name->type, ast_type_id, "id name");
	expect_str(&name->value, "a", "a name");

	Ast_node* let_type = Ast_node_get(let, 1);
	assert_ptr(let_type, "ptr let_type");
	expect_int_equal(let_type->type, ast_type_type, "type let_type");

	Ast_node* as = Ast_node_get(cu.root, 1);
	assert_ptr(as, "ptr as");
	expect_int_equal(as->type, ast_type_array_subscript, "type array-subscript");

	Ast_node* id = Ast_node_get(as, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "a", "a id");

	Ast_node* index = Ast_node_get(as, 1);
	assert_ptr(index, "ptr index");
	expect_int_equal(index->type, ast_type_number, "number index");
	expect_str(&index->value, "1", "1 index");

    parse_teardown(&cu);
}

void test_parse_error_dseq_comma()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo(a: i64,) end", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "expected declaration after comma");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_error_declaration_colon()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "expected colon after variable(s)");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_error_declaration_type()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("let a: ", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "expected type identifier or fn");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_error_type_not_defined()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a: Foo", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "type not defined: Foo");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_error_not_a_type()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("let foo: i64; let a: foo", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "identifier is not a type: foo");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_error_duplicate_declarations()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let x: i64; let x: i64", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "duplicate declaration in same scope: x");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_error_return_type()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo()->i64 true end", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "returned type does not match function return type");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_error_param()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo(a: i64) true end; foo(true)", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "parameter and aguments types do not match");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_error_param_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo(a: i64) true end; foo(foo(1))", &cu);
	expect_has_errors(&cu.el);
	expect_source_error(&cu.el, "argument expression has no value");
	expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_newline_declaration()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a\n: \ni64", &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    Ast_node* let = Ast_node_get(cu.root, 0);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, ast_type_let, "type let");

    parse_teardown(&cu);
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
	test_parse_error_dseq_comma();
    test_parse_error_declaration_colon();
	test_parse_error_declaration_type();
	test_parse_error_type_not_defined();
	test_parse_error_not_a_type();
	test_parse_error_duplicate_declarations();
	test_parse_error_return_type();
	test_parse_types_error_param();
	test_parse_types_error_param_no_value();
    test_parse_types_newline_declaration();
}
