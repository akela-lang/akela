#include <stdbool.h>
#include "unit_parse.h"
#include "akela/ast.h"
#include "akela/parse_tools.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "akela/comp_table.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_blank(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Ake_comp_table* ct = NULL;
    Ake_comp_table_create_str(&ct, "");

    Ake_parse_result pr = Ake_comp_unit_parse(ct->primary);

    if (!Zinc_expect_no_errors(test, pr.errors)) {
		return Zinc_assert();
	}

    if (!Zinc_expect_ptr(test, pr.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, pr.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* node = Ast_node_get(pr.root, 0);
    if (!Zinc_expect_null(test, node, "null node")) {
		return Zinc_assert();
	}

    Ake_comp_table_free(ct);
}

void AkeUnit_parse_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    Ake_comp_table* ct = NULL;
    Ake_comp_table_create_str(&ct,
         "var speed: Int32 = 100; speed + 1"
    );

    Ake_parse_result pr = Ake_comp_unit_parse(ct->primary);

    if (!Zinc_expect_no_errors(test, pr.errors)) {
		return Zinc_assert();
	}

    if (!Zinc_expect_ptr(test, pr.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, pr.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* add = Ast_node_get(pr.root, 1);
    if (!Zinc_expect_ptr(test, add, "ptr add")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, add->type, Ake_ast_type_plus, "plus add");

    Ake_type_use* tu = add->tu;
    if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

    Ake_TypeDef* td = tu->td;
    if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, td->kind, AKE_TYPE_DEF_INTEGER, "integer td");
    Zinc_expect_string(test, &td->name, "Int32", "Int32 td");

    Ake_ast* left = Ast_node_get(add, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &left->value, "speed", "speed");

    Ake_ast* right = Ast_node_get(add, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &right->value, "1", "1");

    Ake_comp_table_free(ct);
}

void AkeUnit_parse_add_error_expected_term(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("1 +", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "expected term after additive operator");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_add_error_left_no_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; foo() + 1", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "addition operand has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_add_error_left_not_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("true + 1", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "addition on non-numeric operand");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_add_error_right_no_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; 1 + foo()", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "addition operand has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_add_error_right_not_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("1 + true", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "addition on non-numeric operand");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_add_mixed_types(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("1 + 5.0", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* add = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, add, "ptr add")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, add->type, Ake_ast_type_plus, "plus add");

    Ake_type_use* add_tu = add->tu;
    if (!Zinc_expect_ptr(test, add_tu, "ptr add_tu")) {
		return Zinc_assert();
	}

    Ake_TypeDef* add_td = add_tu->td;
    if (!Zinc_expect_ptr(test, add_td, "ptr add_td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, add_td->kind, AKE_TYPE_DEF_INTEGER, "kind add_td");
    Zinc_expect_string(test, &add_td->name, "Int32", "name add_td");

    Ake_ast* left = Ast_node_get(add, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_number, "number left");
    Zinc_expect_string(test, &left->value, "1", "1 left");

    Ake_ast* right = Ast_node_get(add, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &right->value, "5.0", "5.0 right");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_add_positive(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("var speed: Int32 = 100; speed + +1", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* add = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, add, "ptr add")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, add->type, Ake_ast_type_plus, "plus add");

    Ake_ast* left = Ast_node_get(add, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &left->value, "speed", "speed");

    Ake_ast* right = Ast_node_get(add, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_sign, "sign");

    if (!Zinc_expect_null(test, Ast_node_get(cu.root, 2), "only 2 children")) {
		return Zinc_assert();
	}

    Ake_ast* left2 = Ast_node_get(right, 0);
    if (!Zinc_expect_ptr(test, left2, "left2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_plus, "plus2");

    Ake_ast* right2 = Ast_node_get(right, 1);
    if (!Zinc_expect_ptr(test, right2, "right2")) {
		return Zinc_assert();
	}
    Zinc_expect_string(test, &right2->value, "1", "1");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_add_negative(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const speed: Int32; speed + -1", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* add = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, add, "ptr add")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, add->type, Ake_ast_type_plus, "plus add");

    Ake_ast* left = Ast_node_get(add, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &left->value, "speed", "speed");

    Ake_ast* right = Ast_node_get(add, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_sign, "sign");

    if (!Zinc_expect_null(test, Ast_node_get(add, 2), "only 2 children")) {
		return Zinc_assert();
	}

    Ake_ast* left2 = Ast_node_get(right, 0);
    if (!Zinc_expect_ptr(test, left2, "left2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_minus, "minus");

    Ake_ast* right2 = Ast_node_get(right, 1);
    if (!Zinc_expect_ptr(test, right2, "right2")) {
		return Zinc_assert();
	}
    Zinc_expect_string(test, &right2->value, "1", "1");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_sub(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const delta: Int32 = 3; 100 - delta", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* sub = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, sub, "ptr sub")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, sub->type, Ake_ast_type_minus, "minus sub");

    Ake_ast* left = Ast_node_get(sub, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &left->value, "100", "100");

    Ake_ast* right = Ast_node_get(sub, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &right->value, "delta", "delta");

    if (!Zinc_expect_null(test, Ast_node_get(cu.root, 2), "only 2 children")) {
		return Zinc_assert();
	}

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_sub_positive(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const speed: Int32; speed - +1", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* sub = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, sub, "cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, sub->type, Ake_ast_type_minus, "minus sub");

    Ake_ast* left = Ast_node_get(sub, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &left->value, "speed", "speed");

    Ake_ast* right = Ast_node_get(sub, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_sign, "sign");

    if (!Zinc_expect_null(test, Ast_node_get(sub, 2), "only 2 children")) {
		return Zinc_assert();
	}

    Ake_ast* left2 = Ast_node_get(right, 0);
    if (!Zinc_expect_ptr(test, left2, "left2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_plus, "plus");

    Ake_ast* right2 = Ast_node_get(right, 1);
    if (!Zinc_expect_ptr(test, right2, "right2")) {
		return Zinc_assert();
	}
    Zinc_expect_string(test, &right2->value, "1", "1");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_sub_negative(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const speed: Int32; speed - -1", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* sub = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, sub, "ptr sub")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, sub->type, Ake_ast_type_minus, "minus sub");

    Ake_ast* left = Ast_node_get(sub, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &left->value, "speed", "speed");

    Ake_ast* right = Ast_node_get(sub, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_sign, "sign");

    if (!Zinc_expect_null(test, Ast_node_get(sub, 2), "only 2 children")) {
		return Zinc_assert();
	}

    Ake_ast* left2 = Ast_node_get(right, 0);
    if (!Zinc_expect_ptr(test, left2, "left2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_minus, "minus2");

    Ake_ast* right2 = Ast_node_get(right, 1);
    if (!Zinc_expect_ptr(test, right2, "right2")) {
		return Zinc_assert();
	}
    Zinc_expect_string(test, &right2->value, "1", "1");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_mult(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("5 * 2", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* mult = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, mult, "cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, mult->type, Ake_ast_type_mult, "mult mult");

    Ake_type_use* tu = mult->tu;
    if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}
    
    Ake_TypeDef* td = tu->td;
    if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, td->kind, AKE_TYPE_DEF_INTEGER, "integer td");
    Zinc_expect_string(test, &td->name, "Int32", "Int32 td");

    Ake_ast* left = Ast_node_get(mult, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &left->value, "5", "5");

    Ake_ast* right = Ast_node_get(mult, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &right->value, "2", "2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_mult_error_expected_term(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("5*", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "expected term after operator");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
void AkeUnit_parse_mult_error_left_no_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; foo() * 1", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "multiplication operand has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_mult_error_left_not_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("true * 1", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "multiplication on non-numeric operand");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_mult_error_right_no_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; 1 * foo()", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "multiplication operand has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_mult_error_right_not_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("1 * true", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "multiplication on non-numeric operand");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_mult_positive(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const speed: Int32; speed * +1", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* mult = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, mult, "ptr mult")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, mult->type, Ake_ast_type_mult, "mult mult");

    Ake_ast* left = Ast_node_get(mult, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &left->value, "speed", "speed");

    Ake_ast* right = Ast_node_get(mult, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_sign, "sign");

    if (!Zinc_expect_null(test, Ast_node_get(mult, 2), "only 2 children")) {
		return Zinc_assert();
	}

    Ake_ast* left2 = Ast_node_get(right, 0);
    if (!Zinc_expect_ptr(test, left2, "left2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_plus, "plus");

    Ake_ast* right2 = Ast_node_get(right, 1);
    if (!Zinc_expect_ptr(test, right2, "right2")) {
		return Zinc_assert();
	}
    Zinc_expect_string(test, &right2->value, "1", "1");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_mult_negative(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const speed: Int32; speed * -1", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* mult = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, mult, "ptr mult")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, mult->type, Ake_ast_type_mult, "mult mult");

    Ake_ast* left = Ast_node_get(mult, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &left->value, "speed", "speed");

    Ake_ast* right = Ast_node_get(mult, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_sign, "sign");

    if (!Zinc_expect_null(test, Ast_node_get(mult, 2), "only 2 children")) {
		return Zinc_assert();
	}

    Ake_ast* left2 = Ast_node_get(right, 0);
    if (!Zinc_expect_ptr(test, left2, "left2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_minus, "minus2");

    Ake_ast* right2 = Ast_node_get(right, 1);
    if (!Zinc_expect_ptr(test, right2, "right2")) {
		return Zinc_assert();
	}
    Zinc_expect_string(test, &right2->value, "1", "1");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_divide(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("52 / 2", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* divide = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, divide, "ptr divide")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, divide->type, Ake_ast_type_divide, "divide divide")) {
		return Zinc_assert();
	}

    Ake_ast* left = Ast_node_get(divide, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &left->value, "52", "52");

    Ake_ast* right = Ast_node_get(divide, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &right->value, "2", "2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_add_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("1 + 2 + 3", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* plus0 = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, plus0, "ptr plus0")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, plus0->type, Ake_ast_type_plus, "plus plus0")) {
		return Zinc_assert();
	}

    Ake_ast* left = Ast_node_get(plus0, 0);
    if (!Zinc_expect_ptr(test, left, "ptr left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_plus, "plus left");

    Ake_ast* left2 = Ast_node_get(left, 0);
    if (!Zinc_expect_ptr(test, left2, "ptr left2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_number, "number left2");
    Zinc_expect_string(test, &left2->value, "1", "1 left2");

    Ake_ast* right = Ast_node_get(left, 1);
    if (!Zinc_expect_ptr(test, right, "ptr right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number right");
    Zinc_expect_string(test, &right->value, "2", "2 right");

    Ake_ast* right2 = Ast_node_get(plus0, 1);
    if (!Zinc_expect_ptr(test, right2, "ptr right2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right2->type, Ake_ast_type_number, "number right2");
    Zinc_expect_string(test, &right2->value, "3", "3 right2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_mult_mult(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("1 * 2 * 3", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* mult = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, mult, "ptr mult")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, mult->type, Ake_ast_type_mult, "mult mult")) {
		return Zinc_assert();
	}

    Ake_ast* left = Ast_node_get(mult, 0);
    if (!Zinc_expect_ptr(test, left, "ptr left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_mult, "mult left");

    Ake_ast* left2 = Ast_node_get(left, 0);
    if (!Zinc_expect_ptr(test, left2, "ptr left2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_number, "number left2");
    Zinc_expect_string(test, &left2->value, "1", "1 left2");

    Ake_ast* right = Ast_node_get(left, 1);
    if (!Zinc_expect_ptr(test, right, "ptr right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number right");
    Zinc_expect_string(test, &right->value, "2", "2 right");

    Ake_ast* right2 = Ast_node_get(mult, 1);
    if (!Zinc_expect_ptr(test, right2, "ptr right2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right2->type, Ake_ast_type_number, "number right2");
    Zinc_expect_string(test, &right2->value, "3", "3 right2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_add_mult(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("5 + 3 * 2", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* plus = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, plus, "cu.root")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, plus->type, Ake_ast_type_plus, "plus")) {
		return Zinc_assert();
	}

    Ake_ast* left = Ast_node_get(plus, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &left->value, "5", "5");

    Ake_ast* right = Ast_node_get(plus, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_mult, "mult");

    Ake_ast* left2 = Ast_node_get(right, 0);
    if (!Zinc_expect_ptr(test, left2, "left2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_number, "number 2");
    Zinc_expect_string(test, &left2->value, "3", "3");

    Ake_ast* right2 = Ast_node_get(right, 1);
    if (!Zinc_expect_ptr(test, right2, "right2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right2->type, Ake_ast_type_number, "number 3");
    Zinc_expect_string(test, &right2->value, "2", "2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_mult_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("4 * 3 + 2", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* plus = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, plus, "ptr plus")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, plus->type, Ake_ast_type_plus, "plus plus")) {
		return Zinc_assert();
	}

    Ake_ast* left = Ast_node_get(plus, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_mult, "mult");

    Ake_ast* left2 = Ast_node_get(left, 0);
    if (!Zinc_expect_ptr(test, left2, "left2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_number, "number 4");
    Zinc_expect_string(test, &left2->value, "4", "4");

    Ake_ast* right2 = Ast_node_get(left, 1);
    if (!Zinc_expect_ptr(test, right2, "right2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right2->type, Ake_ast_type_number, "number 3");
    Zinc_expect_string(test, &right2->value, "3", "3");

    Ake_ast* right = Ast_node_get(plus, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number 2");
    Zinc_expect_string(test, &right->value, "2", "2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_power(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("5 ^ 2", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "cu.valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* pow = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, pow, "ptr pow")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, pow->type, Ake_ast_type_power, "power pow");

    Ake_type_use* tu = pow->tu;
    if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}
    
    Ake_TypeDef* td = tu->td;
    if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, td->kind, AKE_TYPE_DEF_INTEGER, "integer td");
    Zinc_expect_string(test, &td->name, "Int32", "Int32 td");

    Ake_ast* number0 = Ast_node_get(pow, 0);
    if (!Zinc_expect_ptr(test, number0, "ptr number0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, number0->type, Ake_ast_type_number, "number number0");
    Zinc_expect_string(test, &number0->value, "5", "5 number0");

    Ake_ast* number1 = Ast_node_get(pow, 1);
    if (!Zinc_expect_ptr(test, number1, "ptr number1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, number1->type, Ake_ast_type_number, "number number1");
    Zinc_expect_string(test, &number1->value, "2", "2 number1");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_power_error_expected_term(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("5^", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "expected term after caret");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_power_error_left_no_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; 5 ^ foo()", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "power operand has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_power_error_left_not_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("true ^ 2", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "power on non-numeric operand");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_power_error_right_no_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; 5 ^ foo()", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "power operand has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_power_error_right_not_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("5 ^ true", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "power on non-numeric operand");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_paren_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const speed: Int32; (speed + 1)", &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* paren = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, paren, "ptr paren")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

    Ake_ast* plus = Ast_node_get(paren, 0);
    if (!Zinc_expect_ptr(test, plus, "cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, plus->type, Ake_ast_type_plus, "plus");

    Ake_ast* left = Ast_node_get(plus, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &left->value, "speed", "speed");

    Ake_ast* right = Ast_node_get(plus, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &right->value, "1", "1");

    if (!Zinc_expect_null(test, Ast_node_get(cu.root, 2), "only 2 children")) {
		return Zinc_assert();
	}

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_paren_add2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const speed: Int32; (speed) + 1", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* plus = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, plus, "cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, plus->type, Ake_ast_type_plus, "plus");

    Ake_ast* paren = Ast_node_get(plus, 0);
    if (!Zinc_expect_ptr(test, paren, "ptr paren")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

    Ake_ast* left = Ast_node_get(paren, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &left->value, "speed", "speed");

    Ake_ast* right = Ast_node_get(plus, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &right->value, "1", "1");

    if (!Zinc_expect_null(test, Ast_node_get(cu.root, 2), "only 2 children")) {
		return Zinc_assert();
	}

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_paren_add3(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const speed: Int32; speed + (1)", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* plus = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, plus, "cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, plus->type, Ake_ast_type_plus, "plus");

    Ake_ast* left = Ast_node_get(plus, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &left->value, "speed", "speed");

    Ake_ast* paren = Ast_node_get(plus, 1);
    if (!Zinc_expect_ptr(test, paren, "ptr paren")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

    Ake_ast* right = Ast_node_get(paren, 0);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &right->value, "1", "1");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_paren_add_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("1 + (2 + 3)", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* plus = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, plus, "ptr plus")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, plus->type, Ake_ast_type_plus, "plus plus")) {
		return Zinc_assert();
	}

    Ake_ast* left = Ast_node_get(plus, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &left->value, "1", "1");

    Ake_ast* paren = Ast_node_get(plus, 1);
    if (!Zinc_expect_ptr(test, paren, "ptr paren")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

    Ake_ast* right = Ast_node_get(paren, 0);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_plus, "plus 2");

    if (!Zinc_expect_null(test, Ast_node_get(cu.root, 2), "null")) {
		return Zinc_assert();
	}

    Ake_ast* left2 = Ast_node_get(right, 0);
    if (!Zinc_expect_ptr(test, left2, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_number, "number 2");
    Zinc_expect_string(test, &left2->value, "2", "2");

    Ake_ast* right2 = Ast_node_get(right, 1);
    if (!Zinc_expect_ptr(test, right2, "right2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right2->type, Ake_ast_type_number, "number 3");
    Zinc_expect_string(test, &right2->value, "3", "3");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_paren_add_add2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("(1 + 2) + 3", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* plus = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, plus, "ptr plus")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, plus->type, Ake_ast_type_plus, "plus plus")) {
		return Zinc_assert();
	}

    Ake_ast* paren = Ast_node_get(plus, 0);
    if (!Zinc_expect_ptr(test, paren, "ptr paren")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

    Ake_ast* left = Ast_node_get(paren, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_plus, "plus 2");

    Ake_ast* left2 = Ast_node_get(left, 0);
    if (!Zinc_expect_ptr(test, left2, "left2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &left2->value, "1", "1");

    Ake_ast* right = Ast_node_get(left, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number 2");
    Zinc_expect_string(test, &right->value, "2", "2");

    Ake_ast* right2 = Ast_node_get(plus, 1);
    if (!Zinc_expect_ptr(test, right2, "right2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right2->type, Ake_ast_type_number, "number 3");
    Zinc_expect_string(test, &right2->value, "3", "3");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_paren_mult(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("(5 * 2)", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* paren = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, paren, "ptr paren")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

    Ake_ast* mult = Ast_node_get(paren, 0);
    if (!Zinc_expect_ptr(test, mult, "ptr mult")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, mult->type, Ake_ast_type_mult, "mult mult");

    Ake_ast* left = Ast_node_get(mult, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &left->value, "5", "5");

    Ake_ast* right = Ast_node_get(mult, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &right->value, "2", "2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_paren_mult_mult(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("1 * (2 * 3)", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* mult = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, mult, "ptr mult")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, mult->type, Ake_ast_type_mult, "mult mult")) {
		return Zinc_assert();
	}

    Ake_ast* left = Ast_node_get(mult, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &left->value, "1", "1");

    Ake_ast* paren = Ast_node_get(mult, 1);
    if (!Zinc_expect_ptr(test, paren, "ptr paren")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

    Ake_ast* right = Ast_node_get(paren, 0);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_mult, "mult 2");

    Ake_ast* left2 = Ast_node_get(right, 0);
    if (!Zinc_expect_ptr(test, left2, "left 2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_number, "number 2");
    Zinc_expect_string(test, &left2->value, "2", "2");

    Ake_ast* right2 = Ast_node_get(right, 1);
    if (!Zinc_expect_ptr(test, right2, "right2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right2->type, Ake_ast_type_number, "number 3");
    Zinc_expect_string(test, &right2->value, "3", "3");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_paren_mult_mult2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("(1 * 2) * 3", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* mult = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, mult, "ptr mult")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, mult->type, Ake_ast_type_mult, "mult mult")) {
		return Zinc_assert();
	}

    Ake_ast* paren = Ast_node_get(mult, 0);
    if (!Zinc_expect_ptr(test, paren, "ptr paren")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

    Ake_ast* left = Ast_node_get(paren, 0);
    if (!Zinc_expect_ptr(test, left, "left")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left->type, Ake_ast_type_mult, "mult 2");

    Ake_ast* left2 = Ast_node_get(left, 0);
    if (!Zinc_expect_ptr(test, left2, "left2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_number, "number");
    Zinc_expect_string(test, &left2->value, "1", "1");

    Ake_ast* right = Ast_node_get(left, 1);
    if (!Zinc_expect_ptr(test, right, "right")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right->type, Ake_ast_type_number, "number 2");
    Zinc_expect_string(test, &right->value, "2", "2");

    Ake_ast* right2 = Ast_node_get(mult, 1);
    if (!Zinc_expect_ptr(test, right2, "right2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right2->type, Ake_ast_type_number, "number 3");
    Zinc_expect_string(test, &right2->value, "3", "3");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_comparison(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("var count: Int32 = 5; count == 10; count != 11.1; count <= 12; count >= 13", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_ast* cond0 = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, cond0, "ptr cond0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cond0->type, Ake_ast_type_equality, "equality cond0");

    Ake_type_use* cond0_tu = cond0->tu;
    if (!Zinc_expect_ptr(test, cond0_tu, "ptr cond0_tu")) {
		return Zinc_assert();
	}
    
    Ake_TypeDef* cond0_td = cond0_tu->td;
    if (!Zinc_expect_ptr(test, cond0_td, "ptr cond0_td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cond0_td->kind, AKE_TYPE_DEF_INTEGER, "integer cond0_td");
    Zinc_expect_string(test, &cond0_td->name, "Int32", "Int32 cond0_td");

    Ake_ast* left0 = Ast_node_get(cond0, 0);
    if (!Zinc_expect_ptr(test, left0, "ptr left0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left0->type, Ake_ast_type_id, "id left0");
    Zinc_expect_string(test, &left0->value, "count", "count left0");

    Ake_ast* right0 = Ast_node_get(cond0, 1);
    if (!Zinc_expect_ptr(test, right0, "ptr right0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right0->type, Ake_ast_type_number, "number right0");
    Zinc_expect_string(test, &right0->value, "10", "10 right0");

    Ake_ast* cond1 = Ast_node_get(cu.root, 2);
    if (!Zinc_expect_ptr(test, cond1, "ptr cond1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cond1->type, Ake_ast_type_not_equal, "not equal cond1");

    Ake_type_use* cond1_tu = cond1->tu;
    if (!Zinc_expect_ptr(test, cond1_tu, "ptr cond1_tu")) {
		return Zinc_assert();
	}

    Ake_TypeDef* cond1_td = cond1_tu->td;
    if (!Zinc_expect_ptr(test, cond1_td, "ptr cond0_td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cond1_td->kind, AKE_TYPE_DEF_INTEGER, "kind cond1_td");
    Zinc_expect_string(test, &cond1_td->name, "Int32", "name cond1_td");

    Ake_ast* left1 = Ast_node_get(cond1, 0);
    if (!Zinc_expect_ptr(test, left1, "ptr left1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left1->type, Ake_ast_type_id, "id left1");
    Zinc_expect_string(test, &left1->value, "count", "count left1");

    Ake_ast* right1 = Ast_node_get(cond1, 1);
    if (!Zinc_expect_ptr(test, right1, "ptr right1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right1->type, Ake_ast_type_number, "number right1");
    Zinc_expect_string(test, &right1->value, "11.1", "11.1 right1");

    Ake_ast* cond2 = Ast_node_get(cu.root, 3);
    if (!Zinc_expect_ptr(test, cond2, "ptr cond2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cond2->type, Ake_ast_type_less_than_or_equal, "less than or equal cond2");

    Ake_ast* left2 = Ast_node_get(cond2, 0);
    if (!Zinc_expect_ptr(test, left2, "ptr left2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left2->type, Ake_ast_type_id, "id left2");
    Zinc_expect_string(test, &left2->value, "count", "count left2");

    Ake_ast* right2 = Ast_node_get(cond2, 1);
    if (!Zinc_expect_ptr(test, right2, "ptr right2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right2->type, Ake_ast_type_number, "number right2");
    Zinc_expect_string(test, &right2->value, "12", "12 right2");

    Ake_ast* cond3 = Ast_node_get(cu.root, 4);
    if (!Zinc_expect_ptr(test, cond3, "ptr cond3")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cond3->type, Ake_ast_type_greater_than_or_equal, "greater than or equal cond3");

    Ake_ast* left3 = Ast_node_get(cond3, 0);
    if (!Zinc_expect_ptr(test, left3, "ptr left3")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, left3->type, Ake_ast_type_id, "id left3");
    Zinc_expect_string(test, &left3->value, "count", "count left3");

    Ake_ast* right3 = Ast_node_get(cond3, 1);
    if (!Zinc_expect_ptr(test, right3, "ptr right3")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, right3->type, Ake_ast_type_number, "number right3");
    Zinc_expect_string(test, &right3->value, "13", "13 right3");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_comparison_identity(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("true == true; true != true", &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* comp0 = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, comp0, "ptr comp0")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_ptr(test, comp0->tu, "ptr comp0->tu")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_ptr(test, comp0->tu->td, "ptr comp0->tu->td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, comp0->tu->td->kind, AKE_TYPE_DEF_BOOLEAN, "boolean comp0->tu->td->type");
    Zinc_expect_string(test, &comp0->tu->td->name, "Bool", "Bool comp0->tu->td->name");

    Ake_ast* comp1 = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, comp1, "ptr comp1")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_ptr(test, comp1->tu, "ptr comp->tu1")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_ptr(test, comp1->tu->td, "ptr comp1->tu->td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, comp1->tu->td->kind, AKE_TYPE_DEF_BOOLEAN, "boolean comp1->tu->td->type");
    Zinc_expect_string(test, &comp1->tu->td->name, "Bool", "Bool comp1->tu->td->name");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_comparison_error_no_term(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("100 <", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "expected term after comparison operator");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_comparison_error_left_no_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; foo() < 100", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "operand has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_comparison_error_right_no_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; 100 < foo()", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "operand has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_comparison_error_left_not_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("true < 100", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "comparison operand is not numeric");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_comparison_error_right_not_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("true < 100", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "comparison operand is not numeric");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_and(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: Bool; const b: Bool; a && b", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}

    Ake_ast* and = Ast_node_get(cu.root, 2);
    if (!Zinc_expect_ptr(test, and, "ptr and")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, and->type, Ake_ast_type_and, "and and");

    Ake_ast* a = Ast_node_get(and, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Ake_ast_type_id, "id a");
    Zinc_expect_string(test, &a->value, "a", "a a");

    Ake_ast* b = Ast_node_get(and, 1);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Ake_ast_type_id, "id b");
    Zinc_expect_string(test, &b->value, "b", "b b");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_or(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: Bool; const b: Bool; a || b", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* or = Ast_node_get(cu.root, 2);
    if (!Zinc_expect_ptr(test, or, "ptr or")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, or->type, Ake_ast_type_or, "or or");

    Ake_type_use* tu = or->tu;
    if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

    Ake_TypeDef* td = tu->td;
    if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, td->kind, AKE_TYPE_DEF_BOOLEAN, "boolean td");
    Zinc_expect_string(test, &td->name, "Bool", "Bool td");

    Ake_ast* a = Ast_node_get(or, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Ake_ast_type_id, "id a");
    Zinc_expect_string(test, &a->value, "a", "a a");

    Ake_ast* b = Ast_node_get(or, 1);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Ake_ast_type_id, "id b");
    Zinc_expect_string(test, &b->value, "b", "b b");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_or_or(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: Bool; const b: Bool; const c: Bool; a || b || c", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* or0 = Ast_node_get(cu.root, 3);
    if (!Zinc_expect_ptr(test, or0 , "ptr or0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, or0->type, Ake_ast_type_or, "or or0");

    Ake_ast* or1 = Ast_node_get(or0, 0);
    if (!Zinc_expect_ptr(test, or1, "ptr or1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, or1->type, Ake_ast_type_or, "or or1");

    Ake_ast* a = Ast_node_get(or1, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Ake_ast_type_id, "id a");
    Zinc_expect_string(test, &a->value, "a", "a a");

    Ake_ast* b = Ast_node_get(or1, 1);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Ake_ast_type_id, "id b");
    Zinc_expect_string(test, &b->value, "b", "b b");

    Ake_ast* c = Ast_node_get(or0, 1);
    if (!Zinc_expect_ptr(test, c, "ptr c")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c->type, Ake_ast_type_id, "id c");
    Zinc_expect_string(test, &c->value, "c", "c c");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_boolean_error_expected_term(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("true &&", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "expected term after && or ||");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_boolean_error_left_no_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    
    struct Ake_comp_unit cu;


    AkeUnit_parse_setup("fn foo() end; foo() && true", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "left-side operand of boolean operator has no type");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_boolean_error_right_no_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    
    struct Ake_comp_unit cu;


    AkeUnit_parse_setup("fn foo() end; true && foo()", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "operand of boolean operator has no type");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_boolean_error_left_not_boolean(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("1 && true", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "left-side expression of boolean operator is not boolean");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_boolean_error_right_not_boolean(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("true && 1", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "cu.valid");
    Zinc_expect_source_error(test, &cu.errors, "expression of boolean operator is not boolean");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_array_declare(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: [5]Int32\n"
               "a[0]\n",
               &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_array_subscript(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: [10]Int32; a[1]", &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* as = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, as, "ptr as")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, as->type, Ake_ast_type_array_subscript, "array-subscript as");

    Ake_type_use* tu = as->tu;
    if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

    Ake_TypeDef* td = tu->td;
    if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, td->kind, AKE_TYPE_DEF_INTEGER, "integer td");
    Zinc_expect_string(test, &td->name, "Int32", "Int32 td");

    Ake_ast* name = Ast_node_get(as, 0);
    if (!Zinc_expect_ptr(test, name, "ptr name")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, name->type, Ake_ast_type_id, "id name");
    Zinc_expect_string(test, &name->value, "a", "a name");

    Ake_ast* index = Ast_node_get(as, 1);
    if (!Zinc_expect_ptr(test, index, "ptr index")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, index->type, Ake_ast_type_number, "number index");
    Zinc_expect_string(test, &index->value, "1", "1 index");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_array_subscript2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: [10][10]Int32; a[0][1]", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_ast* a = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, a->type, Ake_ast_type_array_subscript, "array-subscript a")) {
		return Zinc_assert();
	}

    Ake_type_use* a_tu = a->tu;
    if (!Zinc_expect_ptr(test, a_tu, "ptr tu")) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, a_tu->is_array, "is array to");

    Ake_TypeDef* td = a_tu->td;
    if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, td->kind, AKE_TYPE_DEF_INTEGER, "integer td");
    Zinc_expect_string(test, &td->name, "Int32", "Int32 td");

    Ake_ast* b = Ast_node_get(a, 0);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Ake_ast_type_array_subscript, "array_subscript b");

    Ake_ast* c = Ast_node_get(b, 0);
    if (!Zinc_expect_ptr(test, c, "ptr c")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c->type, Ake_ast_type_id, "id c");
    Zinc_expect_string(test, &c->value, "a", "value a");

    Ake_ast* d = Ast_node_get(b, 1);
    if (!Zinc_expect_ptr(test, d, "ptr d")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, d->type, Ake_ast_type_number, "type d");
    Zinc_expect_string(test, &d->value, "0", "value d");

    Ake_ast* e = Ast_node_get(a, 1);
    if (!Zinc_expect_ptr(test, e, "ptr e")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, d->type, Ake_ast_type_number, "number e");
    Zinc_expect_string(test, &e->value, "1", "value e");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_array_subscript3(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: [5]Int32; const b: Int32; a[b]", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_ast* a = Ast_node_get(cu.root, 2);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Ake_ast_type_array_subscript, "type a");

    Ake_type_use* tu = a->tu;
    if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

    Ake_TypeDef* td = tu->td;
    if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, td->kind, AKE_TYPE_DEF_INTEGER, "integer td");
    Zinc_expect_string(test, &td->name, "Int32", "Int32 td");

    Ake_ast* b = Ast_node_get(a, 0);
    Zinc_expect_int_equal(test, b->type, Ake_ast_type_id, "type b");
    Zinc_expect_string(test, &b->value, "a", "a b");

    Ake_ast* c = Ast_node_get(a, 1);
    Zinc_expect_int_equal(test, c->type, Ake_ast_type_id, "type c");
    Zinc_expect_string(test, &c->value, "b", "value c");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expr_array_subscript_3d(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const mut x: [2][3][4 const]Int32 = \n"
               "[\n"
               "  [\n"
               "    [1, 2, 3, 4],\n"
               "    [5, 6, 7, 8],\n"
               "    [9, 10, 11, 12]\n"
               "  ],\n"
               "  [\n"
               "    [13, 14, 15, 16],\n"
               "    [17, 18, 19, 20],\n"
               "    [21, 22, 23, 24]\n"
               "  ]\n"
               "]\n"
               "x[1][1][3]\n",
               &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_ast* const_ = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, const_, "per const_")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, const_->type, Ake_ast_type_const, "type const_");

    Ake_ast* const_type = Ast_node_get(const_, 1);
    if (!Zinc_expect_ptr(test, const_type, "ptr const_type")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, const_type->type, Ake_ast_type_type, "type const_type");

    Ake_ast* a = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    if (!Zinc_expect_int_equal(test, a->type, Ake_ast_type_array_subscript, "type a")) {
		return Zinc_assert();
	}

    Ake_type_use* a_tu = a->tu;
    if (!Zinc_expect_ptr(test, a_tu, "ptr a_tu")) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, a_tu->is_mut, "is_mut a_tu");
    Zinc_expect_false(test, a_tu->is_array, "is_array a_tu");
    Zinc_expect_size_t_equal(test, a_tu->dim.count, 0, "dim.count a_tu");

    Ake_TypeDef* a_td = a_tu->td;
    if (!Zinc_expect_ptr(test, a_td, "ptr a_td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a_td->kind, AKE_TYPE_DEF_INTEGER, "type a_td");
    Zinc_expect_string(test, &a_td->name, "Int32", "name a_td");

    Ake_ast* b = Ast_node_get(a, 0);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Ake_ast_type_array_subscript, "type b");

    Ake_type_use* b_tu = b->tu;
    if (!Zinc_expect_ptr(test, b_tu, "ptr b_tu")) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, b_tu->is_array, "is_array b_tu");
    Zinc_expect_size_t_equal(test, b_tu->dim.count, 1, "dim.count b_tu");

    Ake_type_dimension* b_dim0 = (Ake_type_dimension*)ZINC_VECTOR_PTR(&b_tu->dim, 0);
    if (!Zinc_expect_ptr(test, b_dim0, "ptr b_dim0")) {
		return Zinc_assert();
	}
    Zinc_expect_size_t_equal(test, b_dim0->size, 4, "size b_dim0");
    Zinc_expect_size_t_equal(test, b_dim0->option, Ake_array_element_const, "option b_dim0");

    Ake_ast* c = Ast_node_get(b, 0);
    if (!Zinc_expect_ptr(test, c, "ptr c")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c->type, Ake_ast_type_array_subscript, "type c");

    Ake_type_use* c_tu = c->tu;
    if (!Zinc_expect_ptr(test, c_tu, "ptr c_tu")) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, c_tu->is_mut, "is_mut c_tu");
    Zinc_expect_true(test, c_tu->is_array, "is_array c_tu");
    Zinc_expect_size_t_equal(test, c_tu->dim.count, 2, "dim.count c_tu");

    Ake_type_dimension* c_dim0 = (Ake_type_dimension*)ZINC_VECTOR_PTR(&c_tu->dim, 0);
    if (!Zinc_expect_ptr(test, c_dim0, "ptr c_dim0")) {
		return Zinc_assert();
	}
    Zinc_expect_size_t_equal(test, c_dim0->size, 3, "size c_dim0");
    Zinc_expect_int_equal(test, c_dim0->option, Ake_array_element_default, "option c_dim0");

    Ake_type_dimension* c_dim1 = (Ake_type_dimension*)ZINC_VECTOR_PTR(&c_tu->dim, 1);
    if (!Zinc_expect_ptr(test, c_dim1, "ptr c_dim1")) {
		return Zinc_assert();
	}
    Zinc_expect_size_t_equal(test, c_dim1->size, 4, "size c_dim1");
    Zinc_expect_int_equal(test, c_dim1->option, Ake_array_element_const, "option c_dim1");

    Ake_ast* d = Ast_node_get(c, 0);
    if (!Zinc_expect_ptr(test, d, "ptr d")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, d->type, Ake_ast_type_id, "type d");

    Ake_type_use* d_tu = d->tu;
    if (!Zinc_expect_ptr(test, d_tu, "ptr d_tu")) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, d_tu->is_mut, "is_mut d_tu");
    Zinc_expect_true(test, d_tu->is_array, "is_array d_tu");
    Zinc_expect_size_t_equal(test, d_tu->dim.count, 3, "dim.count d_tu");

    Ake_type_dimension* d_dim0 = (Ake_type_dimension*)ZINC_VECTOR_PTR(&d_tu->dim, 0);
    if (!Zinc_expect_ptr(test, d_dim0, "ptr d_dim0")) {
		return Zinc_assert();
	}
    Zinc_expect_size_t_equal(test, d_dim0->size, 2, "size d_dim0");
    Zinc_expect_int_equal(test, d_dim0->option, Ake_array_element_default, "option d_dim0");

    Ake_type_dimension* d_dim1 = (Ake_type_dimension*)ZINC_VECTOR_PTR(&d_tu->dim, 1);
    if (!Zinc_expect_ptr(test, d_dim1, "ptr d_dim1")) {
		return Zinc_assert();
	}
    Zinc_expect_size_t_equal(test, d_dim1->size, 3, "size d_dim1");
    Zinc_expect_int_equal(test, d_dim1->option, Ake_array_element_default, "option d_dim1");

    Ake_type_dimension* d_dim2 = (Ake_type_dimension*)ZINC_VECTOR_PTR(&d_tu->dim, 2);
    if (!Zinc_expect_ptr(test, d_dim2, "ptr d_dim2")) {
		return Zinc_assert();
	}
    Zinc_expect_size_t_equal(test, d_dim2->size, 4, "size d_dim2");
    Zinc_expect_int_equal(test, d_dim2->option, Ake_array_element_const, "option d_dim2");

    Ake_ast* e = Ast_node_get(c, 1);
    if (!Zinc_expect_ptr(test, e, "ptr e")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, e->type, Ake_ast_type_number, "type e");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_subscript_error_no_type(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; foo()[1]", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "expression has subscript but has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_subscript_error_not_array(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: Int32; a[1]", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "expression has subscript but is not an array or slice");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_subscript_error_expected_right_square_bracket(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: [10]Int32; a[1", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "expected right-square-bracket");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_assign_string(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("var mut a: [6 const]Nat8; a = \"hello\"", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts root");

    Ake_ast* assign = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, assign, "ptr assign")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign->type, Ake_ast_type_assign, "assign assign");

    Ake_type_use* tu = assign->tu;
    if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

    Ake_TypeDef* td = tu->td;
    if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, td->kind, AKE_TYPE_DEF_NATURAL, "type td");
    Zinc_expect_string(test, &td->name, "Nat8", "name td");
         
    Ake_ast* lhv = Ast_node_get(assign, 0);
    if (!Zinc_expect_ptr(test, lhv, "ptr lhv")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lhv->type, Ake_ast_type_id, "id lhv");
    Zinc_expect_string(test, &lhv->value, "a", "a lhv");

    Ake_ast* rhv = Ast_node_get(assign, 1);
    if (!Zinc_expect_ptr(test, rhv, "ptr rhv")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, rhv->type, Ake_ast_type_string, "string rhv");
    Zinc_expect_string(test, &rhv->value, "hello", "hello rhv");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_assign_multiple(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("var mut a: Int32; var mut b: Int32; var mut c: Int32; a = b = c = 0", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* assign = Ast_node_get(cu.root, 3);
    if (!Zinc_expect_ptr(test, assign, "ptr assign")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign->type, Ake_ast_type_assign, "assign assign");

    Ake_type_use* assign_tu = assign->tu;
    if (!Zinc_expect_ptr(test, assign_tu, "ptr assign_tu")) {
		return Zinc_assert();
	}

    Ake_TypeDef* assign_td = assign_tu->td;
    if (!Zinc_expect_ptr(test, assign_td, "ptr assign_td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign_td->kind, AKE_TYPE_DEF_INTEGER, "integer assign_td");
    Zinc_expect_string(test, &assign_td->name, "Int32", "Int32 assign_td");

    Ake_ast* assign0 = Ast_node_get(assign, 0);
    if (!Zinc_expect_ptr(test, assign0, "ptr lhv0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign0->type, Ake_ast_type_id, "id assign0");
    Zinc_expect_string(test, &assign0->value, "a", "assign0 value");

    Ake_ast* assign1 = Ast_node_get(assign, 1);
    if (!Zinc_expect_ptr(test, assign1, "ptr assign1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign1->type, Ake_ast_type_id, "assign assign1");
    Zinc_expect_string(test, &assign1->value, "b", "assign1 value");

    Ake_ast* assign2 = Ast_node_get(assign, 2);
    if (!Zinc_expect_ptr(test, assign2, "ptr assign2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign2->type, Ake_ast_type_id, "assign assign2");
    Zinc_expect_string(test, &assign2->value, "c", "assign2 value");

    Ake_ast* assign3 = Ast_node_get(assign, 3);
    if (!Zinc_expect_ptr(test, assign3, "ptr assign3")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign3->type, Ake_ast_type_number, "assign assign3");
    Zinc_expect_string(test, &assign3->value, "0", "assign3 value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expr_assignment_eseq_error_eseq_count(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("var mut a: Int32; var mut b: Int32; var mut c: Int32; a, b, c = 1, 2", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "assignment sequence counts do not match");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_const_assign_error_term(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: Bool =", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "expected expression");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_assign_error_no_value_right(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; const a: [10]Nat8 = foo()", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "cannot assign with operand that has no value");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_assign_error_not_compatible(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const x: Int32 = true", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "values in assignment are not compatible");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_assign_error_lvalue(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("true = true", &cu);
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "invalid lvalue");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expr_newline_assignment(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("var mut a: Int32; a =\n1", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_ast* op = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, op, "ptr op")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, op->type, Ake_ast_type_assign, "assign op");

    Ake_ast* one = Ast_node_get(op, 0);
    if (!Zinc_expect_ptr(test, one, "one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->type, Ake_ast_type_id, "id one");
    Zinc_expect_string(test, &one->value, "a", "a one");

    Ake_ast* two = Ast_node_get(op, 1);
    if (!Zinc_expect_ptr(test, two, "two")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, two->type, Ake_ast_type_number, "number two");
    Zinc_expect_string(test, &two->value, "1", "1 two");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expr_newline_boolean(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("true &&\nfalse", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_ast* op = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, op, "ptr add")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, op->type, Ake_ast_type_and, "less than op");

    Ake_ast* one = Ast_node_get(op, 0);
    if (!Zinc_expect_ptr(test, one, "one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->type, Ake_ast_type_boolean, "boolean one");
    Zinc_expect_string(test, &one->value, "true", "true");

    Ake_ast* two = Ast_node_get(op, 1);
    if (!Zinc_expect_ptr(test, two, "two")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, two->type, Ake_ast_type_boolean, "boolean two");
    Zinc_expect_string(test, &two->value, "false", "false");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expr_newline_comparison(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("1 <\n2", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_ast* op = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, op, "ptr add")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, op->type, Ake_ast_type_less_than, "less than op");

    Ake_ast* one = Ast_node_get(op, 0);
    if (!Zinc_expect_ptr(test, one, "one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->type, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1");

    Ake_ast* two = Ast_node_get(op, 1);
    if (!Zinc_expect_ptr(test, two, "two")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, two->type, Ake_ast_type_number, "number two");
    Zinc_expect_string(test, &two->value, "2", "2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expr_newline_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("1 +\n2", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_ast* add = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, add, "ptr add")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, add->type, Ake_ast_type_plus, "plus add");

    Ake_ast* one = Ast_node_get(add, 0);
    if (!Zinc_expect_ptr(test, one, "one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->type, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1");

    Ake_ast* two = Ast_node_get(add, 1);
    if (!Zinc_expect_ptr(test, two, "two")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, two->type, Ake_ast_type_number, "number two");
    Zinc_expect_string(test, &two->value, "2", "2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expr_newline_mult(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("1 *\n2", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_ast* op = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, op, "ptr op")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, op->type, Ake_ast_type_mult, "plus op");

    Ake_ast* one = Ast_node_get(op, 0);
    if (!Zinc_expect_ptr(test, one, "one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->type, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1");

    Ake_ast* two = Ast_node_get(op, 1);
    if (!Zinc_expect_ptr(test, two, "two")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, two->type, Ake_ast_type_number, "number two");
    Zinc_expect_string(test, &two->value, "2", "2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expr_newline_power(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("1^\n2", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_ast* op = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, op, "ptr op")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, op->type, Ake_ast_type_power, "plus op");

    Ake_ast* one = Ast_node_get(op, 0);
    if (!Zinc_expect_ptr(test, one, "one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->type, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1");

    Ake_ast* two = Ast_node_get(op, 1);
    if (!Zinc_expect_ptr(test, two, "two")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, two->type, Ake_ast_type_number, "number two");
    Zinc_expect_string(test, &two->value, "2", "2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expr_newline_subscript(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: [1]Int32; a[\n0\n]", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_ast* subscript = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, subscript, "ptr subscript")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, subscript->type, Ake_ast_type_array_subscript, "array_subscript subscript");

    Ake_ast* array = Ast_node_get(subscript, 0);
    if (!Zinc_expect_ptr(test, array, "array")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, array->type, Ake_ast_type_id, "id array");
    Zinc_expect_string(test, &array->value, "a", "a");

    Ake_ast* zero = Ast_node_get(subscript, 1);
    if (!Zinc_expect_ptr(test, zero, "zero")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, zero->type, Ake_ast_type_number, "number zero");
    Zinc_expect_string(test, &zero->value, "0", "0");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expr_newline_function_call(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(a: Int32, b: Int32)->Int32 a+b end; foo(\n1,\n2\n)", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_ast* call = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, call, "ptr call")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, call->type, Ake_ast_type_call, "call call");

    Ake_ast* foo = Ast_node_get(call, 0);
    if (!Zinc_expect_ptr(test, foo, "ptr foo")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, foo->type, Ake_ast_type_id, "id foo");
    Zinc_expect_string(test, &foo->value, "foo", "foo");

    Ake_ast* cseq = Ast_node_get(call, 1);
    if (!Zinc_expect_ptr(test, cseq, "ptr cseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq->type, Ake_ast_type_cseq, "cseq cseq");

    Ake_ast* one = Ast_node_get(cseq, 0);
    if (!Zinc_expect_ptr(test, one, "ptr one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->type, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1");

    Ake_ast* two = Ast_node_get(cseq, 1);
    if (!Zinc_expect_ptr(test, two, "ptr two")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, two->type, Ake_ast_type_number, "number two");
    Zinc_expect_string(test, &two->value, "2", "2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expr_assign_eseq(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("var mut a: Int32\n"
               "var mut b: Int32\n"
               "var mut c: Int32\n"
               "a,b,c = 1,2,3", &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* assign = Ast_node_get(cu.root, 3);
    if (!Zinc_expect_ptr(test, assign, "ptr assign")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign->type, Ake_ast_type_assign, "assign assign");

    Ake_ast* lvalue = Ast_node_get(assign, 0);
    if (!Zinc_expect_ptr(test, lvalue, "ptr lvalue")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lvalue->type, Ake_ast_type_eseq, "eseq lvalue");

    Ake_ast* a = Ast_node_get(lvalue, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Ake_ast_type_id, "id a");
    Zinc_expect_string(test, &a->value, "a", "a");

    Ake_ast* b = Ast_node_get(lvalue, 1);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Ake_ast_type_id, "id b");
    Zinc_expect_string(test, &b->value, "b", "b");

    Ake_ast* c = Ast_node_get(lvalue, 2);
    if (!Zinc_expect_ptr(test, c, "ptr c")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c->type, Ake_ast_type_id, "id c");
    Zinc_expect_string(test, &c->value, "c", "c");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expr_error_lvalue(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("var a: Int32\n"
               "1 = 1", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_source_error(test, &cu.errors, "invalid lvalue");
    Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expr_error_eseq_lvalue(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("var a: Int32\n"
               "var b: Int32\n"
               "var c: Int32\n"
               "1,2,3 = 1,2,3", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_source_error(test, &cu.errors, "invalid lvalue");
    Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_array_string_return(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(a: Int32, b: Int32, c: Int32)->[6 const]Nat8\n"
               "  \"hello\"\n"
               "end\n"
               "foo(1, 2, 3)\n",
               &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_expression(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeUnit_parse_blank);
        Zinc_test_register(test, AkeUnit_parse_add);
        Zinc_test_register(test, AkeUnit_parse_add_error_expected_term);
        Zinc_test_register(test, AkeUnit_parse_add_error_left_no_value);
        Zinc_test_register(test, AkeUnit_parse_add_error_left_not_numeric);
        Zinc_test_register(test, AkeUnit_parse_add_error_right_no_value);
        Zinc_test_register(test, AkeUnit_parse_add_error_right_not_numeric);
        Zinc_test_register(test, AkeUnit_parse_add_mixed_types);
        Zinc_test_register(test, AkeUnit_parse_add_positive);
        Zinc_test_register(test, AkeUnit_parse_add_negative);
        Zinc_test_register(test, AkeUnit_parse_sub);
        Zinc_test_register(test, AkeUnit_parse_sub_positive);
        Zinc_test_register(test, AkeUnit_parse_sub_negative);
        Zinc_test_register(test, AkeUnit_parse_mult);
        Zinc_test_register(test, AkeUnit_parse_mult_error_expected_term);
        Zinc_test_register(test, AkeUnit_parse_mult_error_left_no_value);
        Zinc_test_register(test, AkeUnit_parse_mult_error_left_not_numeric);
        Zinc_test_register(test, AkeUnit_parse_mult_error_right_no_value);
        Zinc_test_register(test, AkeUnit_parse_mult_error_right_not_numeric);
        Zinc_test_register(test, AkeUnit_parse_mult_positive);
        Zinc_test_register(test, AkeUnit_parse_mult_negative);
        Zinc_test_register(test, AkeUnit_parse_divide);
        Zinc_test_register(test, AkeUnit_parse_add_add);
        Zinc_test_register(test, AkeUnit_parse_mult_mult);
        Zinc_test_register(test, AkeUnit_parse_add_mult);
        Zinc_test_register(test, AkeUnit_parse_mult_add);
        Zinc_test_register(test, AkeUnit_parse_power);
        Zinc_test_register(test, AkeUnit_parse_power_error_expected_term);
        Zinc_test_register(test, AkeUnit_parse_power_error_left_no_value);
        Zinc_test_register(test, AkeUnit_parse_power_error_left_not_numeric);
        Zinc_test_register(test, AkeUnit_parse_power_error_right_no_value);
        Zinc_test_register(test, AkeUnit_parse_power_error_right_not_numeric);
        Zinc_test_register(test, AkeUnit_parse_paren_add);
        Zinc_test_register(test, AkeUnit_parse_paren_add2);
        Zinc_test_register(test, AkeUnit_parse_paren_add3);
        Zinc_test_register(test, AkeUnit_parse_paren_add_add);
        Zinc_test_register(test, AkeUnit_parse_paren_add_add2);
        Zinc_test_register(test, AkeUnit_parse_paren_mult);
        Zinc_test_register(test, AkeUnit_parse_paren_mult_mult);
        Zinc_test_register(test, AkeUnit_parse_paren_mult_mult2);
        Zinc_test_register(test, AkeUnit_parse_comparison);
        Zinc_test_register(test, AkeUnit_parse_comparison_identity);
        Zinc_test_register(test, AkeUnit_parse_comparison_error_no_term);
        Zinc_test_register(test, AkeUnit_parse_comparison_error_left_no_value);
        Zinc_test_register(test, AkeUnit_parse_comparison_error_right_no_value);
        Zinc_test_register(test, AkeUnit_parse_comparison_error_left_not_numeric);
        Zinc_test_register(test, AkeUnit_parse_comparison_error_right_not_numeric);
        Zinc_test_register(test, AkeUnit_parse_and);
        Zinc_test_register(test, AkeUnit_parse_or);
        Zinc_test_register(test, AkeUnit_parse_or_or);
        Zinc_test_register(test, AkeUnit_parse_boolean_error_expected_term);
        Zinc_test_register(test, AkeUnit_parse_boolean_error_left_no_value);
        Zinc_test_register(test, AkeUnit_parse_boolean_error_right_no_value);
        Zinc_test_register(test, AkeUnit_parse_boolean_error_left_not_boolean);
        Zinc_test_register(test, AkeUnit_parse_boolean_error_right_not_boolean);
        Zinc_test_register(test, AkeUnit_parse_array_declare);
        Zinc_test_register(test, AkeUnit_parse_array_subscript);
        Zinc_test_register(test, AkeUnit_parse_array_subscript2);
        Zinc_test_register(test, AkeUnit_parse_array_subscript3);
        Zinc_test_register(test, AkeUnit_parse_expr_array_subscript_3d);
        Zinc_test_register(test, AkeUnit_parse_subscript_error_no_type);
        Zinc_test_register(test, AkeUnit_parse_subscript_error_not_array);
        Zinc_test_register(test, AkeUnit_parse_subscript_error_expected_right_square_bracket);
        Zinc_test_register(test, AkeUnit_parse_assign_string);
        Zinc_test_register(test, AkeUnit_parse_assign_multiple);
        Zinc_test_register(test, AkeUnit_parse_expr_assignment_eseq_error_eseq_count);
        Zinc_test_register(test, AkeUnit_parse_const_assign_error_term);
        Zinc_test_register(test, AkeUnit_parse_assign_error_no_value_right);
        Zinc_test_register(test, AkeUnit_parse_assign_error_not_compatible);
        Zinc_test_register(test, AkeUnit_parse_assign_error_lvalue);
        Zinc_test_register(test, AkeUnit_parse_expr_newline_assignment);
        Zinc_test_register(test, AkeUnit_parse_expr_newline_boolean);
        Zinc_test_register(test, AkeUnit_parse_expr_newline_comparison);
        Zinc_test_register(test, AkeUnit_parse_expr_newline_add);
        Zinc_test_register(test, AkeUnit_parse_expr_newline_mult);
        Zinc_test_register(test, AkeUnit_parse_expr_newline_power);
        Zinc_test_register(test, AkeUnit_parse_expr_newline_subscript);
        Zinc_test_register(test, AkeUnit_parse_expr_newline_function_call);
        Zinc_test_register(test, AkeUnit_parse_expr_assign_eseq);
        Zinc_test_register(test, AkeUnit_parse_expr_error_lvalue);
        Zinc_test_register(test, AkeUnit_parse_expr_error_eseq_lvalue);
        Zinc_test_register(test, AkeUnit_parse_array_string_return);

        return;
    }

    Zinc_test_perform(test);
}