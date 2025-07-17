#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "unit_parse.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_stmts4(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const x: Int32; const y: Int32; const z: Int32; x+1; 5-4; y+z", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	if (!Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid")) {
		return Zinc_assert();
	}

	if (!Zinc_expect_ptr(test, cu.root, "cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts");

	Ake_Ast* a = Ake_ast_get(cu.root, 3);
	if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a->kind, Ake_ast_type_plus, "plus a");

	Ake_Ast* b = Ake_ast_get(a, 0);
	if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, b->kind, Ake_ast_type_id, "id b");
	Zinc_expect_string(test, &b->value, "x", "x b");

	Ake_Ast* c = Ake_ast_get(a, 1);
	if (!Zinc_expect_ptr(test, c, "ptr c")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, c->kind, Ake_ast_type_number, "number c");
	Zinc_expect_string(test, &c->value, "1", "1 c");

	Ake_Ast* d = Ake_ast_get(cu.root, 4);
	if (!Zinc_expect_ptr(test, d, "ptr d")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, d->kind, Ake_ast_type_minus, "minus d");

	Ake_Ast* e = Ake_ast_get(d, 0);
	if (!Zinc_expect_ptr(test, e, "ptr e")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, e->kind, Ake_ast_type_number, "number e");
	Zinc_expect_string(test, &e->value, "5", "5 b");

	Ake_Ast* f = Ake_ast_get(d, 1);
	if (!Zinc_expect_ptr(test, f, "ptr f")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, f->kind, Ake_ast_type_number, "number f");
	Zinc_expect_string(test, &f->value, "4", "4 c");

	Ake_Ast* g = Ake_ast_get(cu.root, 5);
	if (!Zinc_expect_ptr(test, g, "ptr g")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, g->kind, Ake_ast_type_plus, "plus g");

	Ake_Ast* h = Ake_ast_get(g, 0);
	if (!Zinc_expect_ptr(test, h, "ptr h")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, h->kind, Ake_ast_type_id, "id h");
	Zinc_expect_string(test, &h->value, "y", "y h");

	Ake_Ast* i = Ake_ast_get(g, 1);
	if (!Zinc_expect_ptr(test, i, "ptr i")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, i->kind, Ake_ast_type_id, "id i");
	Zinc_expect_string(test, &i->value, "z", "z c");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
