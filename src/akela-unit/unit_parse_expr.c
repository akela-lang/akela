#include <stdbool.h>
#include "unit_parse.h"
#include "akela/ast.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_expr_assign_eseq(Zinc_test* test)
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
               "a,b,c = 1,2,3", &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_Ast* assign = Ake_ast_get(cu.root, 3);
    if (!Zinc_expect_ptr(test, assign, "ptr assign")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign->kind, Ake_ast_type_assign, "assign assign");

    Ake_Ast* lvalue = Ake_ast_get(assign, 0);
    if (!Zinc_expect_ptr(test, lvalue, "ptr lvalue")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lvalue->kind, Ake_ast_type_eseq, "eseq lvalue");

    Ake_Ast* a = Ake_ast_get(lvalue, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->kind, Ake_ast_type_id, "id a");
    Zinc_expect_string(test, &a->value, "a", "a");

    Ake_Ast* b = Ake_ast_get(lvalue, 1);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->kind, Ake_ast_type_id, "id b");
    Zinc_expect_string(test, &b->value, "b", "b");

    Ake_Ast* c = Ake_ast_get(lvalue, 2);
    if (!Zinc_expect_ptr(test, c, "ptr c")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c->kind, Ake_ast_type_id, "id c");
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

        Zinc_test_register(test, AkeUnit_parse_expr_assign_eseq);
        Zinc_test_register(test, AkeUnit_parse_expr_error_lvalue);
        Zinc_test_register(test, AkeUnit_parse_expr_error_eseq_lvalue);
        Zinc_test_register(test, AkeUnit_parse_array_string_return);

        return;
    }

    Zinc_test_perform(test);
}