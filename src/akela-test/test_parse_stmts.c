#include "zinc/unit_test.h"
#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "test_parse.h"
#include "akela/type_def.h"
#include "akela/comp_unit.h"
#include "zinc/error_unit_test.h"

void test_parse_assign()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let mut a: i64; a = 1", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* assign = Ast_node_get(cu.root, 1);
	assert_ptr(assign, "ptr assign");
	assert_int_equal(assign->type, Ake_ast_type_assign, "assign");

	Ake_ast* left = Ast_node_get(assign, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ake_ast_type_id, "id");
	expect_str(&left->value, "a", "a");

	Ake_ast* right = Ast_node_get(assign, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ake_ast_type_number, "number");
	expect_str(&right->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_assign2()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let a: i64; a = 1 + 2", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* assign = Ast_node_get(cu.root, 1);
	assert_ptr(assign, "ptr assign");
	assert_int_equal(assign->type, Ake_ast_type_assign, "assign");

	Ake_ast* left = Ast_node_get(assign, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ake_ast_type_id, "id");
	expect_str(&left->value, "a", "a");

	Ake_ast* right = Ast_node_get(assign, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ake_ast_type_plus, "plus");

	Ake_ast* left2 = Ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, Ake_ast_type_number, "number");
	expect_str(&left2->value, "1", "1");

	Ake_ast* right2 = Ast_node_get(right, 1);
	assert_ptr(right2, "right");
	expect_int_equal(right2->type, Ake_ast_type_number, "number2");
	expect_str(&right2->value, "2", "2");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_stmts()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let i: i64; let x: i64; i + 1; x * 1", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr1");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts");

	Ake_ast* left = Ast_node_get(cu.root, 2);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ake_ast_type_plus, "plus");

	Ake_ast* left2 = Ast_node_get(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, Ake_ast_type_id, "id");
	expect_str(&left2->value, "i", "i");

	Ake_ast* right = Ast_node_get(left, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ake_ast_type_number, "number");
	expect_str(&right->value, "1", "1");

	Ake_ast* right2 = Ast_node_get(cu.root, 3);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, Ake_ast_type_mult, "mult");

	Ake_ast* left3 = Ast_node_get(right2, 0);
	assert_ptr(left3, "left3");
	expect_int_equal(left3->type, Ake_ast_type_id, "id");
	expect_str(&left3->value, "x", "x");

	Ake_ast* right3 = Ast_node_get(right2, 1);
	assert_ptr(right3, "right3");
	expect_int_equal(right3->type, Ake_ast_type_number, "number2");
	expect_str(&right3->value, "1", "1");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_stmts2()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let x: i64; x+1\n5-4\n", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts");

	Ake_ast* a = Ast_node_get(cu.root, 1);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ake_ast_type_plus, "plus a");

	Ake_ast* b = Ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, Ake_ast_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	Ake_ast* c = Ast_node_get(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, Ake_ast_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	Ake_ast* d = Ast_node_get(cu.root, 2);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, Ake_ast_type_minus, "minus d");

	Ake_ast* e = Ast_node_get(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, Ake_ast_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	Ake_ast* f = Ast_node_get(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, Ake_ast_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_stmts3()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let x: i64; let y: i64; let z: i64; x+1; 5-4; y+z", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts");

	Ake_ast* a = Ast_node_get(cu.root, 3);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ake_ast_type_plus, "plus a");

	Ake_ast* b = Ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, Ake_ast_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	Ake_ast* c = Ast_node_get(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, Ake_ast_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	Ake_ast* d = Ast_node_get(cu.root, 4);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, Ake_ast_type_minus, "minus d");

	Ake_ast* e = Ast_node_get(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, Ake_ast_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	Ake_ast* f = Ast_node_get(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, Ake_ast_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

	Ake_ast* g = Ast_node_get(cu.root, 5);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, Ake_ast_type_plus, "plus g");

	Ake_ast* h = Ast_node_get(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, Ake_ast_type_id, "id h");
	expect_str(&h->value, "y", "y h");

	Ake_ast* i = Ast_node_get(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, Ake_ast_type_id, "id i");
	expect_str(&i->value, "z", "z c");

    parse_teardown(&cu);
}

