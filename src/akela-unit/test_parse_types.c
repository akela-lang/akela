#include "zinc/unit_test.h"
#include "unit_parse.h"
#include "akela/type_def.h"
#include "akela/comp_unit.h"
#include "zinc/error_unit_test.h"

void test_parse_types_missing_declaration()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("x + 1", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "variable not declared: x");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_types_missing_declaration2()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("foo() + 1", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "variable not declared: foo");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_types_missing_declaration3()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("x = fn() end", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "variable not declared: x");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_types_double_function()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; fn foo() end", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "duplicate declaration in same scope: foo");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_types_reserved_type()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const Int32: Int32", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "identifier reserved as a type: Int32");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_types_reserved_type2()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn Int32() end", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "identifier reserved as a type: Int32");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_types_reserved_type3()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("for Int32: Int32 = 1:10 end", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "identifier reserved as a type: Int32");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_types_reserved_type4()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const list: [10]Int32; for Int32: Int32 in list end", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "identifier reserved as a type: Int32");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_types_exists()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const x: SuperInt; x + 1", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "type not defined: SuperInt");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_types_array()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: [10]Int32; a[1]", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "AkeUnit_parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* const_ = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(const_, "ptr const");
	Zinc_expect_int_equal(const_->type, Ake_ast_type_const, "type const");

	Ake_ast* let_lseq = Ast_node_get(const_, 0);
	Zinc_assert_ptr(let_lseq, "ptr let_lseq");
	Zinc_expect_int_equal(let_lseq->type, Ake_ast_type_let_lseq, "type let_lseq");

	Ake_ast* name = Ast_node_get(let_lseq, 0);
	Zinc_assert_ptr(name, "ptr name");
	Zinc_expect_int_equal(name->type, Ake_ast_type_id, "id name");
	Zinc_expect_string(&name->value, "a", "a name");

	Ake_ast* let_type = Ast_node_get(const_, 1);
	Zinc_assert_ptr(let_type, "ptr let_type");
	Zinc_expect_int_equal(let_type->type, Ake_ast_type_type, "type let_type");

	Ake_ast* as = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(as, "ptr as");
	Zinc_expect_int_equal(as->type, Ake_ast_type_array_subscript, "type array-subscript");

	Ake_ast* id = Ast_node_get(as, 0);
	Zinc_assert_ptr(id, "ptr id");
	Zinc_expect_int_equal(id->type, Ake_ast_type_id, "id id");
	Zinc_expect_string(&id->value, "a", "a id");

	Ake_ast* index = Ast_node_get(as, 1);
	Zinc_assert_ptr(index, "ptr index");
	Zinc_expect_int_equal(index->type, Ake_ast_type_number, "number index");
	Zinc_expect_string(&index->value, "1", "1 index");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_error_dseq_comma()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(a: Int32,) end", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "expected declaration after comma");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_error_declaration_colon()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "expected colon after variable(s)");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_error_declaration_type()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: ", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "expected type identifier or fn");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_error_type_not_defined()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: Foo", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "type not defined: Foo");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_error_not_a_type()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const foo: Int32; const a: foo", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "identifier is not a type: foo");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_error_duplicate_declarations()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const x: Int32; const x: Int32", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "duplicate declaration in same scope: x");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_error_return_type()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo()->Int32 true end", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "returned type does not match function return type");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_types_error_param()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(a: Int32) true end; foo(true)", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "parameter and aguments types do not match");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_types_error_param_no_value()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(a: Int32) true end; foo(foo(1))", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_source_error(&cu.errors, "argument expression has no value");
	Zinc_expect_false(cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void test_parse_types_newline_declaration()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a\n: \nInt32", &cu);
    Zinc_expect_no_errors(&cu.errors);
    Zinc_expect_true(cu.valid, "valid");

    Ake_ast* const_ = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(const_, "ptr let");
    Zinc_expect_int_equal(const_->type, Ake_ast_type_const, "type let");

    AkeUnit_parse_teardown(&cu);
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