void AkeUnit_parse_stmts5(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("if true 1 else 2 end", &cu);
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

	Ake_Ast* if_stmt = Ake_ast_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, if_stmt, "ptr if_stmt")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, if_stmt->kind, Ake_ast_type_if, "if if_stmt");

	Ake_Ast* cb = Ake_ast_get(if_stmt, 0);
	if (!Zinc_expect_ptr(test, cb, "ptr cb")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cb->kind, Ake_ast_type_conditional_branch, "conditional branch cb");

	Ake_Ast* cond = Ake_ast_get(cb, 0);
	if (!Zinc_expect_ptr(test, cond, "ptr cond")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cond->kind, Ake_ast_type_boolean, "boolean cond");
	Zinc_expect_string(test, &cond->value, "true", "true cond");

	Ake_Ast* stmts_cb = Ake_ast_get(cb, 1);
	if (!Zinc_expect_ptr(test, stmts_cb, "ptr stmts_cb")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts_cb->kind, Ake_ast_type_stmts, "parse_stmts stmts_cb");

	Ake_Ast* num0 = Ake_ast_get(stmts_cb, 0);
	if (!Zinc_expect_ptr(test, num0, "ptr num0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num0->kind, Ake_ast_type_number, "number num0");
	Zinc_expect_string(test, &num0->value, "1", "1 num0");

	Ake_Ast* db = Ake_ast_get(if_stmt, 1);
	if (!Zinc_expect_ptr(test, db, "ptr db")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, db->kind, Ake_ast_type_default_branch, "default branch db");

	Ake_Ast* stmts_db = Ake_ast_get(db, 0);
	if (!Zinc_expect_ptr(test, stmts_db, "ptr stmts_db")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts_db->kind, Ake_ast_type_stmts, "parse_stmts stmts_db");

	Ake_Ast* num1 = Ake_ast_get(stmts_db, 0);
	if (!Zinc_expect_ptr(test, num1, "ptr num1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num1->kind, Ake_ast_type_number, "number num1");
	Zinc_expect_string(test, &num1->value, "2", "2 num1");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_stmts_type(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_comp_unit cu;

    AkeUnit_parse_setup("1; true; \"hello\"", &cu);
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

	Ake_Type* type = cu.root->type;
	if (!Zinc_expect_ptr(test, type, "ptr tu")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, type->kind, AKE_TYPE_ARRAY, "kind tu");
	Zinc_expect_true(test, type->data.array.is_const, "is_const type");
	Zinc_expect_uint8_t_equal(test, type->data.array.dim, 6, "dim tu");

	Ake_Type* type2 = type->data.array.type;
	if (!Zinc_expect_ptr(test, type2, "ptr td")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, type2->kind, AKE_TYPE_NATURAL, "type td");
	Zinc_expect_string(test, &type2->name, "Nat8", "name td");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
void AkeUnit_parse_if(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("if true const x: Int32; const y: Int32; 10 + 20; x * y end", &cu);
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

	Ake_Ast* if_node = Ake_ast_get(cu.root, 0);
	if (!Zinc_expect_ptr(test, if_node, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, if_node->kind, Ake_ast_type_if, "if if_node");

    Ake_Type* type = if_node->type;
	Zinc_expect_null(test, type, "null tu");

	Ake_Ast* cb = Ake_ast_get(if_node, 0);
	if (!Zinc_expect_ptr(test, cb, "ptr cb")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cb->kind, Ake_ast_type_conditional_branch, "conditional branch");

	Ake_Ast* cond = Ake_ast_get(cb, 0);
	if (!Zinc_expect_ptr(test, cond, "ptr cond")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cond->kind, Ake_ast_type_boolean, "boolean");
	Zinc_expect_string(test, &cond->value, "true", "true");

	Ake_Ast* stmts = Ake_ast_get(cb, 1);
	if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts->kind, Ake_ast_type_stmts, "parse_stmts");

	Ake_Ast* plus = Ake_ast_get(stmts, 2);
	if (!Zinc_expect_ptr(test, plus, "ptr plus")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, plus->kind, Ake_ast_type_plus, "plus");

	Ake_Ast* num0 = Ake_ast_get(plus, 0);
	if (!Zinc_expect_ptr(test, num0, "ptr num0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num0->kind, Ake_ast_type_number, "number 0");
	Zinc_expect_string(test, &num0->value, "10", "10");

	Ake_Ast* num1 = Ake_ast_get(plus, 1);
	if (!Zinc_expect_ptr(test, num1, "ptr num1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num1->kind, Ake_ast_type_number, "number 1");
	Zinc_expect_string(test, &num1->value, "20", "20");

	Ake_Ast* mult = Ake_ast_get(stmts, 3);
	if (!Zinc_expect_ptr(test, mult, "ptr mult")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, mult->kind, Ake_ast_type_mult, "mult");

	Ake_Ast* x = Ake_ast_get(mult, 0);
	if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, x->kind, Ake_ast_type_id, "id x");
	Zinc_expect_string(test, &x->value, "x", "x");

	Ake_Ast* y = Ake_ast_get(mult, 1);
	if (!Zinc_expect_ptr(test, y, "ptr y")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, y->kind, Ake_ast_type_id, "id y");
	Zinc_expect_string(test, &y->value, "y", "y");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
void AkeUnit_parse_elseif(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const x: Int32; const y: Int32; if true 10 + 20; x * y elseif true 1; 2 end", &cu);
	if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	if (!Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid")) {
		return Zinc_assert();
	}

	if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}

	Ake_Ast* if_stmt = Ake_ast_get(cu.root, 2);
	if (!Zinc_expect_ptr(test, if_stmt, "ptr if_stmt")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, if_stmt->kind, Ake_ast_type_if, "if");

	Ake_Ast* cb = Ake_ast_get(if_stmt, 0);
	if (!Zinc_expect_ptr(test, cb, "ptr cb")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cb->kind, Ake_ast_type_conditional_branch, "conditional branch");

	Ake_Ast* cond = Ake_ast_get(cb, 0);
	if (!Zinc_expect_ptr(test, cond, "ptr cond")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cond->kind, Ake_ast_type_boolean, "boolean cond");
	Zinc_expect_string(test, &cond->value, "true", "true");

	Ake_Ast* stmts = Ake_ast_get(cb, 1);
	if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts->kind, Ake_ast_type_stmts, "parse_stmts");

	Ake_Ast* plus = Ake_ast_get(stmts, 0);
	if (!Zinc_expect_ptr(test, plus, "ptr plus")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, plus->kind, Ake_ast_type_plus, "plus");

	Ake_Ast* num0 = Ake_ast_get(plus, 0);
	if (!Zinc_expect_ptr(test, num0, "ptr num0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num0->kind, Ake_ast_type_number, "number 0");
	Zinc_expect_string(test, &num0->value, "10", "10");

	Ake_Ast* num1 = Ake_ast_get(plus, 1);
	if (!Zinc_expect_ptr(test, num1, "ptr num1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num1->kind, Ake_ast_type_number, "number 1");
	Zinc_expect_string(test, &num1->value, "20", "20");

	Ake_Ast* mult = Ake_ast_get(stmts, 1);
	if (!Zinc_expect_ptr(test, mult, "ptr mult")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, mult->kind, Ake_ast_type_mult, "mult");

	Ake_Ast* x = Ake_ast_get(mult, 0);
	if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, x->kind, Ake_ast_type_id, "id x");
	Zinc_expect_string(test, &x->value, "x", "x");

	Ake_Ast* y = Ake_ast_get(mult, 1);
	if (!Zinc_expect_ptr(test, y, "ptr y")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, y->kind, Ake_ast_type_id, "id y");
	Zinc_expect_string(test, &y->value, "y", "y");

	Ake_Ast* cb2 = Ake_ast_get(if_stmt, 1);
	if (!Zinc_expect_ptr(test, cb2, "ptr cb2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cb2->kind, Ake_ast_type_conditional_branch, "conditional branch cb2");

	Ake_Ast* cond2 = Ake_ast_get(cb2, 0);
	if (!Zinc_expect_ptr(test, cond2, "ptr cond2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cond2->kind, Ake_ast_type_boolean, "boolean cond2");
	Zinc_expect_string(test, &cond2->value, "true", "true cond2");

	Ake_Ast* stmts2 = Ake_ast_get(cb2, 1);
	if (!Zinc_expect_ptr(test, stmts2, "ptr stmts2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts2->kind, Ake_ast_type_stmts, "parse_stmts stmts2");

	Ake_Ast* num2 = Ake_ast_get(stmts2, 0);
	if (!Zinc_expect_ptr(test, num2, "ptr num2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num2->kind, Ake_ast_type_number, "number num2");
	Zinc_expect_string(test, &num2->value, "1", "1 num2");

	Ake_Ast* num3 = Ake_ast_get(stmts2, 1);
	if (!Zinc_expect_ptr(test, num3, "ptr num3")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num3->kind, Ake_ast_type_number, "number num3");
	Zinc_expect_string(test, &num3->value, "2", "2 num3");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
void AkeUnit_parse_elseif2(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const x: Int32\n"
        "const y: Int32\n"
        "if true\n"
        "  10 + 20\n"
        "  x * y\n"
        "elseif true\n"
        "  1\n"
        "  2\n"
        "elseif true\n"
        "  x\n"
        "  y\n"
        "end\n"
        , &cu);
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

	Ake_Ast* if_stmt = Ake_ast_get(cu.root, 2);
	if (!Zinc_expect_ptr(test, if_stmt, "ptr cu.root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, if_stmt->kind, Ake_ast_type_if, "if if_stmt");

	Ake_Ast* cb = Ake_ast_get(if_stmt, 0);
	if (!Zinc_expect_ptr(test, cb, "ptr cb")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cb->kind, Ake_ast_type_conditional_branch, "conditional branch");

	Ake_Ast* cond = Ake_ast_get(cb, 0);
	if (!Zinc_expect_ptr(test, cond, "ptr cond")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cond->kind, Ake_ast_type_boolean, "boolean cond");
	Zinc_expect_string(test, &cond->value, "true", "true");

	Ake_Ast* stmts = Ake_ast_get(cb, 1);
	if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts->kind, Ake_ast_type_stmts, "parse_stmts");

	Ake_Ast* plus = Ake_ast_get(stmts, 0);
	if (!Zinc_expect_ptr(test, plus, "ptr plus")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, plus->kind, Ake_ast_type_plus, "plus");

	Ake_Ast* num0 = Ake_ast_get(plus, 0);
	if (!Zinc_expect_ptr(test, num0, "ptr num0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num0->kind, Ake_ast_type_number, "number 0");
	Zinc_expect_string(test, &num0->value, "10", "10");

	Ake_Ast* num1 = Ake_ast_get(plus, 1);
	if (!Zinc_expect_ptr(test, num1, "ptr num1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num1->kind, Ake_ast_type_number, "number 1");
	Zinc_expect_string(test, &num1->value, "20", "20");

	Ake_Ast* mult = Ake_ast_get(stmts, 1);
	if (!Zinc_expect_ptr(test, mult, "ptr mult")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, mult->kind, Ake_ast_type_mult, "mult");

	Ake_Ast* x = Ake_ast_get(mult, 0);
	if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, x->kind, Ake_ast_type_id, "id x");
	Zinc_expect_string(test, &x->value, "x", "x");

	Ake_Ast* y = Ake_ast_get(mult, 1);
	if (!Zinc_expect_ptr(test, y, "ptr y")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, y->kind, Ake_ast_type_id, "id y");
	Zinc_expect_string(test, &y->value, "y", "y");

	/* first elseif */
	Ake_Ast* cb2 = Ake_ast_get(if_stmt, 1);
	if (!Zinc_expect_ptr(test, cb2, "ptr cb2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cb2->kind, Ake_ast_type_conditional_branch, "conditional branch cb2");

	Ake_Ast* cond2 = Ake_ast_get(cb2, 0);
	if (!Zinc_expect_ptr(test, cond2, "ptr cond2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cond2->kind, Ake_ast_type_boolean, "boolean cond2");
	Zinc_expect_string(test, &cond2->value, "true", "true cond2");

	Ake_Ast* stmts2 = Ake_ast_get(cb2, 1);
	if (!Zinc_expect_ptr(test, stmts2, "ptr stmts2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts2->kind, Ake_ast_type_stmts, "parse_stmts stmts2");

	Ake_Ast* num2 = Ake_ast_get(stmts2, 0);
	if (!Zinc_expect_ptr(test, num2, "ptr num2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num2->kind, Ake_ast_type_number, "number num2");
	Zinc_expect_string(test, &num2->value, "1", "1 num2");

	Ake_Ast* num3 = Ake_ast_get(stmts2, 1);
	if (!Zinc_expect_ptr(test, num3, "ptr num3")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num3->kind, Ake_ast_type_number, "number num3");
	Zinc_expect_string(test, &num3->value, "2", "2 num3");

	/* second elseif */
	Ake_Ast* cb3 = Ake_ast_get(if_stmt, 2);
	if (!Zinc_expect_ptr(test, cb3, "ptr cb3")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cb3->kind, Ake_ast_type_conditional_branch, "conditional branch cb3");

	Ake_Ast* cond3 = Ake_ast_get(cb3, 0);
	if (!Zinc_expect_ptr(test, cond3, "ptr cond3")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cond3->kind, Ake_ast_type_boolean, "boolean cond3");
	Zinc_expect_string(test, &cond3->value, "true", "true cond3");

	Ake_Ast* stmts3 = Ake_ast_get(cb3, 1);
	if (!Zinc_expect_ptr(test, stmts3, "ptr stmts3")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts3->kind, Ake_ast_type_stmts, "parse_stmts stmts3");

	Ake_Ast* x2 = Ake_ast_get(stmts3, 0);
	if (!Zinc_expect_ptr(test, x2, "ptr x2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, x2->kind, Ake_ast_type_id, "id x2");
	Zinc_expect_string(test, &x2->value, "x", "x x2");

	Ake_Ast* y2 = Ake_ast_get(stmts3, 1);
	if (!Zinc_expect_ptr(test, y2, "ptr y2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, y2->kind, Ake_ast_type_id, "id y2");
	Zinc_expect_string(test, &y2->value, "y", "y y2");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
void AkeUnit_parse_else(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const x: Int32; const y: Int32; if false 10 else x; y end", &cu);
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

	Ake_Ast* if_stmt = Ake_ast_get(cu.root, 2);
	if (!Zinc_expect_ptr(test, if_stmt, "ptr if_stmt")) {
		return Zinc_assert();
	}
	if (!Zinc_expect_int_equal(test, if_stmt->kind, Ake_ast_type_if, "if if_stmt")) {
		return Zinc_assert();
	}

	Ake_Ast* cb0 = Ake_ast_get(if_stmt, 0);
	if (!Zinc_expect_ptr(test, cb0, "ptr cb0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cb0->kind, Ake_ast_type_conditional_branch, "conditional branch cb0");

	Ake_Ast* cond = Ake_ast_get(cb0, 0);
	if (!Zinc_expect_ptr(test, cond, "ptr cond")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cond->kind, Ake_ast_type_boolean, "boolean cond");
	Zinc_expect_string(test, &cond->value, "false", "false cond");

	Ake_Ast* stmts0 = Ake_ast_get(cb0, 1);
	if (!Zinc_expect_ptr(test, stmts0, "ptr parse_stmts 0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts0->kind, Ake_ast_type_stmts, "parse_stmts stmts0");

	Ake_Ast* num = Ake_ast_get(stmts0, 0);
	if (!Zinc_expect_ptr(test, num, "ptr num")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num->kind, Ake_ast_type_number, "number num");
	Zinc_expect_string(test, &num->value, "10", "10 num");

	Ake_Ast* cb1 = Ake_ast_get(if_stmt, 1);
	if (!Zinc_expect_ptr(test, cb1, "ptr cb1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cb1->kind, Ake_ast_type_default_branch, "default branch cb1");

	Ake_Ast* stmts1 = Ake_ast_get(cb1, 0);
	if (!Zinc_expect_ptr(test, stmts1, "ptr stmts1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts1->kind, Ake_ast_type_stmts, "parse_stmts stmts1");

	Ake_Ast* x = Ake_ast_get(stmts1, 0);
	if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, x->kind, Ake_ast_type_id, "id x");
	Zinc_expect_string(test, &x->value, "x", "x");

	Ake_Ast* y = Ake_ast_get(stmts1, 1);
	if (!Zinc_expect_ptr(test, y, "ptr y")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, y->kind, Ake_ast_type_id, "id y");
	Zinc_expect_string(test, &y->value, "y", "y");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
void AkeUnit_parse_else2(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const x: Int32; const y: Int32; if false 10 elseif false 20 else x; y end", &cu);
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

	Ake_Ast* if_stmt = Ake_ast_get(cu.root, 2);
	if (!Zinc_expect_ptr(test, if_stmt, "ptr cu.root")) {
		return Zinc_assert();
	}
	if (!Zinc_expect_int_equal(test, if_stmt->kind, Ake_ast_type_if, "if")) {
		return Zinc_assert();
	}

	/* if */
	Ake_Ast* cb0 = Ake_ast_get(if_stmt, 0);
	if (!Zinc_expect_ptr(test, cb0, "ptr cb0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cb0->kind, Ake_ast_type_conditional_branch, "conditional branch cb0");

	Ake_Ast* cond0 = Ake_ast_get(cb0, 0);
	if (!Zinc_expect_ptr(test, cond0, "ptr cond0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cond0->kind, Ake_ast_type_boolean, "boolean cond0");
	Zinc_expect_string(test, &cond0->value, "false", "false cond0");

	Ake_Ast* stmts0 = Ake_ast_get(cb0, 1);
	if (!Zinc_expect_ptr(test, stmts0, "ptr parse_stmts 0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts0->kind, Ake_ast_type_stmts, "parse_stmts stmts0");

	Ake_Ast* num0 = Ake_ast_get(stmts0, 0);
	if (!Zinc_expect_ptr(test, num0, "ptr num0")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, num0->kind, Ake_ast_type_number, "number num0");
	Zinc_expect_string(test, &num0->value, "10", "10 num0");

	/* elseif */
	Ake_Ast* cb1 = Ake_ast_get(if_stmt, 1);
	if (!Zinc_expect_ptr(test, cb1, "ptr cb1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cb1->kind, Ake_ast_type_conditional_branch, "conditional branch cb1");

	Ake_Ast* cond1 = Ake_ast_get(cb1, 0);
	if (!Zinc_expect_ptr(test, cond1, "ptr cond")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, cond1->kind, Ake_ast_type_boolean, "boolean cond1");
	Zinc_expect_string(test, &cond1->value, "false", "false cond1");

	Ake_Ast* stmts1 = Ake_ast_get(cb1, 1);
	if (!Zinc_expect_ptr(test, stmts1, "ptr stmts1")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts1->kind, Ake_ast_type_stmts, "parse_stmts stmts1");

	Ake_Ast* num1 = Ake_ast_get(stmts1, 0);
	Zinc_expect_int_equal(test, num1->kind, Ake_ast_type_number, "number num1");
	Zinc_expect_string(test, &num1->value, "20", "20 num1");

	/* else */
	Ake_Ast* db = Ake_ast_get(if_stmt, 2);
	if (!Zinc_expect_ptr(test, db, "ptr db")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, db->kind, Ake_ast_type_default_branch, "default branch db");

	Ake_Ast* stmts2 = Ake_ast_get(db, 0);
	if (!Zinc_expect_ptr(test, stmts2, "ptr stmts2")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, stmts2->kind, Ake_ast_type_stmts, "parse_stmts stmts2");

	Ake_Ast* x = Ake_ast_get(stmts2, 0);
	if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, x->kind, Ake_ast_type_id, "id x");
	Zinc_expect_string(test, &x->value, "x", "x");

	Ake_Ast* y = Ake_ast_get(stmts2, 1);
	if (!Zinc_expect_ptr(test, y, "ptr y")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, y->kind, Ake_ast_type_id, "id y");
	Zinc_expect_string(test, &y->value, "y", "y");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
void AkeUnit_parse_if_error_expected_expression(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("if end", &cu);
	if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
	Zinc_expect_false(test, cu.valid, "valid");
	Zinc_expect_source_error(test, &cu.errors, "expected condition after if");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
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

		Zinc_test_register(test, AkeUnit_parse_stmts4);
		Zinc_test_register(test, AkeUnit_parse_stmts5);
		Zinc_test_register(test, AkeUnit_parse_stmts_type);
		Zinc_test_register(test, AkeUnit_parse_if);
		Zinc_test_register(test, AkeUnit_parse_elseif);
		Zinc_test_register(test, AkeUnit_parse_elseif2);
		Zinc_test_register(test, AkeUnit_parse_else);
		Zinc_test_register(test, AkeUnit_parse_else2);
		Zinc_test_register(test, AkeUnit_parse_if_error_expected_expression);
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