void test_parse_stmts4()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let x: i64; let y: i64; let z: i64; x+1; 5-4; y+z", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts");

	Ake_ast* a = Ast_node_get(cu.root, 3);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ake_ast_type_plus, "plus a");

	Ake_ast* b = Ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, Ake_ast_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	Ake_ast* c = Ast_node_get(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, Ake_ast_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	Ake_ast* d = Ast_node_get(cu.root, 4);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, Ake_ast_type_minus, "minus d");

	Ake_ast* e = Ast_node_get(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, Ake_ast_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	Ake_ast* f = Ast_node_get(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, Ake_ast_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

	Ake_ast* g = Ast_node_get(cu.root, 5);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, Ake_ast_type_plus, "plus g");

	Ake_ast* h = Ast_node_get(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, Ake_ast_type_id, "id h");
	expect_str(&h->value, "y", "y h");

	Ake_ast* i = Ast_node_get(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, Ake_ast_type_id, "id i");
	expect_str(&i->value, "z", "z c");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_stmts5()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("if true 1 else 2 end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* if_stmt = Ast_node_get(cu.root, 0);
	assert_ptr(if_stmt, "ptr if_stmt");
	expect_int_equal(if_stmt->type, Ake_ast_type_if, "if if_stmt");

	Ake_ast* cb = Ast_node_get(if_stmt, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, Ake_ast_type_conditional_branch, "conditional branch cb");

	Ake_ast* cond = Ast_node_get(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, Ake_ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "true", "true cond");

	Ake_ast* stmts_cb = Ast_node_get(cb, 1);
	assert_ptr(stmts_cb, "ptr stmts_cb");
	expect_int_equal(stmts_cb->type, Ake_ast_type_stmts, "parse_stmts stmts_cb");

	Ake_ast* num0 = Ast_node_get(stmts_cb, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, Ake_ast_type_number, "number num0");
	expect_str(&num0->value, "1", "1 num0");

	Ake_ast* db = Ast_node_get(if_stmt, 1);
	assert_ptr(db, "ptr db");
	expect_int_equal(db->type, Ake_ast_type_default_branch, "default branch db");

	Ake_ast* stmts_db = Ast_node_get(db, 0);
	assert_ptr(stmts_db, "ptr stmts_db");
	expect_int_equal(stmts_db->type, Ake_ast_type_stmts, "parse_stmts stmts_db");

	Ake_ast* num1 = Ast_node_get(stmts_db, 0);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, Ake_ast_type_number, "number num1");
	expect_str(&num1->value, "2", "2 num1");

    parse_teardown(&cu);
}

