#include <stdbool.h>
#include "unit_parse.h"
#include "akela/ast.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

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

    Ake_Ast* add = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, add, "ptr add")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, add->kind, Ake_ast_type_plus, "plus add");

    Ake_Ast* one = Ake_ast_get(add, 0);
    if (!Zinc_expect_ptr(test, one, "one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->kind, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1");

    Ake_Ast* two = Ake_ast_get(add, 1);
    if (!Zinc_expect_ptr(test, two, "two")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, two->kind, Ake_ast_type_number, "number two");
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

    Ake_Ast* op = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, op, "ptr op")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, op->kind, Ake_ast_type_mult, "plus op");

    Ake_Ast* one = Ake_ast_get(op, 0);
    if (!Zinc_expect_ptr(test, one, "one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->kind, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1");

    Ake_Ast* two = Ake_ast_get(op, 1);
    if (!Zinc_expect_ptr(test, two, "two")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, two->kind, Ake_ast_type_number, "number two");
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

    Ake_Ast* op = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, op, "ptr op")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, op->kind, Ake_ast_type_power, "plus op");

    Ake_Ast* one = Ake_ast_get(op, 0);
    if (!Zinc_expect_ptr(test, one, "one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->kind, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1");

    Ake_Ast* two = Ake_ast_get(op, 1);
    if (!Zinc_expect_ptr(test, two, "two")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, two->kind, Ake_ast_type_number, "number two");
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

    Ake_Ast* subscript = Ake_ast_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, subscript, "ptr subscript")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, subscript->kind, Ake_ast_type_array_subscript, "array_subscript subscript");

    Ake_Ast* array = Ake_ast_get(subscript, 0);
    if (!Zinc_expect_ptr(test, array, "array")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, array->kind, Ake_ast_type_id, "id array");
    Zinc_expect_string(test, &array->value, "a", "a");

    Ake_Ast* zero = Ake_ast_get(subscript, 1);
    if (!Zinc_expect_ptr(test, zero, "zero")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, zero->kind, Ake_ast_type_number, "number zero");
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

    Ake_Ast* call = Ake_ast_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, call, "ptr call")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, call->kind, Ake_ast_type_call, "call call");

    Ake_Ast* foo = Ake_ast_get(call, 0);
    if (!Zinc_expect_ptr(test, foo, "ptr foo")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, foo->kind, Ake_ast_type_id, "id foo");
    Zinc_expect_string(test, &foo->value, "foo", "foo");

    Ake_Ast* cseq = Ake_ast_get(call, 1);
    if (!Zinc_expect_ptr(test, cseq, "ptr cseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq->kind, Ake_ast_type_cseq, "cseq cseq");

    Ake_Ast* one = Ake_ast_get(cseq, 0);
    if (!Zinc_expect_ptr(test, one, "ptr one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->kind, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1");

    Ake_Ast* two = Ake_ast_get(cseq, 1);
    if (!Zinc_expect_ptr(test, two, "ptr two")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, two->kind, Ake_ast_type_number, "number two");
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