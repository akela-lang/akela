#include <stdbool.h>
#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "unit_parse.h"
#include "akela/type_def.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_number_integer(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}
	
	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("32", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

	Ake_ast* number = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, number, "ptr number")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, number->type, Ake_ast_type_number, "number num");
	Zinc_expect_string(test, &number->value, "32", "32 num");

	Ake_type_use* tu = number->tu;
	if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

	struct Ake_type_def* td = tu->td;
	if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, td->type, Ake_type_integer, "type td");
	Zinc_expect_string(test, &td->name, "Int32", "str td");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
void AkeUnit_parse_number_float(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

    AkeUnit_parse_setup("5.0e0", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

	Ake_ast* number = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, number, "ptr num")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, number->type, Ake_ast_type_number, "number number");
	Zinc_expect_string(test, &number->value, "5.0e0", "5.0e0 number");

	Ake_type_use* tu = number->tu;
	if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

	Ake_type_def* td = tu->td;
	if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, td->type, Ake_type_float, "integer td");
	Zinc_expect_string(test, &td->name, "Real64", "str td");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_string(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

    AkeUnit_parse_setup("\"hello\"", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "type cu.root");

	Ake_ast* string = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, string, "ptr string")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, string->type, Ake_ast_type_string, "type string");
	Zinc_expect_string(test, &string->value, "hello", "value string");

	Ake_type_use* tu = string->tu;
	if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, tu->is_array, "is_array tu");
    Zinc_expect_size_t_equal(test, tu->dim.count, 1, "dim.count tu");

    Ake_type_dimension* dim = (Ake_type_dimension*)ZINC_VECTOR_PTR(&tu->dim, 0);
    if (!Zinc_expect_ptr(test, dim, "ptr dim")) {
	    return Zinc_assert();
    }
    Zinc_expect_size_t_equal(test, dim->size, 6, "size dim");
    Zinc_expect_int_equal(test, dim->option, Ake_array_element_const, "option dim");

	Ake_type_def* td = tu->td;
	if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, td->type, Ake_type_integer, "type td");
	Zinc_expect_string(test, &td->name, "Nat8", "name td");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_boolean_true(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

    AkeUnit_parse_setup("true", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* true_node = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, true_node, "ptr true_node")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, true_node->type, Ake_ast_type_boolean, "boolean true_node");
	Zinc_expect_string(test, &true_node->value, "true", "true true_node");

	Ake_type_use* tu = true_node->tu;
	if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

	Ake_type_def* td = tu->td;
	if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, td->type, Ake_type_boolean, "boolean td");
	Zinc_expect_string(test, &td->name, "Bool", "name td");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_boolean_false(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

    AkeUnit_parse_setup("false", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* false_node = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, false_node, "ptr false_node")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, false_node->type, Ake_ast_type_boolean, "boolean false_node");
	Zinc_expect_string(test, &false_node->value, "false", "false false_node");

	Ake_type_use* tu = false_node->tu;
	if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

	Ake_type_def* td = tu->td;
	if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, td->type, Ake_type_boolean, "boolean td");
	Zinc_expect_string(test, &td->name, "Bool", "name td");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_id(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

    AkeUnit_parse_setup("const x: Int64; x", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

	Ake_ast* let = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, let, "ptr let")) {
		return Zinc_assert();
	}
	if (!Zinc_expect_int_equal(test, let->type, Ake_ast_type_const, "type let")) {
		return Zinc_assert();
	}

	Ake_ast* let_lseq = Ast_node_get(let, 0);
	if (!Zinc_expect_ptr(test, let_lseq, "ptr let_lseq")) {
		return Zinc_assert();
	}
	if (!Zinc_expect_int_equal(test, let_lseq->type, Ake_ast_type_let_lseq, "let_lseq let_lseq")) {
		return Zinc_assert();
	}

	Ake_ast* id = Ast_node_get(let_lseq, 0);
	if (!Zinc_expect_ptr(test, id, "ptr id")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, id->type, Ake_ast_type_id, "id id");
	Zinc_expect_string(test, &id->value, "x", "x");

	Ake_ast* type = Ast_node_get(let, 1);
	if (!Zinc_expect_ptr(test, type, "ptr type")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, type->type, Ake_ast_type_type, "type type");

	Ake_ast* id2 = Ast_node_get(cu.root, 1);
	if (!Zinc_expect_ptr(test, id2, "ptr id2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, id2->type, Ake_ast_type_id, "id id2");
	Zinc_expect_string(test, &id2->value, "x", "x id2");

	Ake_type_use* tu = id2->tu;
	if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

	Ake_type_def* td = tu->td;
	if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, td->type, Ake_type_integer, "integer td");
	Zinc_expect_string(test, &td->name, "Int64", "name td");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_id2(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

    AkeUnit_parse_setup("const _a23: Int64; _a23", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

	Ake_ast* id = Ast_node_get(cu.root, 1);
	if (!Zinc_expect_ptr(test, id, "ptr id")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, id->type, Ake_ast_type_id, "id id");
	Zinc_expect_string(test, &id->value, "_a23", "_a23 id");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_id3(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a2: Int64; a2", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

	Ake_ast* id = Ast_node_get(cu.root, 1);
	if (!Zinc_expect_ptr(test, id, "ptr id")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, id->type, Ake_ast_type_id, "id id");
	Zinc_expect_string(test, &id->value, "a2", "a2 id");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_id_greek(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const αβγ: Int64; αβγ", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
	    return Zinc_assert();
    }
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* id = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, id, "ptr id")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, id->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &id->value, "αβγ", "value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_id_cyrillic(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

    AkeUnit_parse_setup("const я: Int32; я", &cu);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_error* e = Zinc_expect_source_error(test, &cu.errors, "Unrecognized character: я");
    if (!Zinc_expect_ptr(test, e, "ptr e")) {
	    return Zinc_assert();
    }
    Zinc_expect_size_t_equal(test, e->loc.start, 6, "start_pos");
    Zinc_expect_size_t_equal(test, e->loc.end, 8, "size");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line");
    Zinc_expect_size_t_equal(test, e->loc.col, 7, "col");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_sign_negative(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

    AkeUnit_parse_setup("-30", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* sign = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, sign, "ptr sign")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, sign->type, Ake_ast_type_sign, "sign sign");

	Ake_type_use* tu = sign->tu;
	if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

	Ake_type_def* td = tu->td;
	if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, td->type, Ake_type_integer, "integer td");
	Zinc_expect_string(test, &td->name, "Int32", "Int32 td");

	Ake_ast* left = Ast_node_get(sign, 0);
	if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, left->type, Ake_ast_type_minus, "minus");

	Ake_ast* right = Ast_node_get(sign, 1);
	if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number");
	Zinc_expect_string(test, &right->value, "30", "30");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_sign_positive(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("+30", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

	Ake_ast* sign = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, sign, "ptr sign")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, sign->type, Ake_ast_type_sign, "sign sign");

	Ake_type_use* tu = sign->tu;
	if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

	struct Ake_type_def* td = tu->td;
	if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, td->type, Ake_type_integer, "integer td");
	Zinc_expect_string(test, &td->name, "Int32", "Int32 td");

	Ake_ast* left = Ast_node_get(sign, 0);
	if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
	if (!Zinc_expect_int_equal(test, left->type, Ake_ast_type_plus, "plus")) {
		return Zinc_assert();
	}

	Ake_ast* right = Ast_node_get(sign, 1);
	if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
	if (!Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number")) {
		return Zinc_assert();
	}
	Zinc_expect_string(test, &right->value, "30", "30");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_sign_error_no_value(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end\n-foo()", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "negative operator was used on expression with no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_sign_expected_factor(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

    AkeUnit_parse_setup("-", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "expected parse_factor after sign");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_not_id(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: Bool; !a", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* not = Ast_node_get(cu.root, 1);
	if (!Zinc_expect_ptr(test, not, "ptr not")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, not->type, Ake_ast_type_not, "not not");

	Ake_type_use* tu = not->tu;
	if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

	Ake_type_def* td = tu->td;
	if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, td->type, Ake_type_boolean, "boolean td");
	Zinc_expect_string(test, &td->name, "Bool", "name td");

	Ake_ast* id = Ast_node_get(not, 0);
	if (!Zinc_expect_ptr(test, id, "ptr id")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, id->type, Ake_ast_type_id, "id id");
	Zinc_expect_string(test, &id->value, "a", "a id");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_not_literal(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("!true", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* not = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, not, "ptr not")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, not->type, Ake_ast_type_not, "not not");

	Ake_type_use* tu = not->tu;
	if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

	struct Ake_type_def* td = tu->td;
	if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, td->type, Ake_type_boolean, "boolean td");
	Zinc_expect_string(test, &td->name, "Bool", "name td");

	Ake_ast* lit_bool = Ast_node_get(not, 0);
	if (!Zinc_expect_ptr(test, lit_bool, "ptr lit_bool")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, lit_bool->type, Ake_ast_type_boolean, "boolean true");
	Zinc_expect_string(test, &lit_bool->value, "true", "true lit_bool");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_not_error(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; !foo()", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "! operator used on parse_factor with no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_array_literal_integer(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("[1,2,3]", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* a = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a->type, Ake_ast_type_array_literal, "array-literal a");

	Ake_type_use* a_tu = a->tu;
	if (!Zinc_expect_ptr(test, a_tu, "ptr array_tu")) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, a_tu->is_array, "is_array a_tu");
    Zinc_expect_size_t_equal(test, a_tu->dim.count, 1, "dim.count a_tu");
    Zinc_expect_size_t_equal(test, *(size_t*)ZINC_VECTOR_PTR(&a_tu->dim, 0), 3, "dim[0] a_tu");

	struct Ake_type_def* a_td = a_tu->td;
	if (!Zinc_expect_ptr(test, a_td, "ptr array_td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a_td->type, Ake_type_integer, "integer array_td");
	Zinc_expect_string(test, &a_td->name, "Int32", "Int32 array_td");

	Ake_ast* a0 = Ast_node_get(a, 0);
	if (!Zinc_expect_ptr(test, a0, "ptr a0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a0->type, Ake_ast_type_number, "number a0");
	Zinc_expect_string(test, &a0->value, "1", "1 a0");

	Ake_ast* a1 = Ast_node_get(a, 1);
	if (!Zinc_expect_ptr(test, a1, "ptr a1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a1->type, Ake_ast_type_number, "number a1");
	Zinc_expect_string(test, &a1->value, "2", "2 a1");

	Ake_ast* a2 = Ast_node_get(a, 2);
	if (!Zinc_expect_ptr(test, a2, "ptr a2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a2->type, Ake_ast_type_number, "number a2");
	Zinc_expect_string(test, &a2->value, "3", "3 a2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_array_literal_float(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("[1.0,2.5,3.2]", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* a = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a->type, Ake_ast_type_array_literal, "array-literal a");

	Ake_type_use* array_tu = a->tu;
	if (!Zinc_expect_ptr(test, array_tu, "ptr array_tu")) {
		return Zinc_assert();
	}

	struct Ake_type_def* array_td = array_tu->td;
	if (!Zinc_expect_ptr(test, array_td, "ptr array_td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, array_td->type, Ake_type_float, "float array_td");
	Zinc_expect_string(test, &array_td->name, "Real64", "name array_td");

	Ake_ast* a0 = Ast_node_get(a, 0);
	if (!Zinc_expect_ptr(test, a0, "ptr a0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a0->type, Ake_ast_type_number, "number a0");
	Zinc_expect_string(test, &a0->value, "1.0", "1.0 a0");

	Ake_ast* a1 = Ast_node_get(a, 1);
	if (!Zinc_expect_ptr(test, a1, "ptr a1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a1->type, Ake_ast_type_number, "number a1");
	Zinc_expect_string(test, &a1->value, "2.5", "2.5 a1");

	Ake_ast* a2 = Ast_node_get(a, 2);
	if (!Zinc_expect_ptr(test, a2, "ptr a2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a2->type, Ake_ast_type_number, "number a2");
	Zinc_expect_string(test, &a2->value, "3.2", "3.2 a2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_array_literal_numeric(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("[1, 2.5, 3]", &cu);
	Zinc_expect_no_errors(test, &cu.errors);
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* a = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a->type, Ake_ast_type_array_literal, "array-literal a");

	Ake_type_use* array_tu = a->tu;
	if (!Zinc_expect_ptr(test, array_tu, "ptr array_tu")) {
		return Zinc_assert();
	}

	struct Ake_type_def* array_td = array_tu->td;
	if (!Zinc_expect_ptr(test, array_td, "ptr array_td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, array_td->type, Ake_type_float, "float array_td");
	Zinc_expect_string(test, &array_td->name, "Real64", "name array_td");

	Ake_ast* a0 = Ast_node_get(a, 0);
	if (!Zinc_expect_ptr(test, a0, "ptr a0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a0->type, Ake_ast_type_number, "number a0");
	Zinc_expect_string(test, &a0->value, "1", "1 a0");

	Ake_ast* a1 = Ast_node_get(a, 1);
	if (!Zinc_expect_ptr(test, a1, "ptr a1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a1->type, Ake_ast_type_number, "number a1");
	Zinc_expect_string(test, &a1->value, "2.5", "2.5 a1");

	Ake_ast* a2 = Ast_node_get(a, 2);
	if (!Zinc_expect_ptr(test, a2, "ptr a2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a2->type, Ake_ast_type_number, "number a2");
	Zinc_expect_string(test, &a2->value, "3", "3 a2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_array_literal_mixed_error(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("[1,true,3]", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "array elements not the same type");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_array_literal_empty_error(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("[]", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "array literal has no elements");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_array_literal_error_right_square_bracket(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("[1,2", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "expected right square bracket");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_array_literal_error_expected_expr(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("[1,]", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "expected expr after comma");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_paren_num(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("(32)", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* paren = Ast_node_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, paren, "ptr paren")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

	Ake_ast* number = Ast_node_get(paren, 0);
	Zinc_expect_int_equal(test, number->type, Ake_ast_type_number, "number number");
	Zinc_expect_string(test, &number->value, "32", "32 number");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_paren_error_empty(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("()", &cu);
	if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	struct Zinc_error* e = Zinc_expect_source_error(test, &cu.errors, "empty parenthesis");
    if (e) {
        Zinc_expect_size_t_equal(test, e->loc.line, 1, "line");
        Zinc_expect_size_t_equal(test, e->loc.col, 2, "col");
        Zinc_expect_size_t_equal(test, e->loc.start, 1, "start_pos");
        Zinc_expect_size_t_equal(test, e->loc.end, 2, "size");
    }

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_paren_error_right_parenthesis(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("(1", &cu);
	if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "expected right parenthesis");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_paren_error_no_value(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; (foo())", &cu);
	if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "parenthesis on expression that has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_not_error_expected_factor(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("!", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "expected parse_factor after !");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_not_error_no_value(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; !foo()", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "! operator used on parse_factor with no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_not_error_not_boolean(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("!1", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
	Zinc_expect_source_error(test, &cu.errors, "not operator used on non-boolean");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor_newline_let(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const\nx: Int32", &cu);
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_no_errors(test, &cu.errors);

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor_newline_const_assign(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const\nx: Int32 =\n1", &cu);
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_no_errors(test, &cu.errors);

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor_newline_not(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("!\ntrue", &cu);
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_no_errors(test, &cu.errors);

    Ake_ast* stmts = cu.root;
    if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, stmts->type, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_ast* not = stmts->head;
    if (!Zinc_expect_ptr(test, not, "ptr not")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, not->type, not->type, "not not");

    Ake_ast* tr = not->head;
    if (!Zinc_expect_ptr(test, tr, "ptr tr")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, tr->type, Ake_ast_type_boolean, "boolean tr");
    Zinc_expect_string(test, &tr->value, "true", "true");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor_newline_sign(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("-\n1", &cu);
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_no_errors(test, &cu.errors);

    Ake_ast* stmts = cu.root;
    if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, stmts->type, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_ast* sign = stmts->head;
    if (!Zinc_expect_ptr(test, sign, "ptr sign")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, sign->type, sign->type, "sign sign");

    Ake_ast* op = sign->head;
    if (!Zinc_expect_ptr(test, op, "ptr op")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, op->type, Ake_ast_type_minus, "minus op");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor_newline_array_literal(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("[\n1,\n2,\n3\n]", &cu);
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_no_errors(test, &cu.errors);

    Ake_ast* stmts = cu.root;
    if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, stmts->type, Ake_ast_type_stmts, "parse_stmts stmts");

    Ake_ast* al = Ast_node_get(stmts, 0);
    if (!Zinc_expect_ptr(test, al, "ptr al")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, al->type, Ake_ast_type_array_literal, "array_literal al");

    Ake_ast* one = Ast_node_get(al, 0);
    if (!Zinc_expect_ptr(test, one, "ptr one")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, one->type, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1");

    Ake_ast* two = Ast_node_get(al, 1);
    if (!Zinc_expect_ptr(test, two, "ptr two")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, two->type, Ake_ast_type_number, "number two");
    Zinc_expect_string(test, &two->value, "2", "2");

    Ake_ast* three = Ast_node_get(al, 2);
    if (!Zinc_expect_ptr(test, three, "ptr three")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, three->type, Ake_ast_type_number, "number three");
    Zinc_expect_string(test, &three->value, "3", "3");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor_newline_array_parenthesis(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("(\n1+2)", &cu);
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_no_errors(test, &cu.errors);

    Ake_ast* stmts = cu.root;
    if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, stmts->type, Ake_ast_type_stmts, "parse_stmts stmts");

    Ake_ast* paren = Ast_node_get(stmts, 0);
    if (!Zinc_expect_ptr(test, paren, "ptr paren")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, paren->type, Ake_ast_type_parenthesis, " parenthesis paren");

    Ake_ast* plus = Ast_node_get(paren, 0);
    if (!Zinc_expect_ptr(test, plus, "ptr plus")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, plus->type, Ake_ast_type_plus, "plus plus");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor_array_element_const(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const mut a: [4 const]Int32 = [1, 2, 3, 4]\n"
                "a[0]\n",
                &cu);
    Zinc_expect_true(test, cu.valid, "valid");
    Zinc_expect_no_errors(test, &cu.errors);

    Ake_ast* let = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, let, "ptr let")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, let->type, Ake_ast_type_const, "type let");

    Ake_ast* let_type = Ast_node_get(let, 1);
    if (!Zinc_expect_ptr(test, let_type, "ptr type")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, let_type->type, Ake_ast_type_type, "type type");
    Zinc_expect_true(test, let_type->tu->is_array, "is_array type");
    Zinc_expect_size_t_equal(test, let_type->tu->dim.count, 1, "dim.count type");

    struct Ake_type_dimension* let_type_dim = (Ake_type_dimension*)ZINC_VECTOR_PTR(&let_type->tu->dim, 0);
    Zinc_expect_size_t_equal(test, let_type_dim->size, 4, "size let_type_dim");
    Zinc_expect_int_equal(test, let_type_dim->option, Ake_array_element_const, "option let_type_dim");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor_array_element_const_error(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const mut a: [4 const]Int32 = [1, 2, 3, 4]\n"
                "a[0] = 10\n",
                &cu);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_source_error(test, &cu.errors, "immutable variable changed in assignment");
    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, AkeUnit_parse_number_integer);
		Zinc_test_register(test, AkeUnit_parse_number_float);
		Zinc_test_register(test, AkeUnit_parse_string);
		Zinc_test_register(test, AkeUnit_parse_boolean_true);
		Zinc_test_register(test, AkeUnit_parse_boolean_false);
		Zinc_test_register(test, AkeUnit_parse_id);
		Zinc_test_register(test, AkeUnit_parse_id2);
		Zinc_test_register(test, AkeUnit_parse_id3);
		Zinc_test_register(test, AkeUnit_parse_id_greek);
		Zinc_test_register(test, AkeUnit_parse_id_cyrillic);
		Zinc_test_register(test, AkeUnit_parse_sign_negative);
		Zinc_test_register(test, AkeUnit_parse_sign_positive);
		Zinc_test_register(test, AkeUnit_parse_sign_error_no_value);
		Zinc_test_register(test, AkeUnit_parse_sign_expected_factor);
		Zinc_test_register(test, AkeUnit_parse_not_id);
		Zinc_test_register(test, AkeUnit_parse_not_literal);
		Zinc_test_register(test, AkeUnit_parse_not_error);
		Zinc_test_register(test, AkeUnit_parse_array_literal_integer);
		Zinc_test_register(test, AkeUnit_parse_array_literal_float);
		Zinc_test_register(test, AkeUnit_parse_array_literal_numeric);
		Zinc_test_register(test, AkeUnit_parse_array_literal_mixed_error);
		Zinc_test_register(test, AkeUnit_parse_array_literal_empty_error);
		Zinc_test_register(test, AkeUnit_parse_array_literal_error_right_square_bracket);
		Zinc_test_register(test, AkeUnit_parse_array_literal_error_expected_expr);
		Zinc_test_register(test, AkeUnit_parse_paren_num);
		Zinc_test_register(test, AkeUnit_parse_paren_error_empty);
		Zinc_test_register(test, AkeUnit_parse_paren_error_right_parenthesis);
		Zinc_test_register(test, AkeUnit_parse_paren_error_no_value);
		Zinc_test_register(test, AkeUnit_parse_not_error_expected_factor);
		Zinc_test_register(test, AkeUnit_parse_not_error_no_value);
		Zinc_test_register(test, AkeUnit_parse_not_error_not_boolean);
		Zinc_test_register(test, AkeUnit_parse_factor_newline_let);
		Zinc_test_register(test, AkeUnit_parse_factor_newline_const_assign);
		Zinc_test_register(test, AkeUnit_parse_factor_newline_not);
		Zinc_test_register(test, AkeUnit_parse_factor_newline_sign);
		Zinc_test_register(test, AkeUnit_parse_factor_newline_array_literal);
		Zinc_test_register(test, AkeUnit_parse_factor_newline_array_parenthesis);
		Zinc_test_register(test, AkeUnit_parse_factor_array_element_const);
		Zinc_test_register(test, AkeUnit_parse_factor_array_element_const_error);

		return;
	}

	Zinc_test_perform(test);
}
