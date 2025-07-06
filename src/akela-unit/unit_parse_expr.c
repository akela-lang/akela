#include <stdbool.h>
#include "unit_parse.h"
#include "akela/ast.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_assign_string(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("var a: [6 const]Nat8; a = \"hello\"", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts root");

    Ake_Ast* assign = Ake_ast_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, assign, "ptr assign")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign->kind, Ake_ast_type_assign, "assign assign");

    Ake_Type* type = assign->type;
    if (!Zinc_expect_ptr(test, type, "ptr type")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type->kind, AKE_TYPE_ARRAY, "type type");
	Zinc_expect_true(test, type->data.array.is_const, "is_const type");
    Zinc_expect_uint8_t_equal(test, type->data.array.dim, 6, "dim type");

	Ake_Type* type2 = type->data.array.type;
	if (!Zinc_expect_ptr(test, type2, "ptr type2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, type2->kind, AKE_TYPE_NATURAL, "kind type2");
	Zinc_expect_string(test, &type2->name, "Nat8", "name type2");

    Ake_Ast* lhv = Ake_ast_get(assign, 0);
    if (!Zinc_expect_ptr(test, lhv, "ptr lhv")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lhv->kind, Ake_ast_type_id, "id lhv");
    Zinc_expect_string(test, &lhv->value, "a", "a lhv");

    Ake_Ast* rhv = Ake_ast_get(assign, 1);
    if (!Zinc_expect_ptr(test, rhv, "ptr rhv")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, rhv->kind, Ake_ast_type_string, "string rhv");
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

    AkeUnit_parse_setup("var a: Int32; var b: Int32; var c: Int32; a = b = c = 0", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
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

    Ake_Type* assign_type = assign->type;
    if (!Zinc_expect_ptr(test, assign_type, "ptr assign_tu")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign_type->kind, AKE_TYPE_INTEGER, "integer assign_td");
    Zinc_expect_string(test, &assign_type->name, "Int32", "Int32 assign_td");

    Ake_Ast* assign0 = Ake_ast_get(assign, 0);
    if (!Zinc_expect_ptr(test, assign0, "ptr lhv0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign0->kind, Ake_ast_type_id, "id assign0");
    Zinc_expect_string(test, &assign0->value, "a", "assign0 value");

    Ake_Ast* assign1 = Ake_ast_get(assign, 1);
    if (!Zinc_expect_ptr(test, assign1, "ptr assign1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign1->kind, Ake_ast_type_id, "assign assign1");
    Zinc_expect_string(test, &assign1->value, "b", "assign1 value");

    Ake_Ast* assign2 = Ake_ast_get(assign, 2);
    if (!Zinc_expect_ptr(test, assign2, "ptr assign2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign2->kind, Ake_ast_type_id, "assign assign2");
    Zinc_expect_string(test, &assign2->value, "c", "assign2 value");

    Ake_Ast* assign3 = Ake_ast_get(assign, 3);
    if (!Zinc_expect_ptr(test, assign3, "ptr assign3")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign3->kind, Ake_ast_type_number, "assign assign3");
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

    AkeUnit_parse_setup("var a: Int32; var b: Int32; var c: Int32; a, b, c = 1, 2", &cu);
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

    AkeUnit_parse_setup("var a: Int32; a =\n1", &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_Ast* op = Ake_ast_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, op, "ptr op")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, op->kind, Ake_ast_type_assign, "assign op");

    Ake_Ast* one = Ake_ast_get(op, 0);
    if (!Zinc_expect_ptr(test, one, "one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->kind, Ake_ast_type_id, "id one");
    Zinc_expect_string(test, &one->value, "a", "a one");

    Ake_Ast* two = Ake_ast_get(op, 1);
    if (!Zinc_expect_ptr(test, two, "two")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, two->kind, Ake_ast_type_number, "number two");
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

    Ake_Ast* op = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, op, "ptr add")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, op->kind, Ake_ast_type_and, "less than op");

    Ake_Ast* one = Ake_ast_get(op, 0);
    if (!Zinc_expect_ptr(test, one, "one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->kind, Ake_ast_type_boolean, "boolean one");
    Zinc_expect_string(test, &one->value, "true", "true");

    Ake_Ast* two = Ake_ast_get(op, 1);
    if (!Zinc_expect_ptr(test, two, "two")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, two->kind, Ake_ast_type_boolean, "boolean two");
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

    Ake_Ast* op = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, op, "ptr add")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, op->kind, Ake_ast_type_less_than, "less than op");

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