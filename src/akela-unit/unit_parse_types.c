#include "unit_parse.h"
#include "akela/type_def.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_types_missing_declaration(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("x + 1", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "variable not declared: x");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types_missing_declaration2(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("foo() + 1", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "variable not declared: foo");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types_missing_declaration3(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("x = fn() end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "variable not declared: x");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types_double_function(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; fn foo() end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "duplicate declaration in same scope: foo");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types_reserved_type(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const Int32: Int32", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "identifier reserved as a type: Int32");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types_reserved_type2(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn Int32() end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "identifier reserved as a type: Int32");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types_reserved_type3(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("for Int32: Int32 = 1:10 end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "identifier reserved as a type: Int32");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types_reserved_type4(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const list: [10]Int32; for Int32: Int32 in list end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "identifier reserved as a type: Int32");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types_exists(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const x: SuperInt; x + 1", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "type not defined: SuperInt");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

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
	Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* const_ = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, const_, "ptr const")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, const_->type, Ake_ast_type_const, "type const");

	Ake_ast* let_lseq = Ast_node_get(const_, 0);
	if (!Zinc_expect_ptr(test, let_lseq, "ptr let_lseq")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, let_lseq->type, Ake_ast_type_let_lseq, "type let_lseq");

	Ake_ast* name = Ast_node_get(let_lseq, 0);
	if (!Zinc_expect_ptr(test, name, "ptr name")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, name->type, Ake_ast_type_id, "id name");
	Zinc_expect_string(test, &name->value, "a", "a name");

	Ake_ast* let_type = Ast_node_get(const_, 1);
	if (!Zinc_expect_ptr(test, let_type, "ptr let_type")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, let_type->type, Ake_ast_type_type, "type let_type");

	Ake_ast* as = Ast_node_get(cu.root, 1);
	if (!Zinc_expect_ptr(test, as, "ptr as")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, as->type, Ake_ast_type_array_subscript, "type array-subscript");

	Ake_ast* id = Ast_node_get(as, 0);
	if (!Zinc_expect_ptr(test, id, "ptr id")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, id->type, Ake_ast_type_id, "id id");
	Zinc_expect_string(test, &id->value, "a", "a id");

	Ake_ast* index = Ast_node_get(as, 1);
	if (!Zinc_expect_ptr(test, index, "ptr index")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, index->type, Ake_ast_type_number, "number index");
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

void AkeUnit_parse_error_duplicate_declarations(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const x: Int32; const x: Int32", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "duplicate declaration in same scope: x");
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
	Zinc_expect_source_error(test, &cu.errors, "parameter and aguments types do not match");
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

    Ake_ast* const_ = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, const_, "ptr let")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, const_->type, Ake_ast_type_const, "type let");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, AkeUnit_parse_types_missing_declaration);
		Zinc_test_register(test, AkeUnit_parse_types_missing_declaration2);
		Zinc_test_register(test, AkeUnit_parse_types_missing_declaration3);
		Zinc_test_register(test, AkeUnit_parse_types_double_function);
		Zinc_test_register(test, AkeUnit_parse_types_reserved_type);
		Zinc_test_register(test, AkeUnit_parse_types_reserved_type2);
		Zinc_test_register(test, AkeUnit_parse_types_reserved_type3);
		Zinc_test_register(test, AkeUnit_parse_types_reserved_type4);
		Zinc_test_register(test, AkeUnit_parse_types_exists);
		Zinc_test_register(test, AkeUnit_parse_types_array);
		Zinc_test_register(test, AkeUnit_parse_error_dseq_comma);
		Zinc_test_register(test, AkeUnit_parse_error_declaration_colon);
		Zinc_test_register(test, AkeUnit_parse_error_declaration_type);
		Zinc_test_register(test, AkeUnit_parse_error_type_not_defined);
		Zinc_test_register(test, AkeUnit_parse_error_not_a_type);
		Zinc_test_register(test, AkeUnit_parse_error_duplicate_declarations);
		Zinc_test_register(test, AkeUnit_parse_error_return_type);
		Zinc_test_register(test, AkeUnit_parse_types_error_param);
		Zinc_test_register(test, AkeUnit_parse_types_error_param_no_value);
		Zinc_test_register(test, AkeUnit_parse_types_newline_declaration);

		return;
	}

	Zinc_test_perform(test);
}