void test_parse_stmts_type()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("1; true; \"hello\"", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Type_use* tu = cu.root->tu;
	assert_ptr(tu, "ptr tu");
	
	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "type td");
	expect_str(&td->name, "u8", "name td");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_if()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("if true let x: i64; let y: i64; 10 + 20; x * y end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* if_node = Ast_node_get(cu.root, 0);
	assert_ptr(if_node, "ptr cu.root");
	expect_int_equal(if_node->type, Ake_ast_type_if, "if if_node");

    Type_use* tu = if_node->tu;
	expect_null(tu, "null tu");

	Ake_ast* cb = Ast_node_get(if_node, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, Ake_ast_type_conditional_branch, "conditional branch");

	Ake_ast* cond = Ast_node_get(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, Ake_ast_type_boolean, "boolean");
	expect_str(&cond->value, "true", "true");

	Ake_ast* stmts = Ast_node_get(cb, 1);
	assert_ptr(stmts, "ptr parse_stmts");
	expect_int_equal(stmts->type, Ake_ast_type_stmts, "parse_stmts");

	Ake_ast* plus = Ast_node_get(stmts, 2);
	assert_ptr(plus, "ptr plus");
	expect_int_equal(plus->type, Ake_ast_type_plus, "plus");

	Ake_ast* num0 = Ast_node_get(plus, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, Ake_ast_type_number, "number 0");
	expect_str(&num0->value, "10", "10");

	Ake_ast* num1 = Ast_node_get(plus, 1);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, Ake_ast_type_number, "number 1");
	expect_str(&num1->value, "20", "20");

	Ake_ast* mult = Ast_node_get(stmts, 3);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, Ake_ast_type_mult, "mult");

	Ake_ast* x = Ast_node_get(mult, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, Ake_ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	Ake_ast* y = Ast_node_get(mult, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, Ake_ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_elseif()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let x: i64; let y: i64; if true 10 + 20; x * y elseif true 1; 2 end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");

	Ake_ast* if_stmt = Ast_node_get(cu.root, 2);
	assert_ptr(if_stmt, "ptr if_stmt");
	expect_int_equal(if_stmt->type, Ake_ast_type_if, "if");

	Ake_ast* cb = Ast_node_get(if_stmt, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, Ake_ast_type_conditional_branch, "conditional branch");

	Ake_ast* cond = Ast_node_get(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, Ake_ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "true", "true");

	Ake_ast* stmts = Ast_node_get(cb, 1);
	assert_ptr(stmts, "ptr parse_stmts");
	expect_int_equal(stmts->type, Ake_ast_type_stmts, "parse_stmts");

	Ake_ast* plus = Ast_node_get(stmts, 0);
	assert_ptr(plus, "ptr plus");
	expect_int_equal(plus->type, Ake_ast_type_plus, "plus");

	Ake_ast* num0 = Ast_node_get(plus, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, Ake_ast_type_number, "number 0");
	expect_str(&num0->value, "10", "10");

	Ake_ast* num1 = Ast_node_get(plus, 1);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, Ake_ast_type_number, "number 1");
	expect_str(&num1->value, "20", "20");

	Ake_ast* mult = Ast_node_get(stmts, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, Ake_ast_type_mult, "mult");

	Ake_ast* x = Ast_node_get(mult, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, Ake_ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	Ake_ast* y = Ast_node_get(mult, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, Ake_ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

	Ake_ast* cb2 = Ast_node_get(if_stmt, 1);
	assert_ptr(cb2, "ptr cb2");
	expect_int_equal(cb2->type, Ake_ast_type_conditional_branch, "conditional branch cb2");

	Ake_ast* cond2 = Ast_node_get(cb2, 0);
	assert_ptr(cond2, "ptr cond2");
	expect_int_equal(cond2->type, Ake_ast_type_boolean, "boolean cond2");
	expect_str(&cond2->value, "true", "true cond2");

	Ake_ast* stmts2 = Ast_node_get(cb2, 1);
	assert_ptr(stmts2, "ptr stmts2");
	expect_int_equal(stmts2->type, Ake_ast_type_stmts, "parse_stmts stmts2");

	Ake_ast* num2 = Ast_node_get(stmts2, 0);
	assert_ptr(num2, "ptr num2");
	expect_int_equal(num2->type, Ake_ast_type_number, "number num2");
	expect_str(&num2->value, "1", "1 num2");

	Ake_ast* num3 = Ast_node_get(stmts2, 1);
	assert_ptr(num3, "ptr num3");
	expect_int_equal(num3->type, Ake_ast_type_number, "number num3");
	expect_str(&num3->value, "2", "2 num3");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_elseif2()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let x: i64\n"
        "let y: i64\n"
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
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* if_stmt = Ast_node_get(cu.root, 2);
	assert_ptr(if_stmt, "ptr cu.root");
	expect_int_equal(if_stmt->type, Ake_ast_type_if, "if if_stmt");

	Ake_ast* cb = Ast_node_get(if_stmt, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, Ake_ast_type_conditional_branch, "conditional branch");

	Ake_ast* cond = Ast_node_get(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, Ake_ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "true", "true");

	Ake_ast* stmts = Ast_node_get(cb, 1);
	assert_ptr(stmts, "ptr parse_stmts");
	expect_int_equal(stmts->type, Ake_ast_type_stmts, "parse_stmts");

	Ake_ast* plus = Ast_node_get(stmts, 0);
	assert_ptr(plus, "ptr plus");
	expect_int_equal(plus->type, Ake_ast_type_plus, "plus");

	Ake_ast* num0 = Ast_node_get(plus, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, Ake_ast_type_number, "number 0");
	expect_str(&num0->value, "10", "10");

	Ake_ast* num1 = Ast_node_get(plus, 1);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, Ake_ast_type_number, "number 1");
	expect_str(&num1->value, "20", "20");

	Ake_ast* mult = Ast_node_get(stmts, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, Ake_ast_type_mult, "mult");

	Ake_ast* x = Ast_node_get(mult, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, Ake_ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	Ake_ast* y = Ast_node_get(mult, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, Ake_ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

	/* first elseif */
	Ake_ast* cb2 = Ast_node_get(if_stmt, 1);
	assert_ptr(cb2, "ptr cb2");
	expect_int_equal(cb2->type, Ake_ast_type_conditional_branch, "conditional branch cb2");

	Ake_ast* cond2 = Ast_node_get(cb2, 0);
	assert_ptr(cond2, "ptr cond2");
	expect_int_equal(cond2->type, Ake_ast_type_boolean, "boolean cond2");
	expect_str(&cond2->value, "true", "true cond2");

	Ake_ast* stmts2 = Ast_node_get(cb2, 1);
	assert_ptr(stmts2, "ptr stmts2");
	expect_int_equal(stmts2->type, Ake_ast_type_stmts, "parse_stmts stmts2");

	Ake_ast* num2 = Ast_node_get(stmts2, 0);
	assert_ptr(num2, "ptr num2");
	expect_int_equal(num2->type, Ake_ast_type_number, "number num2");
	expect_str(&num2->value, "1", "1 num2");

	Ake_ast* num3 = Ast_node_get(stmts2, 1);
	assert_ptr(num3, "ptr num3");
	expect_int_equal(num3->type, Ake_ast_type_number, "number num3");
	expect_str(&num3->value, "2", "2 num3");

	/* second elseif */
	Ake_ast* cb3 = Ast_node_get(if_stmt, 2);
	assert_ptr(cb3, "ptr cb3");
	expect_int_equal(cb3->type, Ake_ast_type_conditional_branch, "conditional branch cb3");

	Ake_ast* cond3 = Ast_node_get(cb3, 0);
	assert_ptr(cond3, "ptr cond3");
	expect_int_equal(cond3->type, Ake_ast_type_boolean, "boolean cond3");
	expect_str(&cond3->value, "true", "true cond3");

	Ake_ast* stmts3 = Ast_node_get(cb3, 1);
	assert_ptr(stmts3, "ptr stmts3");
	expect_int_equal(stmts3->type, Ake_ast_type_stmts, "parse_stmts stmts3");

	Ake_ast* x2 = Ast_node_get(stmts3, 0);
	assert_ptr(x2, "ptr x2");
	expect_int_equal(x2->type, Ake_ast_type_id, "id x2");
	expect_str(&x2->value, "x", "x x2");

	Ake_ast* y2 = Ast_node_get(stmts3, 1);
	assert_ptr(y2, "ptr y2");
	expect_int_equal(y2->type, Ake_ast_type_id, "id y2");
	expect_str(&y2->value, "y", "y y2");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_else()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let x: i64; let y: i64; if false 10 else x; y end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* if_stmt = Ast_node_get(cu.root, 2);
	assert_ptr(if_stmt, "ptr if_stmt");
	assert_int_equal(if_stmt->type, Ake_ast_type_if, "if if_stmt");

	Ake_ast* cb0 = Ast_node_get(if_stmt, 0);
	assert_ptr(cb0, "ptr cb0");
	expect_int_equal(cb0->type, Ake_ast_type_conditional_branch, "conditional branch cb0");

	Ake_ast* cond = Ast_node_get(cb0, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, Ake_ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "false", "false cond");

	Ake_ast* stmts0 = Ast_node_get(cb0, 1);
	assert_ptr(stmts0, "ptr parse_stmts 0");
	expect_int_equal(stmts0->type, Ake_ast_type_stmts, "parse_stmts stmts0");

	Ake_ast* num = Ast_node_get(stmts0, 0);
	assert_ptr(num, "ptr num");
	expect_int_equal(num->type, Ake_ast_type_number, "number num");
	expect_str(&num->value, "10", "10 num");

	Ake_ast* cb1 = Ast_node_get(if_stmt, 1);
	assert_ptr(cb1, "ptr cb1");
	expect_int_equal(cb1->type, Ake_ast_type_default_branch, "default branch cb1");

	Ake_ast* stmts1 = Ast_node_get(cb1, 0);
	assert_ptr(stmts1, "ptr stmts1");
	expect_int_equal(stmts1->type, Ake_ast_type_stmts, "parse_stmts stmts1");

	Ake_ast* x = Ast_node_get(stmts1, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, Ake_ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	Ake_ast* y = Ast_node_get(stmts1, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, Ake_ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_else2()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let x: i64; let y: i64; if false 10 elseif false 20 else x; y end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* if_stmt = Ast_node_get(cu.root, 2);
	assert_ptr(if_stmt, "ptr cu.root");
	assert_int_equal(if_stmt->type, Ake_ast_type_if, "if");

	/* if */
	Ake_ast* cb0 = Ast_node_get(if_stmt, 0);
	assert_ptr(cb0, "ptr cb0");
	expect_int_equal(cb0->type, Ake_ast_type_conditional_branch, "conditional branch cb0");

	Ake_ast* cond0 = Ast_node_get(cb0, 0);
	assert_ptr(cond0, "ptr cond0");
	expect_int_equal(cond0->type, Ake_ast_type_boolean, "boolean cond0");
	expect_str(&cond0->value, "false", "false cond0");

	Ake_ast* stmts0 = Ast_node_get(cb0, 1);
	assert_ptr(stmts0, "ptr parse_stmts 0");
	expect_int_equal(stmts0->type, Ake_ast_type_stmts, "parse_stmts stmts0");

	Ake_ast* num0 = Ast_node_get(stmts0, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, Ake_ast_type_number, "number num0");
	expect_str(&num0->value, "10", "10 num0");

	/* elseif */
	Ake_ast* cb1 = Ast_node_get(if_stmt, 1);
	assert_ptr(cb1, "ptr cb1");
	expect_int_equal(cb1->type, Ake_ast_type_conditional_branch, "conditional branch cb1");

	Ake_ast* cond1 = Ast_node_get(cb1, 0);
	assert_ptr(cond1, "ptr cond");
	expect_int_equal(cond1->type, Ake_ast_type_boolean, "boolean cond1");
	expect_str(&cond1->value, "false", "false cond1");

	Ake_ast* stmts1 = Ast_node_get(cb1, 1);
	assert_ptr(stmts1, "ptr stmts1");
	expect_int_equal(stmts1->type, Ake_ast_type_stmts, "parse_stmts stmts1");

	Ake_ast* num1 = Ast_node_get(stmts1, 0);
	expect_int_equal(num1->type, Ake_ast_type_number, "number num1");
	expect_str(&num1->value, "20", "20 num1");

	/* else */
	Ake_ast* db = Ast_node_get(if_stmt, 2);
	assert_ptr(db, "ptr db");
	expect_int_equal(db->type, Ake_ast_type_default_branch, "default branch db");

	Ake_ast* stmts2 = Ast_node_get(db, 0);
	assert_ptr(stmts2, "ptr stmts2");
	expect_int_equal(stmts2->type, Ake_ast_type_stmts, "parse_stmts stmts2");

	Ake_ast* x = Ast_node_get(stmts2, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, Ake_ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	Ake_ast* y = Ast_node_get(stmts2, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, Ake_ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_if_error_expected_expression()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("if end", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expected condition after if");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_if_error_expected_end()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("if true", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expected end");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_if_error_expected_elseif_expression()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("if true elseif end", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expected condition after elseif");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_while()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("while true 1 end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* node = Ast_node_get(cu.root, 0);
	assert_ptr(node, "ptr node");
	expect_int_equal(node->type, Ake_ast_type_while, "while node");
	expect_null(node->tu, "null node->tu");

	Ake_ast* cond = Ast_node_get(node, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, Ake_ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "true", "true cond");

	Ake_ast* stmts = Ast_node_get(node, 1);
	assert_ptr(stmts, "ptr parse_stmts");
	expect_int_equal(stmts->type, Ake_ast_type_stmts, "parse_stmts stmts");

	Ake_ast* num = Ast_node_get(stmts, 0);
	assert_ptr(num, "ptr num");
	expect_int_equal(num->type, Ake_ast_type_number, "number num");
	expect_str(&num->value, "1", "1 num");

    parse_teardown(&cu);
}

void test_parse_while_error_expected_expression()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("while end", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected expression after while");

    parse_teardown(&cu);
}

void test_parse_while_error_expected_end()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("while true", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected end");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_for_range()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("for i: i64 = 0:10 1 end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	/* for */
	Ake_ast* node = Ast_node_get(cu.root, 0);
	assert_ptr(node, "ptr node");
	expect_int_equal(node->type, Ake_ast_type_for_range, "for-range node");

	/* i */
	Ake_ast* dec = Ast_node_get(node, 0);
	assert_ptr(dec, "ptr dec");
	expect_int_equal(dec->type, Ake_ast_type_declaration, "declaration dec");

	Ake_ast* id = Ast_node_get(dec, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, Ake_ast_type_id, "id id");
	expect_str(&id->value, "i", "i id");

	/* start */
	Ake_ast* start = Ast_node_get(node, 1);
	assert_ptr(start, "ptr start");
	expect_int_equal(start->type, Ake_ast_type_number, "number start");
	expect_str(&start->value, "0", "0 start");

	/* end */
	Ake_ast* end = Ast_node_get(node, 2);
	assert_ptr(end, "ptr end");
	expect_int_equal(end->type, Ake_ast_type_number, "number end");
	expect_str(&end->value, "10", "10 end");

	/* stmts */
	Ake_ast* stmts = Ast_node_get(node, 3);
	assert_ptr(stmts, "ptr parse_stmts");
	expect_int_equal(stmts->type, Ake_ast_type_stmts, "parse_stmts stmts");

	Ake_ast* stmt0 = Ast_node_get(stmts, 0);
	assert_ptr(stmt0, "ptr stmt0");
	expect_int_equal(stmt0->type, Ake_ast_type_number, "number stmt0");
	expect_str(&stmt0->value, "1", "1 stmt0");

    parse_teardown(&cu);
}

void test_parse_for_range2()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("for i: i64 = 0:10 let i: i64 = 1 end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "duplicate declaration in same scope: i");

    parse_teardown(&cu);
}

void test_parse_for_iteration()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let list: [10]i64; for i: i64 in list i end", &cu);
	expect_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	/* for */
	Ake_ast* node = Ast_node_get(cu.root, 1);
	assert_ptr(node, "ptr node");
	expect_int_equal(node->type, Ake_ast_type_for_iteration, "for-iteration node");

	/* declaration */
	Ake_ast* dec = Ast_node_get(node, 0);
	assert_ptr(dec, "ptr dec");
	expect_int_equal(dec->type, Ake_ast_type_declaration, "declaration dec");

	/* id */
	Ake_ast* id = Ast_node_get(dec, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, Ake_ast_type_id, "id id");
	expect_str(&id->value, "i", "i id");

	Ake_ast* i_type_node = Ast_node_get(dec, 1);
	assert_ptr(i_type_node, "ptr i_tu");

    Type_use* i_tu = i_type_node->tu;
    assert_ptr(i_tu, "ptr i_tu");

	struct type_def* i_td = i_tu->td;
	assert_ptr(i_td, "ptr i_td");
	expect_int_equal(i_td->type, type_integer, "integer i_td");
	expect_str(&i_td->name, "i64", "i64 i_td");

	/* expr */
	Ake_ast* expr = Ast_node_get(node, 1);
	assert_ptr(expr, "ptr expr");
	expect_int_equal(expr->type, Ake_ast_type_id, "id expr");
	expect_str(&expr->value, "list", "id list");

	/* stmts */
	Ake_ast* stmts0 = Ast_node_get(node, 2);
	assert_ptr(stmts0, "ptr stmts0");
	expect_int_equal(stmts0->type, Ake_ast_type_stmts, "parse_stmts stmts0");

	/* i */
	Ake_ast* id2 = Ast_node_get(stmts0, 0);
	assert_ptr(id2, "ptr id2");
	expect_int_equal(id2->type, Ake_ast_type_id, "id id2");
	expect_str(&id2->value, "i", "i id2");

    parse_teardown(&cu);
}

void test_parse_for_iteration2()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let list: [10]i64; for i: i64 in list let i: i64 = 1 end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "duplicate declaration in same scope: i");

    parse_teardown(&cu);
}

void test_parse_for_iteration_error_no_value()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn list() end; for i: i64 in list() end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "iteration expression has no value");

    parse_teardown(&cu);
}

void test_parse_for_iteration_error_no_child_element()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let list: i64; for i: i64 in list end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "iteration expression is not an array");

    parse_teardown(&cu);
}

void test_parse_for_iteration_error_cannot_cast()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let list: [10]bool; for i: i64 in list end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "cannot cast list element");

    parse_teardown(&cu);
}

