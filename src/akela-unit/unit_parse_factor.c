#include <stdbool.h>
#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "unit_parse.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

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
