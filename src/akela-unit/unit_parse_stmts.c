#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "unit_parse.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_stmts_newline_for_range(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("for\ni\n: \nInt32\n=\n0\n:\n10 i end", &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_stmts_newline_for_iteration(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const v: [10]Int32; for\nx\n: \nInt32\nin\nv x end", &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_const(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: Int32", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_Ast* _const_ = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, _const_, "ptr let")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, _const_->kind, Ake_ast_type_const, "type const_");

    Ake_Ast* id_node = Ake_ast_get(_const_, 0);
    if (!Zinc_expect_ptr(test, id_node, "ptr id_node")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, id_node->kind, Ake_ast_type_id, "type let_lseq");
    Zinc_expect_string(test, &id_node->value, "a", "a");

    Ake_Ast* type_node = Ake_ast_get(_const_, 1);
    if (!Zinc_expect_ptr(test, type_node, "ptr type_node")) {
		return Zinc_assert();
	}

    Ake_Type* type = type_node->type;
    if (!Zinc_expect_ptr(test, type, "ptr tu")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, type->kind, AKE_TYPE_INTEGER, "kind tu");
    Zinc_expect_string(test, &type->name, "Int32", "Int32 tu");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_let2(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    AkeUnit_parse_setup("const a: Int32 = 1", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "parse valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_Ast* let = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, let, "ptr let")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, let->kind, Ake_ast_type_const, "type let");

    Ake_Ast* id_node = Ake_ast_get(let, 0);
    if (!Zinc_expect_ptr(test, id_node, "ptr id_node")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, id_node->kind, Ake_ast_type_id, "type id_node");
    Zinc_expect_string(test, &id_node->value, "a", "a");

    Ake_Ast* type_node = Ake_ast_get(let, 1);
    if (!Zinc_expect_ptr(test, type_node, "ptr type_node")) {
		return Zinc_assert();
	}

    Ake_Type* type = type_node->type;
    if (!Zinc_expect_ptr(test, type, "ptr tu")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, type->kind, AKE_TYPE_INTEGER, "kind tu");
    Zinc_expect_string(test, &type->name, "Int32", "name tu");

    Ake_Ast* let_expr = Ake_ast_get(let, 2);
    if (!Zinc_expect_ptr(test, let_expr, "ptr let_rseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, let_expr->kind, Ake_ast_type_number, "type let_rseq");
    Zinc_expect_string(test, &let_expr->value, "1", "1");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_let_expected_declaration(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "expected id");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_extern(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("extern foo(a: Int32)\n"
                "foo(1)\n",
                &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    if (!Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid")) {
		return Zinc_assert();
	}

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "type cu.root");

    Ake_Ast* f = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, f, "ptr f")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f->kind, Ake_ast_type_extern, "type f");

    Ake_Type* type = f->type;
    if (!Zinc_expect_ptr(test, type, "ptr tu")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, type->kind, AKE_TYPE_FUNCTION, "kind tu");

    Ake_Ast* proto = Ake_ast_get(f, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->kind, Ake_ast_type_prototype, "type proto");

    Ake_Ast* fid = Ake_ast_get(proto, 0);
    if (!Zinc_expect_ptr(test, fid, "ptr fid")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fid->kind, Ake_ast_type_id, "id");

    Ake_Ast* dseq = Ake_ast_get(proto, 1);
    if (!Zinc_expect_ptr(test, dseq, "ptr dseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq->kind, Ake_ast_type_dseq, "dseq dseq");

    Ake_Ast* dret = Ake_ast_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->kind, Ake_ast_type_dret, "dret dret");

    Ake_Ast* call = Ake_ast_get(cu.root, 1);
    Zinc_expect_ptr(test, call, "ptr call");
    Zinc_expect_int_equal(test, call->kind, Ake_ast_type_call, "type call");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_stmts_mut(Zinc_test* test) {
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("var x: Int32 = 10\n"
                "x = 5\n"
                "x",
                &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

    Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "type cu.root");

    Ake_Ast *let = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, let, "ptr let")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, let->kind, Ake_ast_type_var, "type let");

    Ake_Ast *id_node = Ake_ast_get(let, 0);
    if (!Zinc_expect_ptr(test, id_node, "ptr let_lseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, id_node->kind, Ake_ast_type_id, "type let_lseq");

    Ake_Ast *assign = Ake_ast_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, assign, "ptr assign")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign->kind, Ake_ast_type_assign, "type assign");

    if (!Zinc_expect_ptr(test, assign->type, "ptr assign->tu")) {
		return Zinc_assert();
	}

    Ake_Ast *x = Ake_ast_get(assign, 0);
    if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, x->kind, Ake_ast_type_id, "type id");

    Zinc_expect_ptr(test, x->type, "ptr x->tu");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_stmts_error_mut(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const x: Int32 = 10\n"
                "x = 5\n"
                "x",
                &cu);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_source_error(test, &cu.errors, "immutable variable changed in assignment");
    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_statements(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);

		Zinc_test_register(test, AkeUnit_parse_stmts_newline_for_range);
		Zinc_test_register(test, AkeUnit_parse_stmts_newline_for_iteration);
		Zinc_test_register(test, AkeUnit_parse_const);
		Zinc_test_register(test, AkeUnit_parse_let2);
		Zinc_test_register(test, AkeUnit_parse_let_expected_declaration);
		Zinc_test_register(test, AkeUnit_parse_extern);
		Zinc_test_register(test, AkeUnit_parse_stmts_mut);
		Zinc_test_register(test, AkeUnit_parse_stmts_error_mut);

		return;
	}

	Zinc_test_perform(test);
}
