#include <stdbool.h>
#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "unit_parse.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

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
	Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_Ast* not = Ake_ast_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, not, "ptr not")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, not->kind, Ake_ast_type_not, "not not");

	Ake_Type* type = not->type;
	if (!Zinc_expect_ptr(test, type, "ptr tu")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, type->kind, AKE_TYPE_BOOLEAN, "boolean td");
	Zinc_expect_string(test, &type->name, "Bool", "name td");

	Ake_Ast* lit_bool = Ake_ast_get(not, 0);
	if (!Zinc_expect_ptr(test, lit_bool, "ptr lit_bool")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, lit_bool->kind, Ake_ast_type_boolean, "boolean true");
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
	Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_Ast* a = Ake_ast_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a->kind, Ake_ast_type_array_literal, "array-literal a");

	Ake_Type* a_type = a->type;
	if (!Zinc_expect_ptr(test, a_type, "ptr array_tu")) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, a_type->kind == AKE_TYPE_ARRAY, "type a_tu");
    Zinc_expect_uint8_t_equal(test, a_type->data.array.dim, 3, "dim a_tu");

	Ake_Type* a_type2 = a_type->data.array.type;
	if (!Zinc_expect_ptr(test, a_type2, "ptr array_td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a_type2->kind, AKE_TYPE_INTEGER, "integer a_tu2");
	Zinc_expect_string(test, &a_type2->name, "Int32", "Int32 array_td");
	Zinc_expect_uint8_t_equal(test, a_type2->data.integer.bit_count, 32, "bit_count a_tu2");

	Ake_Ast* a0 = Ake_ast_get(a, 0);
	if (!Zinc_expect_ptr(test, a0, "ptr a0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a0->kind, Ake_ast_type_number, "number a0");
	Zinc_expect_string(test, &a0->value, "1", "1 a0");

	Ake_Ast* a1 = Ake_ast_get(a, 1);
	if (!Zinc_expect_ptr(test, a1, "ptr a1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a1->kind, Ake_ast_type_number, "number a1");
	Zinc_expect_string(test, &a1->value, "2", "2 a1");

	Ake_Ast* a2 = Ake_ast_get(a, 2);
	if (!Zinc_expect_ptr(test, a2, "ptr a2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a2->kind, Ake_ast_type_number, "number a2");
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
	Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_Ast* a = Ake_ast_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a->kind, Ake_ast_type_array_literal, "array-literal a");

	Ake_Type* array_type = a->type;
	if (!Zinc_expect_ptr(test, array_type, "ptr array_tu")) {
		return Zinc_assert();
	}

	Ake_Type* array_type2 = array_type->data.array.type;
	if (!Zinc_expect_ptr(test, array_type2, "ptr array_td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, array_type2->kind, AKE_TYPE_REAL, "float array_td");
	Zinc_expect_string(test, &array_type2->name, "Real64", "name array_td");
	Zinc_expect_uint8_t_equal(test, array_type2->data.real.bit_count, 64, "bit_count array_td");

	Ake_Ast* a0 = Ake_ast_get(a, 0);
	if (!Zinc_expect_ptr(test, a0, "ptr a0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a0->kind, Ake_ast_type_number, "number a0");
	Zinc_expect_string(test, &a0->value, "1.0", "1.0 a0");

	Ake_Ast* a1 = Ake_ast_get(a, 1);
	if (!Zinc_expect_ptr(test, a1, "ptr a1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a1->kind, Ake_ast_type_number, "number a1");
	Zinc_expect_string(test, &a1->value, "2.5", "2.5 a1");

	Ake_Ast* a2 = Ake_ast_get(a, 2);
	if (!Zinc_expect_ptr(test, a2, "ptr a2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a2->kind, Ake_ast_type_number, "number a2");
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

	Ake_comp_unit cu;

    AkeUnit_parse_setup("[1, 2.5, 3]", &cu);
	Zinc_expect_no_errors(test, &cu.errors);
	Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_Ast* a = Ake_ast_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a->kind, Ake_ast_type_array_literal, "array-literal a");

	Ake_Type* array_type = a->type;
	if (!Zinc_expect_ptr(test, array_type, "ptr array_tu")) {
		return Zinc_assert();
	}
	Ake_Type* array_type2 = array_type->data.array.type;
	if (!Zinc_expect_ptr(test, array_type2, "ptr array_td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, array_type2->kind, AKE_TYPE_INTEGER, "float array_td");
	Zinc_expect_string(test, &array_type2->name, "Int32", "name array_td");

	Ake_Ast* a0 = Ake_ast_get(a, 0);
	if (!Zinc_expect_ptr(test, a0, "ptr a0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a0->kind, Ake_ast_type_number, "number a0");
	Zinc_expect_string(test, &a0->value, "1", "1 a0");

	Ake_Ast* a1 = Ake_ast_get(a, 1);
	if (!Zinc_expect_ptr(test, a1, "ptr a1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a1->kind, Ake_ast_type_number, "number a1");
	Zinc_expect_string(test, &a1->value, "2.5", "2.5 a1");

	Ake_Ast* a2 = Ake_ast_get(a, 2);
	if (!Zinc_expect_ptr(test, a2, "ptr a2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a2->kind, Ake_ast_type_number, "number a2");
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

void AkeUnit_parse_array_literal_bool(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

	AkeUnit_parse_setup("[false,true,false]", &cu);
	Zinc_expect_no_errors(test, &cu.errors);
	Zinc_expect_true(test, cu.valid, "valid");

	Zinc_expect_ptr(test, cu.root, "ptr root");
	Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "type stmts");

	Ake_Ast* array = Ake_ast_get(cu.root, 0);
	Zinc_expect_ptr(test, array, "ptr array");
	Zinc_expect_int_equal(test, array->kind, Ake_ast_type_array_literal, "array literal");

	Ake_Ast* a0 = Ake_ast_get(array, 0);
	Zinc_expect_ptr(test, a0, "ptr a0");
	Zinc_expect_int_equal(test, a0->kind, Ake_ast_type_boolean, "type a0");
	Zinc_expect_string(test, &a0->value, "false", "value a0");

	Ake_Ast* a1 = Ake_ast_get(array, 1);
	Zinc_expect_ptr(test, a1, "ptr a1");
	Zinc_expect_int_equal(test, a1->kind, Ake_ast_type_boolean, "type a1");
	Zinc_expect_string(test, &a1->value, "true", "value a1");

	Ake_Ast* a2 = Ake_ast_get(array, 2);
	Zinc_expect_ptr(test, a2, "ptr a2");
	Zinc_expect_int_equal(test, a2->kind, Ake_ast_type_boolean, "type a2");
	Zinc_expect_string(test, &a2->value, "false", "value a2");

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
	Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_Ast* paren = Ake_ast_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, paren, "ptr paren")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, paren->kind, Ake_ast_type_parenthesis, "parenthesis paren");

	Ake_Ast* number = Ake_ast_get(paren, 0);
	Zinc_expect_int_equal(test, number->kind, Ake_ast_type_number, "number number");
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

    Ake_Ast* stmts = cu.root;
    if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, stmts->kind, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_Ast* not = stmts->head;
    if (!Zinc_expect_ptr(test, not, "ptr not")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, not->kind, not->kind, "not not");

    Ake_Ast* tr = not->head;
    if (!Zinc_expect_ptr(test, tr, "ptr tr")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, tr->kind, Ake_ast_type_boolean, "boolean tr");
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

    Ake_Ast* stmts = cu.root;
    if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, stmts->kind, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_Ast* sign = stmts->head;
    if (!Zinc_expect_ptr(test, sign, "ptr sign")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, sign->kind, sign->kind, "sign sign");

    Ake_Ast* op = sign->head;
    if (!Zinc_expect_ptr(test, op, "ptr op")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, op->kind, Ake_ast_type_minus, "minus op");

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

    Ake_Ast* stmts = cu.root;
    if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, stmts->kind, Ake_ast_type_stmts, "parse_stmts stmts");

    Ake_Ast* al = Ake_ast_get(stmts, 0);
    if (!Zinc_expect_ptr(test, al, "ptr al")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, al->kind, Ake_ast_type_array_literal, "array_literal al");

    Ake_Ast* one = Ake_ast_get(al, 0);
    if (!Zinc_expect_ptr(test, one, "ptr one")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, one->kind, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1");

    Ake_Ast* two = Ake_ast_get(al, 1);
    if (!Zinc_expect_ptr(test, two, "ptr two")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, two->kind, Ake_ast_type_number, "number two");
    Zinc_expect_string(test, &two->value, "2", "2");

    Ake_Ast* three = Ake_ast_get(al, 2);
    if (!Zinc_expect_ptr(test, three, "ptr three")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, three->kind, Ake_ast_type_number, "number three");
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

    Ake_Ast* stmts = cu.root;
    if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, stmts->kind, Ake_ast_type_stmts, "parse_stmts stmts");

    Ake_Ast* paren = Ake_ast_get(stmts, 0);
    if (!Zinc_expect_ptr(test, paren, "ptr paren")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, paren->kind, Ake_ast_type_parenthesis, " parenthesis paren");

    Ake_Ast* plus = Ake_ast_get(paren, 0);
    if (!Zinc_expect_ptr(test, plus, "ptr plus")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, plus->kind, Ake_ast_type_plus, "plus plus");

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

    AkeUnit_parse_setup("const a: [4 const]Int32 = [1, 2, 3, 4]\n"
                "a[0]\n",
                &cu);
    Zinc_expect_true(test, cu.valid, "valid");
    Zinc_expect_no_errors(test, &cu.errors);

    Ake_Ast* let = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, let, "ptr let")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, let->kind, Ake_ast_type_const, "type let");

    Ake_Ast* let_type_node = Ake_ast_get(let, 1);
    if (!Zinc_expect_ptr(test, let_type_node, "ptr type")) {
	    return Zinc_assert();
    }
	Zinc_expect_int_equal(test, let_type_node->kind, Ake_ast_type_type, "type type");

	Ake_Type* let_type = let_type_node->type;
	if (!Zinc_expect_ptr(test, let_type, "ptr type")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, let_type->kind, AKE_TYPE_ARRAY, "is_array type");
	Zinc_expect_true(test, let_type->data.array.is_const, "is_const let_type");
    Zinc_expect_size_t_equal(test, let_type->data.array.dim, 4, "size let_type_dim");

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

    AkeUnit_parse_setup("const a: [4 const]Int32 = [1, 2, 3, 4]\n"
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

		Zinc_test_register(test, AkeUnit_parse_not_literal);
		Zinc_test_register(test, AkeUnit_parse_not_error);
		Zinc_test_register(test, AkeUnit_parse_array_literal_integer);
		Zinc_test_register(test, AkeUnit_parse_array_literal_float);
		Zinc_test_register(test, AkeUnit_parse_array_literal_numeric);
		Zinc_test_register(test, AkeUnit_parse_array_literal_mixed_error);
		Zinc_test_register(test, AkeUnit_parse_array_literal_bool);
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
