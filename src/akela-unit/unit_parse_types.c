#include "unit_parse.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_types_array(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: [10]Int32; a[1]", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_Ast* const_ = Ake_ast_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, const_, "ptr const")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, const_->kind, Ake_ast_type_const, "type const");

	Ake_Ast* name = Ake_ast_get(const_, 0);
	if (!Zinc_expect_ptr(test, name, "ptr name")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, name->kind, Ake_ast_type_id, "id name");
	Zinc_expect_string(test, &name->value, "a", "a name");

	Ake_Ast* let_type = Ake_ast_get(const_, 1);
	if (!Zinc_expect_ptr(test, let_type, "ptr let_type")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, let_type->kind, Ake_ast_type_type, "type let_type");

	Ake_Ast* as = Ake_ast_get(cu.root, 1);
	if (!Zinc_expect_ptr(test, as, "ptr as")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, as->kind, Ake_ast_type_array_subscript, "type array-subscript");

	Ake_Ast* id = Ake_ast_get(as, 0);
	if (!Zinc_expect_ptr(test, id, "ptr id")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, id->kind, Ake_ast_type_id, "id id");
	Zinc_expect_string(test, &id->value, "a", "a id");

	Ake_Ast* index = Ake_ast_get(as, 1);
	if (!Zinc_expect_ptr(test, index, "ptr index")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, index->kind, Ake_ast_type_number, "number index");
	Zinc_expect_string(test, &index->value, "1", "1 index");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_error_dseq_comma(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(a: Int32,) end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "expected declaration after comma");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_error_declaration_colon(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "expected colon after variable(s)");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_error_declaration_type(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}
	
	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: ", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "expected type identifier or fn");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_error_type_not_defined(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: Foo", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "type not defined: Foo");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_error_not_a_type(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}
	
	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const foo: Int32; const a: foo", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "identifier is not a type: foo");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_error_return_type(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo()->Int32 true end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "returned type does not match function return type");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types_error_param(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(a: Int32) true end; foo(true)", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "parameter and arguments types do not match");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types_error_param_no_value(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(a: Int32) true end; foo(foo(1))", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "argument expression has no value");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types_newline_declaration(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a\n: \nInt32", &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_Ast* const_ = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, const_, "ptr let")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, const_->kind, Ake_ast_type_const, "type let");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, AkeUnit_parse_types_array);
		Zinc_test_register(test, AkeUnit_parse_error_dseq_comma);
		Zinc_test_register(test, AkeUnit_parse_error_declaration_colon);
		Zinc_test_register(test, AkeUnit_parse_error_declaration_type);
		Zinc_test_register(test, AkeUnit_parse_error_type_not_defined);
		Zinc_test_register(test, AkeUnit_parse_error_not_a_type);
		Zinc_test_register(test, AkeUnit_parse_error_return_type);
		Zinc_test_register(test, AkeUnit_parse_types_error_param);
		Zinc_test_register(test, AkeUnit_parse_types_error_param_no_value);
		Zinc_test_register(test, AkeUnit_parse_types_newline_declaration);

		return;
	}

	Zinc_test_perform(test);
}