void test_parse_for_error_after_declaration()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let list: [10]i64; for i: i64 end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected '=' or 'in' after for element declaration");

    parse_teardown(&cu);
}

void test_parse_for_error_expected_end()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let list: [10]i64; for i: i64 = 1:10", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected end");

    parse_teardown(&cu);
}

void test_parse_for_error_expected_range_start()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let list: [10]i64; for i: i64 =", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected range start");

    parse_teardown(&cu);
}

void test_parse_for_error_expected_colon()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let list: [10]i64; for i: i64 = 1", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected colon");

    parse_teardown(&cu);
}

void test_parse_for_error_expected_range_end()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let list: [10]i64; for i: i64 = 1:", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected range end");

    parse_teardown(&cu);
}

void test_parse_for_range_error_start_no_value()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; for i: i64 = foo():10 end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "start range expression has no value");

    parse_teardown(&cu);
}

void test_parse_for_range_error_start_not_numeric()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("for i: i64 = true:10 end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "start range expression is not numeric");

    parse_teardown(&cu);
}

void test_parse_for_range_error_end_no_value()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; for i: i64 = 1:foo() end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "end range expression has no value");

    parse_teardown(&cu);
}

void test_parse_for_range_error_end_not_numeric()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("for i: i64 = 1:true end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "end range expression is not numeric");

    parse_teardown(&cu);
}

