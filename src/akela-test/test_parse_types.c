#include "zinc/unit_test.h"
#include "test_parse.h"
#include "akela/type_def.h"
#include "akela/comp_unit.h"
#include "zinc/error_unit_test.h"

void test_parse_types_missing_declaration()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("x + 1", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "variable not declared: x");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_missing_declaration2()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("foo() + 1", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "variable not declared: foo");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_missing_declaration3()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("x = fn() end", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "variable not declared: x");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_double_function()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; fn foo() end", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "duplicate declaration in same scope: foo");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_reserved_type()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let i64: i64", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "identifier reserved as a type: i64");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_reserved_type2()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn i64() end", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "identifier reserved as a type: i64");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_reserved_type3()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("for i64: i64 = 1:10 end", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "identifier reserved as a type: i64");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_reserved_type4()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let list: [10]i64; for i64: i64 in list end", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "identifier reserved as a type: i64");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_exists()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let x: SuperInt; x + 1", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "type not defined: SuperInt");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_array()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let a: [10]i64; a[1]", &cu);
	Zinc_assert_no_errors(&cu.el);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* let = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(let, "ptr let");
	Zinc_expect_int_equal(let->type, Ake_ast_type_let, "type let");

	Ake_ast* let_lseq = Ast_node_get(let, 0);
	Zinc_assert_ptr(let_lseq, "ptr let_lseq");
	Zinc_expect_int_equal(let_lseq->type, Ake_ast_type_let_lseq, "type let_lseq");

	Ake_ast* name = Ast_node_get(let_lseq, 0);
	Zinc_assert_ptr(name, "ptr name");
	Zinc_expect_int_equal(name->type, Ake_ast_type_id, "id name");
	Zinc_expect_str(&name->value, "a", "a name");

	Ake_ast* let_type = Ast_node_get(let, 1);
	Zinc_assert_ptr(let_type, "ptr let_type");
	Zinc_expect_int_equal(let_type->type, Ake_ast_type_type, "type let_type");

	Ake_ast* as = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(as, "ptr as");
	Zinc_expect_int_equal(as->type, Ake_ast_type_array_subscript, "type array-subscript");

	Ake_ast* id = Ast_node_get(as, 0);
	Zinc_assert_ptr(id, "ptr id");
	Zinc_expect_int_equal(id->type, Ake_ast_type_id, "id id");
	Zinc_expect_str(&id->value, "a", "a id");

	Ake_ast* index = Ast_node_get(as, 1);
	Zinc_assert_ptr(index, "ptr index");
	Zinc_expect_int_equal(index->type, Ake_ast_type_number, "number index");
	Zinc_expect_str(&index->value, "1", "1 index");

    parse_teardown(&cu);
}

void test_parse_error_dseq_comma()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo(a: i64,) end", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "expected declaration after comma");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_error_declaration_colon()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let a", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "expected colon after variable(s)");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_error_declaration_type()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("let a: ", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "expected type identifier or fn");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_error_type_not_defined()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let a: Foo", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "type not defined: Foo");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_error_not_a_type()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("let foo: i64; let a: foo", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "identifier is not a type: foo");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_error_duplicate_declarations()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let x: i64; let x: i64", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "duplicate declaration in same scope: x");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_error_return_type()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo()->i64 true end", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "returned type does not match function return type");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_error_param()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo(a: i64) true end; foo(true)", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "parameter and aguments types do not match");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_error_param_no_value()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo(a: i64) true end; foo(foo(1))", &cu);
	Zinc_expect_has_errors(&cu.el);
	Zinc_expect_source_error(&cu.el, "argument expression has no value");
	Zinc_expect_false(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_types_newline_declaration()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let a\n: \ni64", &cu);
    Zinc_expect_no_errors(&cu.el);
    Zinc_expect_true(cu.valid, "valid");

    Ake_ast* let = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(let, "ptr let");
    Zinc_expect_int_equal(let->type, Ake_ast_type_let, "type let");

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
