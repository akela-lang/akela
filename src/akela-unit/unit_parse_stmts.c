#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "unit_parse.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_if_error_expected_end(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("if true", &cu);
	if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_false(test, cu.valid, "valid");
	Zinc_expect_source_error(test, &cu.errors, "expected end");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
void AkeUnit_parse_if_error_expected_elseif_expression(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("if true elseif end", &cu);
	if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_false(test, cu.valid, "valid");
	Zinc_expect_source_error(test, &cu.errors, "expected condition after elseif");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
void AkeUnit_parse_while(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("while true 1 end", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	if (!Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid")) {
		return Zinc_assert();
	}

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	if (!Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

	Ake_Ast* node = Ake_ast_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, node, "ptr node")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, node->kind, Ake_ast_type_while, "while node");
	Zinc_expect_null(test, node->type, "null node->tu");

	Ake_Ast* cond = Ake_ast_get(node, 0);
	if (!Zinc_expect_ptr(test, cond, "ptr cond")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cond->kind, Ake_ast_type_boolean, "boolean cond");
	Zinc_expect_string(test, &cond->value, "true", "true cond");

	Ake_Ast* stmts = Ake_ast_get(node, 1);
	if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts->kind, Ake_ast_type_stmts, "parse_stmts stmts");

	Ake_Ast* num = Ake_ast_get(stmts, 0);
	if (!Zinc_expect_ptr(test, num, "ptr num")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num->kind, Ake_ast_type_number, "number num");
	Zinc_expect_string(test, &num->value, "1", "1 num");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_while_error_expected_expression(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("while end", &cu);
	if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "expected expression after while");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_while_error_expected_end(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("while true", &cu);
	if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "expected end");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
void AkeUnit_parse_for_range(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("for i: Int32 = 0:10 1 end", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	if (!Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid")) {
		return Zinc_assert();
	}

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

	/* for */
	Ake_Ast* node = Ake_ast_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, node, "ptr node")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, node->kind, Ake_ast_type_for_range, "for-range node");

	/* i */
	Ake_Ast* dec = Ake_ast_get(node, 0);
	if (!Zinc_expect_ptr(test, dec, "ptr dec")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, dec->kind, Ake_ast_type_declaration, "declaration dec");

	Ake_Ast* id = Ake_ast_get(dec, 0);
	if (!Zinc_expect_ptr(test, id, "ptr id")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, id->kind, Ake_ast_type_id, "id id");
	Zinc_expect_string(test, &id->value, "i", "i id");

	/* start */
	Ake_Ast* start = Ake_ast_get(node, 1);
	if (!Zinc_expect_ptr(test, start, "ptr start")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, start->kind, Ake_ast_type_number, "number start");
	Zinc_expect_string(test, &start->value, "0", "0 start");

	/* end */
	Ake_Ast* end = Ake_ast_get(node, 2);
	if (!Zinc_expect_ptr(test, end, "ptr end")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, end->kind, Ake_ast_type_number, "number end");
	Zinc_expect_string(test, &end->value, "10", "10 end");

	/* stmts */
	Ake_Ast* stmts = Ake_ast_get(node, 3);
	if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts->kind, Ake_ast_type_stmts, "parse_stmts stmts");

	Ake_Ast* stmt0 = Ake_ast_get(stmts, 0);
	if (!Zinc_expect_ptr(test, stmt0, "ptr stmt0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmt0->kind, Ake_ast_type_number, "number stmt0");
	Zinc_expect_string(test, &stmt0->value, "1", "1 stmt0");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_iteration(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const list: [10]Int32; for i: Int32 in list i end", &cu);
	Zinc_expect_no_errors(test, &cu.errors);
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

	/* for */
	Ake_Ast* node = Ake_ast_get(cu.root, 1);
	if (!Zinc_expect_ptr(test, node, "ptr node")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, node->kind, Ake_ast_type_for_iteration, "for-iteration node");

	/* declaration */
	Ake_Ast* dec = Ake_ast_get(node, 0);
	if (!Zinc_expect_ptr(test, dec, "ptr dec")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, dec->kind, Ake_ast_type_declaration, "declaration dec");

	/* id */
	Ake_Ast* id = Ake_ast_get(dec, 0);
	if (!Zinc_expect_ptr(test, id, "ptr id")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, id->kind, Ake_ast_type_id, "id id");
	Zinc_expect_string(test, &id->value, "i", "i id");

	Ake_Ast* i_type_node = Ake_ast_get(dec, 1);
	if (!Zinc_expect_ptr(test, i_type_node, "ptr i_tu")) {
		return Zinc_assert();
	}

    Ake_Type* i_type = i_type_node->type;
    if (!Zinc_expect_ptr(test, i_type, "ptr i_tu")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, i_type->kind, AKE_TYPE_INTEGER, "integer i_td");
	Zinc_expect_string(test, &i_type->name, "Int32", "Int32 i_td");

	/* expr */
	Ake_Ast* expr = Ake_ast_get(node, 1);
	if (!Zinc_expect_ptr(test, expr, "ptr expr")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, expr->kind, Ake_ast_type_id, "id expr");
	Zinc_expect_string(test, &expr->value, "list", "id list");

	/* stmts */
	Ake_Ast* stmts0 = Ake_ast_get(node, 2);
	if (!Zinc_expect_ptr(test, stmts0, "ptr stmts0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts0->kind, Ake_ast_type_stmts, "parse_stmts stmts0");

	/* i */
	Ake_Ast* id2 = Ake_ast_get(stmts0, 0);
	if (!Zinc_expect_ptr(test, id2, "ptr id2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, id2->kind, Ake_ast_type_id, "id id2");
	Zinc_expect_string(test, &id2->value, "i", "i id2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_iteration_error_no_value(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn list() end; for i: Int32 in list() end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "iteration expression has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_iteration_error_no_child_element(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const list: Int32; for i: Int32 in list end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "iteration expression is not an array or slice");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_iteration_error_cannot_cast(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

    AkeUnit_parse_setup("const list: [10]Bool; for i: Int32 in list end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "cannot cast list element");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_error_after_declaration(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const list: [10]Int32; for i: Int32 end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "expected '=' or 'in' after for element declaration");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_error_expected_end(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const list: [10]Int32; for i: Int32 = 1:10", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "expected end");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_error_expected_range_start(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const list: [10]Int32; for i: Int32 =", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "expected range start");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_error_expected_colon(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const list: [10]Int32; for i: Int32 = 1", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "expected colon");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_error_expected_range_end(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const list: [10]Int32; for i: Int32 = 1:", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "expected range end");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_range_error_start_no_value(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; for i: Int32 = foo():10 end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "start range expression has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_range_error_start_not_numeric(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("for i: Int32 = true:10 end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "start range expression is not numeric");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_range_error_end_no_value(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; for i: Int32 = 1:foo() end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "end range expression has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_range_error_end_not_numeric(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("for i: Int32 = 1:true end", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "end range expression is not numeric");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_for_error_expected_iteration_expression(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const list: [10]Int32; for i: Int32 in:", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "expected for iteration expression");

    AkeUnit_parse_teardown(&cu);
}

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
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, AkeUnit_parse_if_error_expected_end);
		Zinc_test_register(test, AkeUnit_parse_if_error_expected_elseif_expression);
		Zinc_test_register(test, AkeUnit_parse_while);
		Zinc_test_register(test, AkeUnit_parse_while_error_expected_expression);
		Zinc_test_register(test, AkeUnit_parse_while_error_expected_end);
		Zinc_test_register(test, AkeUnit_parse_for_range);
		Zinc_test_register(test, AkeUnit_parse_for_range_error_start_no_value);
		Zinc_test_register(test, AkeUnit_parse_for_range_error_start_not_numeric);
		Zinc_test_register(test, AkeUnit_parse_for_range_error_end_no_value);
		Zinc_test_register(test, AkeUnit_parse_for_range_error_end_not_numeric);
		Zinc_test_register(test, AkeUnit_parse_for_iteration);
		Zinc_test_register(test, AkeUnit_parse_for_iteration_error_no_value);
		Zinc_test_register(test, AkeUnit_parse_for_iteration_error_no_child_element);
		Zinc_test_register(test, AkeUnit_parse_for_iteration_error_cannot_cast);
		Zinc_test_register(test, AkeUnit_parse_for_error_after_declaration);
		Zinc_test_register(test, AkeUnit_parse_for_error_expected_end);
		Zinc_test_register(test, AkeUnit_parse_for_error_expected_range_start);
		Zinc_test_register(test, AkeUnit_parse_for_error_expected_colon);
		Zinc_test_register(test, AkeUnit_parse_for_error_expected_range_end);
		Zinc_test_register(test, AkeUnit_parse_for_error_expected_iteration_expression);
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