void test_parse_for_error_expected_iteration_expression()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let list: [10]i64; for i: i64 in:", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected for iteration expression");

    parse_teardown(&cu);
}

void test_parse_stmts_newline_for_range()
{
    test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("for\ni\n: \ni64\n=\n0\n:\n10 i end", &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_stmts_newline_for_iteration()
{
    test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let v: [10]i64; for\nx\n: \ni64\nin\nv x end", &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_let()
{
    test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let a: i32", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    assert_ptr(cu.root, "ptr cu.root");
    assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* let = Ast_node_get(cu.root, 0);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, Ake_ast_type_let, "type let");

    Ake_ast* let_lseq = Ast_node_get(let, 0);
    assert_ptr(let_lseq, "ptr let_lseq");
    expect_int_equal(let_lseq->type, Ake_ast_type_let_lseq, "type let_lseq");

    Ake_ast* id = Ast_node_get(let_lseq, 0);
    assert_ptr(id, "ptr id");
    expect_int_equal(id->type, Ake_ast_type_id, "id id");
    expect_str(&id->value, "a", "a");

    Ake_ast* type_node = Ast_node_get(let, 1);
    assert_ptr(type_node, "ptr type_node");

    Type_use* tu = type_node->tu;
    assert_ptr(tu, "ptr tu");

    struct type_def* td = tu->td;
    assert_ptr(td, "ptr td");
    expect_str(&td->name, "i32", "i32 td");

    parse_teardown(&cu);
}

void test_parse_let2()
{
    test_name(__func__);

    struct Ake_comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup("let a: i32 = 1", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "parse valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* let = Ast_node_get(cu.root, 0);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, Ake_ast_type_let, "type let");

    Ake_ast* let_lseq = Ast_node_get(let, 0);
    assert_ptr(let_lseq, "ptr let_lseq");
    expect_int_equal(let_lseq->type, Ake_ast_type_let_lseq, "type let_lseq");

    Ake_ast* id = Ast_node_get(let_lseq, 0);
    assert_ptr(id, "ptr id");
    expect_int_equal(id->type, Ake_ast_type_id, "id");
    expect_str(&id->value, "a", "a");

    Ake_ast* type_node = Ast_node_get(let, 1);
    assert_ptr(type_node, "ptr type_node");

    Type_use* tu = type_node->tu;
    assert_ptr(tu, "ptr tu");

    struct type_def* td = tu->td;
    assert_ptr(td, "ptr td");
    expect_str(&td->name, "i32", "i32 td");

    Ake_ast* let_rseq = Ast_node_get(let, 2);
    assert_ptr(let_rseq, "ptr let_rseq");
    expect_int_equal(let_rseq->type, Ake_ast_type_let_rseq, "type let_rseq");

    Ake_ast* value = Ast_node_get(let_rseq, 0);
    assert_ptr(value, "ptr value");
    expect_int_equal(value->type, Ake_ast_type_number, "number");
    expect_str(&value->value, "1", "1");

    /* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_let3()
{
    test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let a,b: i32 = 1,2", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "parse valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* let = Ast_node_get(cu.root, 0);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, Ake_ast_type_let, "type let");

    Ake_ast* let_lseq = Ast_node_get(let, 0);
    assert_ptr(let_lseq, "ptr let_lseq");
    expect_int_equal(let_lseq->type, Ake_ast_type_let_lseq, "type let_lseq");

    Ake_ast* id0 = Ast_node_get(let_lseq, 0);
    assert_ptr(id0, "ptr id0");
    expect_int_equal(id0->type, Ake_ast_type_id, "id id0");
    expect_str(&id0->value, "a", "value id0");

    Ake_ast* type_node = Ast_node_get(let, 1);
    assert_ptr(type_node, "ptr type_node");

    Type_use* tu = type_node->tu;
    assert_ptr(tu, "ptr tu");

    struct type_def* td = tu->td;
    assert_ptr(td, "ptr td");
    expect_str(&td->name, "i32", "i32 td");

    Ake_ast* id2 = Ast_node_get(let_lseq, 1);
    assert_ptr(id2, "ptr id2");
    expect_int_equal(id2->type, Ake_ast_type_id, "type id2");
    expect_str(&id2->value, "b", "value id2");

    Ake_ast* let_rseq = Ast_node_get(let, 2);
    assert_ptr(let_rseq, "ptr let_rseq");
    expect_int_equal(let_rseq->type, Ake_ast_type_let_rseq, "type let_rseq");

    Ake_ast* value0 = Ast_node_get(let_rseq, 0);
    assert_ptr(value0, "ptr value0");
    expect_int_equal(value0->type, Ake_ast_type_number, "type value0");
    expect_str(&value0->value, "1", "value value0");

    Ake_ast* value1 = Ast_node_get(let_rseq, 1);
    assert_ptr(value1, "ptr value1");
    expect_int_equal(value1->type, Ake_ast_type_number, "type value1");
    expect_str(&value1->value, "2", "value value1");

    parse_teardown(&cu);
}

void test_parse_let_expected_declaration()
{
    test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let", &cu);
    assert_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "expected variable(s) after let");

    parse_teardown(&cu);
}

void test_parse_extern()
{
    test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("extern foo(a: i64)\n"
                "foo(1)\n",
                &cu);
    assert_no_errors(&cu.el);
    assert_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ake_ast_type_stmts, "type cu.root");

    Ake_ast* f = Ast_node_get(cu.root, 0);
    assert_ptr(f, "ptr f");
    expect_int_equal(f->type, Ake_ast_type_extern, "type f");

    Type_use* tu = f->tu;
    assert_ptr(tu, "ptr tu");

    struct type_def* td = tu->td;
    assert_ptr(td, "ptr td");
    expect_int_equal(td->type, type_function, "type td");
    expect_str(&td->name, "Function", "name td");

    Ake_ast* proto = Ast_node_get(f, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* fid = Ast_node_get(proto, 0);
    assert_ptr(fid, "ptr fid");
    expect_int_equal(fid->type, Ake_ast_type_id, "id");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    assert_ptr(dseq, "ptr dseq");
    expect_int_equal(dseq->type, Ake_ast_type_dseq, "dseq dseq");

    Ake_ast* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* call = Ast_node_get(cu.root, 1);
    expect_ptr(call, "ptr call");
    expect_int_equal(call->type, Ake_ast_type_call, "type call");

    parse_teardown(&cu);
}

void test_parse_stmts_mut() {
    test_name(__func__);
    struct Ake_comp_unit cu;

    parse_setup("let mut x: i64 = 10\n"
                "x = 5\n"
                "x",
                &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "parse_setup valid");

    expect_int_equal(cu.root->type, Ake_ast_type_stmts, "type cu.root");

    Ake_ast *let = Ast_node_get(cu.root, 0);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, Ake_ast_type_let, "type let");

    Ake_ast *let_lseq = Ast_node_get(let, 0);
    assert_ptr(let_lseq, "ptr let_lseq");
    expect_int_equal(let_lseq->type, Ake_ast_type_let_lseq, "type let_lseq");

    Ake_ast *let_lhs = Ast_node_get(let_lseq, 0);
    assert_ptr(let_lhs, "ptr lhs");
    expect_int_equal(let_lhs->type, Ake_ast_type_id, "type lhs");
    expect_true(let_lhs->is_mut, "is_mut lhs");

    Ake_ast *assign = Ast_node_get(cu.root, 1);
    assert_ptr(assign, "ptr assign");
    expect_int_equal(assign->type, Ake_ast_type_assign, "type assign");

    assert_ptr(assign->tu, "ptr assign->tu");
    expect_true(assign->tu->is_mut, "is_mut assign");

    Ake_ast *x = Ast_node_get(assign, 0);
    assert_ptr(x, "ptr x");
    expect_int_equal(x->type, Ake_ast_type_id, "type id");

    expect_ptr(x->tu, "ptr x->tu");
    expect_true(x->tu->is_mut, "is_mut x->tu");

    parse_teardown(&cu);
}

void test_parse_stmts_error_mut()
{
    test_name(__func__);
    struct Ake_comp_unit cu;

    parse_setup("let x: i64 = 10\n"
                "x = 5\n"
                "x",
                &cu);
    expect_false(cu.valid, "parse_setup valid");
    expect_has_errors(&cu.el);
    expect_source_error(&cu.el, "immutable variable changed in assignment");
    parse_teardown(&cu);
}

void test_parse_statements()
{
	test_parse_assign();
	test_parse_assign2();
	test_parse_stmts();
	test_parse_stmts2();
	test_parse_stmts3();
	test_parse_stmts4();
	test_parse_stmts5();
	test_parse_stmts_type();
	test_parse_if();
	test_parse_elseif();
	test_parse_elseif2();
	test_parse_else();
	test_parse_else2();
	test_parse_if_error_expected_expression();
	test_parse_if_error_expected_end();
	test_parse_if_error_expected_elseif_expression();
	test_parse_while();
	test_parse_while_error_expected_expression();
	test_parse_while_error_expected_end();
	test_parse_for_range();
	test_parse_for_range2();
	test_parse_for_range_error_start_no_value();
	test_parse_for_range_error_start_not_numeric();
	test_parse_for_range_error_end_no_value();
	test_parse_for_range_error_end_not_numeric();
	test_parse_for_iteration();
	test_parse_for_iteration2();
	test_parse_for_iteration_error_no_value();
	test_parse_for_iteration_error_no_child_element();
	test_parse_for_iteration_error_cannot_cast();
	test_parse_for_error_after_declaration();
	test_parse_for_error_expected_end();
	test_parse_for_error_expected_range_start();
	test_parse_for_error_expected_colon();
	test_parse_for_error_expected_range_end();
	test_parse_for_error_expected_iteration_expression();
    test_parse_stmts_newline_for_range();
    test_parse_stmts_newline_for_iteration();
    test_parse_let();
    test_parse_let2();
    test_parse_let3();
    test_parse_let_expected_declaration();
    test_parse_extern();
    test_parse_stmts_mut();
    test_parse_stmts_error_mut();
}
