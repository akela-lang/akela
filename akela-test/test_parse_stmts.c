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

	struct comp_unit cu;

    parse_setup("let mut a::Int64; a = 1", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct Ast_node* assign = ast_node_get(cu.root, 1);
	assert_ptr(assign, "ptr assign");
	assert_int_equal(assign->type, ast_type_assign, "assign");

	struct Ast_node* left = ast_node_get(assign, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "a", "a");

	struct Ast_node* right = ast_node_get(assign, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "1", "1");

    parse_teardown(&cu);
}

void test_parse_assign2()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a::Int64; a = 1 + 2", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct Ast_node* assign = ast_node_get(cu.root, 1);
	assert_ptr(assign, "ptr assign");
	assert_int_equal(assign->type, ast_type_assign, "assign");

	struct Ast_node* left = ast_node_get(assign, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "a", "a");

	struct Ast_node* right = ast_node_get(assign, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_plus, "plus");

	struct Ast_node* left2 = ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_number, "number");
	expect_str(&left2->value, "1", "1");

	struct Ast_node* right2 = ast_node_get(right, 1);
	assert_ptr(right2, "right");
	expect_int_equal(right2->type, ast_type_number, "number2");
	expect_str(&right2->value, "2", "2");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_stmts()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let i::Int64; let x::Int64; i + 1; x * 1", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr1");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts");

	struct Ast_node* left = ast_node_get(cu.root, 2);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_plus, "plus");

	struct Ast_node* left2 = ast_node_get(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_id, "id");
	expect_str(&left2->value, "i", "i");

	struct Ast_node* right = ast_node_get(left, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "1", "1");

	struct Ast_node* right2 = ast_node_get(cu.root, 3);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, ast_type_mult, "mult");

	struct Ast_node* left3 = ast_node_get(right2, 0);
	assert_ptr(left3, "left3");
	expect_int_equal(left3->type, ast_type_id, "id");
	expect_str(&left3->value, "x", "x");

	struct Ast_node* right3 = ast_node_get(right2, 1);
	assert_ptr(right3, "right3");
	expect_int_equal(right3->type, ast_type_number, "number2");
	expect_str(&right3->value, "1", "1");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_stmts2()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let x::Int64; x+1\n5-4\n", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts");

	struct Ast_node* a = ast_node_get(cu.root, 1);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_plus, "plus a");

	struct Ast_node* b = ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	struct Ast_node* c = ast_node_get(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, ast_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	struct Ast_node* d = ast_node_get(cu.root, 2);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, ast_type_minus, "minus d");

	struct Ast_node* e = ast_node_get(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, ast_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	struct Ast_node* f = ast_node_get(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, ast_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_stmts3()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let x::Int64; let y::Int64; let z::Int64; x+1; 5-4; y+z", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts");

	struct Ast_node* a = ast_node_get(cu.root, 3);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_plus, "plus a");

	struct Ast_node* b = ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	struct Ast_node* c = ast_node_get(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, ast_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	struct Ast_node* d = ast_node_get(cu.root, 4);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, ast_type_minus, "minus d");

	struct Ast_node* e = ast_node_get(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, ast_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	struct Ast_node* f = ast_node_get(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, ast_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

	struct Ast_node* g = ast_node_get(cu.root, 5);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, ast_type_plus, "plus g");

	struct Ast_node* h = ast_node_get(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, ast_type_id, "id h");
	expect_str(&h->value, "y", "y h");

	struct Ast_node* i = ast_node_get(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, ast_type_id, "id i");
	expect_str(&i->value, "z", "z c");

    parse_teardown(&cu);
}

void test_parse_stmts4()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let x::Int64; let y::Int64; let z::Int64; x+1; 5-4; y+z", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts");

	struct Ast_node* a = ast_node_get(cu.root, 3);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_plus, "plus a");

	struct Ast_node* b = ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	struct Ast_node* c = ast_node_get(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, ast_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	struct Ast_node* d = ast_node_get(cu.root, 4);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, ast_type_minus, "minus d");

	struct Ast_node* e = ast_node_get(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, ast_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	struct Ast_node* f = ast_node_get(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, ast_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

	struct Ast_node* g = ast_node_get(cu.root, 5);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, ast_type_plus, "plus g");

	struct Ast_node* h = ast_node_get(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, ast_type_id, "id h");
	expect_str(&h->value, "y", "y h");

	struct Ast_node* i = ast_node_get(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, ast_type_id, "id i");
	expect_str(&i->value, "z", "z c");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_stmts5()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("if true 1 else 2 end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct Ast_node* if_stmt = ast_node_get(cu.root, 0);
	assert_ptr(if_stmt, "ptr if_stmt");
	expect_int_equal(if_stmt->type, ast_type_if, "if if_stmt");

	struct Ast_node* cb = ast_node_get(if_stmt, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, ast_type_conditional_branch, "conditional branch cb");

	struct Ast_node* cond = ast_node_get(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "true", "true cond");

	struct Ast_node* stmts_cb = ast_node_get(cb, 1);
	assert_ptr(stmts_cb, "ptr stmts_cb");
	expect_int_equal(stmts_cb->type, ast_type_stmts, "parse_stmts stmts_cb");

	struct Ast_node* num0 = ast_node_get(stmts_cb, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, ast_type_number, "number num0");
	expect_str(&num0->value, "1", "1 num0");

	struct Ast_node* db = ast_node_get(if_stmt, 1);
	assert_ptr(db, "ptr db");
	expect_int_equal(db->type, ast_type_default_branch, "default branch db");

	struct Ast_node* stmts_db = ast_node_get(db, 0);
	assert_ptr(stmts_db, "ptr stmts_db");
	expect_int_equal(stmts_db->type, ast_type_stmts, "parse_stmts stmts_db");

	struct Ast_node* num1 = ast_node_get(stmts_db, 0);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, ast_type_number, "number num1");
	expect_str(&num1->value, "2", "2 num1");

    parse_teardown(&cu);
}

void test_parse_stmts_type()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("1; true; \"hello\"", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct Ast_node* tu = cu.root->tu;
	assert_ptr(tu, "ptr tu");
	
	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "type td");
	expect_str(&td->name, "UInt8", "name td");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_if()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("if true let x::Int64; let y::Int64; 10 + 20; x * y end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct Ast_node* if_node = ast_node_get(cu.root, 0);
	assert_ptr(if_node, "ptr cu.root");
	expect_int_equal(if_node->type, ast_type_if, "if if_node");

    struct Ast_node* tu = if_node->tu;
	expect_null(tu, "null tu");

	struct Ast_node* cb = ast_node_get(if_node, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, ast_type_conditional_branch, "conditional branch");

	struct Ast_node* cond = ast_node_get(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, ast_type_boolean, "boolean");
	expect_str(&cond->value, "true", "true");

	struct Ast_node* stmts = ast_node_get(cb, 1);
	assert_ptr(stmts, "ptr parse_stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "parse_stmts");

	struct Ast_node* plus = ast_node_get(stmts, 2);
	assert_ptr(plus, "ptr plus");
	expect_int_equal(plus->type, ast_type_plus, "plus");

	struct Ast_node* num0 = ast_node_get(plus, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, ast_type_number, "number 0");
	expect_str(&num0->value, "10", "10");

	struct Ast_node* num1 = ast_node_get(plus, 1);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, ast_type_number, "number 1");
	expect_str(&num1->value, "20", "20");

	struct Ast_node* mult = ast_node_get(stmts, 3);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, ast_type_mult, "mult");

	struct Ast_node* x = ast_node_get(mult, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct Ast_node* y = ast_node_get(mult, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_elseif()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let x::Int64; let y::Int64; if true 10 + 20; x * y elseif true 1; 2 end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");

	struct Ast_node* if_stmt = ast_node_get(cu.root, 2);
	assert_ptr(if_stmt, "ptr if_stmt");
	expect_int_equal(if_stmt->type, ast_type_if, "if");

	struct Ast_node* cb = ast_node_get(if_stmt, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, ast_type_conditional_branch, "conditional branch");

	struct Ast_node* cond = ast_node_get(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "true", "true");

	struct Ast_node* stmts = ast_node_get(cb, 1);
	assert_ptr(stmts, "ptr parse_stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "parse_stmts");

	struct Ast_node* plus = ast_node_get(stmts, 0);
	assert_ptr(plus, "ptr plus");
	expect_int_equal(plus->type, ast_type_plus, "plus");

	struct Ast_node* num0 = ast_node_get(plus, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, ast_type_number, "number 0");
	expect_str(&num0->value, "10", "10");

	struct Ast_node* num1 = ast_node_get(plus, 1);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, ast_type_number, "number 1");
	expect_str(&num1->value, "20", "20");

	struct Ast_node* mult = ast_node_get(stmts, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, ast_type_mult, "mult");

	struct Ast_node* x = ast_node_get(mult, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct Ast_node* y = ast_node_get(mult, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

	struct Ast_node* cb2 = ast_node_get(if_stmt, 1);
	assert_ptr(cb2, "ptr cb2");
	expect_int_equal(cb2->type, ast_type_conditional_branch, "conditional branch cb2");

	struct Ast_node* cond2 = ast_node_get(cb2, 0);
	assert_ptr(cond2, "ptr cond2");
	expect_int_equal(cond2->type, ast_type_boolean, "boolean cond2");
	expect_str(&cond2->value, "true", "true cond2");

	struct Ast_node* stmts2 = ast_node_get(cb2, 1);
	assert_ptr(stmts2, "ptr stmts2");
	expect_int_equal(stmts2->type, ast_type_stmts, "parse_stmts stmts2");

	struct Ast_node* num2 = ast_node_get(stmts2, 0);
	assert_ptr(num2, "ptr num2");
	expect_int_equal(num2->type, ast_type_number, "number num2");
	expect_str(&num2->value, "1", "1 num2");

	struct Ast_node* num3 = ast_node_get(stmts2, 1);
	assert_ptr(num3, "ptr num3");
	expect_int_equal(num3->type, ast_type_number, "number num3");
	expect_str(&num3->value, "2", "2 num3");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_elseif2()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let x::Int64\n"
        "let y::Int64\n"
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
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct Ast_node* if_stmt = ast_node_get(cu.root, 2);
	assert_ptr(if_stmt, "ptr cu.root");
	expect_int_equal(if_stmt->type, ast_type_if, "if if_stmt");

	struct Ast_node* cb = ast_node_get(if_stmt, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, ast_type_conditional_branch, "conditional branch");

	struct Ast_node* cond = ast_node_get(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "true", "true");

	struct Ast_node* stmts = ast_node_get(cb, 1);
	assert_ptr(stmts, "ptr parse_stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "parse_stmts");

	struct Ast_node* plus = ast_node_get(stmts, 0);
	assert_ptr(plus, "ptr plus");
	expect_int_equal(plus->type, ast_type_plus, "plus");

	struct Ast_node* num0 = ast_node_get(plus, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, ast_type_number, "number 0");
	expect_str(&num0->value, "10", "10");

	struct Ast_node* num1 = ast_node_get(plus, 1);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, ast_type_number, "number 1");
	expect_str(&num1->value, "20", "20");

	struct Ast_node* mult = ast_node_get(stmts, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, ast_type_mult, "mult");

	struct Ast_node* x = ast_node_get(mult, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct Ast_node* y = ast_node_get(mult, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

	/* first elseif */
	struct Ast_node* cb2 = ast_node_get(if_stmt, 1);
	assert_ptr(cb2, "ptr cb2");
	expect_int_equal(cb2->type, ast_type_conditional_branch, "conditional branch cb2");

	struct Ast_node* cond2 = ast_node_get(cb2, 0);
	assert_ptr(cond2, "ptr cond2");
	expect_int_equal(cond2->type, ast_type_boolean, "boolean cond2");
	expect_str(&cond2->value, "true", "true cond2");

	struct Ast_node* stmts2 = ast_node_get(cb2, 1);
	assert_ptr(stmts2, "ptr stmts2");
	expect_int_equal(stmts2->type, ast_type_stmts, "parse_stmts stmts2");

	struct Ast_node* num2 = ast_node_get(stmts2, 0);
	assert_ptr(num2, "ptr num2");
	expect_int_equal(num2->type, ast_type_number, "number num2");
	expect_str(&num2->value, "1", "1 num2");

	struct Ast_node* num3 = ast_node_get(stmts2, 1);
	assert_ptr(num3, "ptr num3");
	expect_int_equal(num3->type, ast_type_number, "number num3");
	expect_str(&num3->value, "2", "2 num3");

	/* second elseif */
	struct Ast_node* cb3 = ast_node_get(if_stmt, 2);
	assert_ptr(cb3, "ptr cb3");
	expect_int_equal(cb3->type, ast_type_conditional_branch, "conditional branch cb3");

	struct Ast_node* cond3 = ast_node_get(cb3, 0);
	assert_ptr(cond3, "ptr cond3");
	expect_int_equal(cond3->type, ast_type_boolean, "boolean cond3");
	expect_str(&cond3->value, "true", "true cond3");

	struct Ast_node* stmts3 = ast_node_get(cb3, 1);
	assert_ptr(stmts3, "ptr stmts3");
	expect_int_equal(stmts3->type, ast_type_stmts, "parse_stmts stmts3");

	struct Ast_node* x2 = ast_node_get(stmts3, 0);
	assert_ptr(x2, "ptr x2");
	expect_int_equal(x2->type, ast_type_id, "id x2");
	expect_str(&x2->value, "x", "x x2");

	struct Ast_node* y2 = ast_node_get(stmts3, 1);
	assert_ptr(y2, "ptr y2");
	expect_int_equal(y2->type, ast_type_id, "id y2");
	expect_str(&y2->value, "y", "y y2");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_else()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let x::Int64; let y::Int64; if false 10 else x; y end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct Ast_node* if_stmt = ast_node_get(cu.root, 2);
	assert_ptr(if_stmt, "ptr if_stmt");
	assert_int_equal(if_stmt->type, ast_type_if, "if if_stmt");

	struct Ast_node* cb0 = ast_node_get(if_stmt, 0);
	assert_ptr(cb0, "ptr cb0");
	expect_int_equal(cb0->type, ast_type_conditional_branch, "conditional branch cb0");

	struct Ast_node* cond = ast_node_get(cb0, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "false", "false cond");

	struct Ast_node* stmts0 = ast_node_get(cb0, 1);
	assert_ptr(stmts0, "ptr parse_stmts 0");
	expect_int_equal(stmts0->type, ast_type_stmts, "parse_stmts stmts0");

	struct Ast_node* num = ast_node_get(stmts0, 0);
	assert_ptr(num, "ptr num");
	expect_int_equal(num->type, ast_type_number, "number num");
	expect_str(&num->value, "10", "10 num");

	struct Ast_node* cb1 = ast_node_get(if_stmt, 1);
	assert_ptr(cb1, "ptr cb1");
	expect_int_equal(cb1->type, ast_type_default_branch, "default branch cb1");

	struct Ast_node* stmts1 = ast_node_get(cb1, 0);
	assert_ptr(stmts1, "ptr stmts1");
	expect_int_equal(stmts1->type, ast_type_stmts, "parse_stmts stmts1");

	struct Ast_node* x = ast_node_get(stmts1, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct Ast_node* y = ast_node_get(stmts1, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_else2()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let x::Int64; let y::Int64; if false 10 elseif false 20 else x; y end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct Ast_node* if_stmt = ast_node_get(cu.root, 2);
	assert_ptr(if_stmt, "ptr cu.root");
	assert_int_equal(if_stmt->type, ast_type_if, "if");

	/* if */
	struct Ast_node* cb0 = ast_node_get(if_stmt, 0);
	assert_ptr(cb0, "ptr cb0");
	expect_int_equal(cb0->type, ast_type_conditional_branch, "conditional branch cb0");

	struct Ast_node* cond0 = ast_node_get(cb0, 0);
	assert_ptr(cond0, "ptr cond0");
	expect_int_equal(cond0->type, ast_type_boolean, "boolean cond0");
	expect_str(&cond0->value, "false", "false cond0");

	struct Ast_node* stmts0 = ast_node_get(cb0, 1);
	assert_ptr(stmts0, "ptr parse_stmts 0");
	expect_int_equal(stmts0->type, ast_type_stmts, "parse_stmts stmts0");

	struct Ast_node* num0 = ast_node_get(stmts0, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, ast_type_number, "number num0");
	expect_str(&num0->value, "10", "10 num0");

	/* elseif */
	struct Ast_node* cb1 = ast_node_get(if_stmt, 1);
	assert_ptr(cb1, "ptr cb1");
	expect_int_equal(cb1->type, ast_type_conditional_branch, "conditional branch cb1");

	struct Ast_node* cond1 = ast_node_get(cb1, 0);
	assert_ptr(cond1, "ptr cond");
	expect_int_equal(cond1->type, ast_type_boolean, "boolean cond1");
	expect_str(&cond1->value, "false", "false cond1");

	struct Ast_node* stmts1 = ast_node_get(cb1, 1);
	assert_ptr(stmts1, "ptr stmts1");
	expect_int_equal(stmts1->type, ast_type_stmts, "parse_stmts stmts1");

	struct Ast_node* num1 = ast_node_get(stmts1, 0);
	expect_int_equal(num1->type, ast_type_number, "number num1");
	expect_str(&num1->value, "20", "20 num1");

	/* else */
	struct Ast_node* db = ast_node_get(if_stmt, 2);
	assert_ptr(db, "ptr db");
	expect_int_equal(db->type, ast_type_default_branch, "default branch db");

	struct Ast_node* stmts2 = ast_node_get(db, 0);
	assert_ptr(stmts2, "ptr stmts2");
	expect_int_equal(stmts2->type, ast_type_stmts, "parse_stmts stmts2");

	struct Ast_node* x = ast_node_get(stmts2, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct Ast_node* y = ast_node_get(stmts2, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_if_error_expected_expression()
{
	test_name(__func__);

	struct comp_unit cu;

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

	struct comp_unit cu;

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

	struct comp_unit cu;

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

	struct comp_unit cu;

    parse_setup("while true 1 end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct Ast_node* node = ast_node_get(cu.root, 0);
	assert_ptr(node, "ptr node");
	expect_int_equal(node->type, ast_type_while, "while node");
	expect_null(node->tu, "null node->tu");

	struct Ast_node* cond = ast_node_get(node, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "true", "true cond");

	struct Ast_node* stmts = ast_node_get(node, 1);
	assert_ptr(stmts, "ptr parse_stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "parse_stmts stmts");

	struct Ast_node* num = ast_node_get(stmts, 0);
	assert_ptr(num, "ptr num");
	expect_int_equal(num->type, ast_type_number, "number num");
	expect_str(&num->value, "1", "1 num");

    parse_teardown(&cu);
}

void test_parse_while_error_expected_expression()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("while end", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected expression after while");

    parse_teardown(&cu);
}

void test_parse_while_error_expected_end()
{
	test_name(__func__);

	struct comp_unit cu;

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

	struct comp_unit cu;

    parse_setup("for i::Int64 = 0:10 1 end", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	/* for */
	struct Ast_node* node = ast_node_get(cu.root, 0);
	assert_ptr(node, "ptr node");
	expect_int_equal(node->type, ast_type_for_range, "for-range node");

	/* i */
	struct Ast_node* dec = ast_node_get(node, 0);
	assert_ptr(dec, "ptr dec");
	expect_int_equal(dec->type, ast_type_declaration, "declaration dec");

	struct Ast_node* id = ast_node_get(dec, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "i", "i id");

	/* start */
	struct Ast_node* start = ast_node_get(node, 1);
	assert_ptr(start, "ptr start");
	expect_int_equal(start->type, ast_type_number, "number start");
	expect_str(&start->value, "0", "0 start");

	/* end */
	struct Ast_node* end = ast_node_get(node, 2);
	assert_ptr(end, "ptr end");
	expect_int_equal(end->type, ast_type_number, "number end");
	expect_str(&end->value, "10", "10 end");

	/* stmts */
	struct Ast_node* stmts = ast_node_get(node, 3);
	assert_ptr(stmts, "ptr parse_stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "parse_stmts stmts");

	struct Ast_node* stmt0 = ast_node_get(stmts, 0);
	assert_ptr(stmt0, "ptr stmt0");
	expect_int_equal(stmt0->type, ast_type_number, "number stmt0");
	expect_str(&stmt0->value, "1", "1 stmt0");

    parse_teardown(&cu);
}

void test_parse_for_range2()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("for i::Int64 = 0:10 let i::Int64 = 1 end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "duplicate declaration in same scope: i");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_for_iteration()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let list::Vector{Int64}; for i::Int64 in list i end", &cu);
	expect_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	/* for */
	struct Ast_node* node = ast_node_get(cu.root, 1);
	assert_ptr(node, "ptr node");
	expect_int_equal(node->type, ast_type_for_iteration, "for-iteration node");

	/* declaration */
	struct Ast_node* dec = ast_node_get(node, 0);
	assert_ptr(dec, "ptr dec");
	expect_int_equal(dec->type, ast_type_declaration, "declaration dec");

	/* id */
	struct Ast_node* id = ast_node_get(dec, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "i", "i id");

	struct Ast_node* i_tu = ast_node_get(dec, 1);
	assert_ptr(i_tu, "ptr i_tu");

	struct type_def* i_td = i_tu->td;
	assert_ptr(i_td, "ptr i_td");
	expect_int_equal(i_td->type, type_integer, "integer i_td");
	expect_str(&i_td->name, "Int64", "Int64 i_td");

	/* expr */
	struct Ast_node* expr = ast_node_get(node, 1);
	assert_ptr(expr, "ptr expr");
	expect_int_equal(expr->type, ast_type_id, "id expr");
	expect_str(&expr->value, "list", "id list");

	/* stmts */
	struct Ast_node* stmts0 = ast_node_get(node, 2);
	assert_ptr(stmts0, "ptr stmts0");
	expect_int_equal(stmts0->type, ast_type_stmts, "parse_stmts stmts0");

	/* i */
	struct Ast_node* id2 = ast_node_get(stmts0, 0);
	assert_ptr(id2, "ptr id2");
	expect_int_equal(id2->type, ast_type_id, "id id2");
	expect_str(&id2->value, "i", "i id2");

    parse_teardown(&cu);
}

void test_parse_for_iteration2()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let list::Vector{Int64}; for i::Int64 in list let i::Int64 = 1 end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "duplicate declaration in same scope: i");

    parse_teardown(&cu);
}

void test_parse_for_iteration_error_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn list() end; for i::Int64 in list() end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "iteration expression has no value");

    parse_teardown(&cu);
}

void test_parse_for_iteration_error_no_child_element()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let list::Vector; for i::Int64 in list end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "iteration expression has no child element");

    parse_teardown(&cu);
}

void test_parse_for_iteration_error_cannot_cast()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let list::Vector{Bool}; for i::Int64 in list end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "cannot cast list element");

    parse_teardown(&cu);
}

void test_parse_for_error_after_declaration()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let list::Vector{Int64}; for i::Int64 end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected '=' or 'in' after for element declaration");

    parse_teardown(&cu);
}

void test_parse_for_error_expected_end()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let list::Vector{Int64}; for i::Int64 = 1:10", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected end");

    parse_teardown(&cu);
}

void test_parse_for_error_expected_range_start()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let list::Vector{Int64}; for i::Int64 =", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected range start");

    parse_teardown(&cu);
}

void test_parse_for_error_expected_colon()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let list::Vector{Int64}; for i::Int64 = 1", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected colon");

    parse_teardown(&cu);
}

void test_parse_for_error_expected_range_end()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let list::Vector{Int64}; for i::Int64 = 1:", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected range end");

    parse_teardown(&cu);
}

void test_parse_for_range_error_start_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; for i::Int64 = foo():10 end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "start range expression has no value");

    parse_teardown(&cu);
}

void test_parse_for_range_error_start_not_numeric()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("for i::Int64 = true:10 end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "start range expression is not numeric");

    parse_teardown(&cu);
}

void test_parse_for_range_error_end_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; for i::Int64 = 1:foo() end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "end range expression has no value");

    parse_teardown(&cu);
}

void test_parse_for_range_error_end_not_numeric()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("for i::Int64 = 1:true end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "end range expression is not numeric");

    parse_teardown(&cu);
}

void test_parse_for_error_expected_iteration_expression()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let list::Vector{Int64}; for i::Int64 in:", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected for iteration expression");

    parse_teardown(&cu);
}

void test_parse_module()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("module math let pi::Float64 = 3.14 end; math.pi", &cu);
	expect_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct Ast_node* module = ast_node_get(cu.root, 0);
	assert_ptr(module, "ptr module");
	expect_int_equal(module->type, ast_type_module, "module module");

	struct Ast_node* id = ast_node_get(module, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "math", "math id");

	struct Ast_node* module_stmts = ast_node_get(module, 1);
	assert_ptr(module_stmts, "ptr module_stmts");
	expect_int_equal(module_stmts->type, ast_type_stmts, "parse_stmts module_stmts");

	struct Ast_node* dot = ast_node_get(cu.root, 1);
	assert_ptr(dot, "ptr dot");
	expect_int_equal(dot->type, ast_type_dot, "dot dot");

	struct Ast_node* tu = dot->tu;
	assert_ptr(tu, "ptr tu");
	expect_int_equal(tu->type, ast_type_type, "type tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_float, "float td");
	expect_str(&td->name, "Float64", "Float64 td");

	struct Ast_node* dot_id_0 = ast_node_get(dot, 0);
	assert_ptr(dot_id_0, "ptr dot_id_0");
	expect_int_equal(dot_id_0->type, ast_type_id, "id dot_id_0");
	expect_str(&dot_id_0->value, "math", "math dot_id_0");

	struct Ast_node* dot_id_1 = ast_node_get(dot, 1);
	assert_ptr(dot_id_1, "ptr dot_id_1");
	expect_int_equal(dot_id_1->type, ast_type_id, "id dot_id_1");
	expect_str(&dot_id_1->value, "pi", "pi dot_id_1");

    parse_teardown(&cu);
}

void test_parse_module_nested()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("module base module math let pi::Float64 = 3.14 end end; base.math.pi", &cu);
	expect_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	/* base */
	struct Ast_node* base_module = ast_node_get(cu.root, 0);
	assert_ptr(base_module, "ptr base_module");
	expect_int_equal(base_module->type, ast_type_module, "module base_module");

	struct Ast_node* base_id = ast_node_get(base_module, 0);
	assert_ptr(base_id, "ptr base_id");
	expect_int_equal(base_id->type, ast_type_id, "id base_id");
	expect_str(&base_id->value, "base", "base id");

	struct Ast_node* base_module_stmts = ast_node_get(base_module, 1);
	assert_ptr(base_module_stmts, "ptr base_module_stmts");
	expect_int_equal(base_module_stmts->type, ast_type_stmts, "parse_stmts base_module_stmts");

	/* math */
	struct Ast_node* math_module = ast_node_get(base_module_stmts, 0);
	assert_ptr(math_module, "ptr math_module");
	expect_int_equal(math_module->type, ast_type_module, "module math_module");

	struct Ast_node* math_id = ast_node_get(math_module, 0);
	assert_ptr(math_id, "ptr math_id");
	expect_int_equal(math_id->type, ast_type_id, "id math_id");
	expect_str(&math_id->value, "math", "math id");

	struct Ast_node* math_module_stmts = ast_node_get(math_module, 1);
	assert_ptr(math_module_stmts, "ptr math_module_stmts");
	expect_int_equal(math_module_stmts->type, ast_type_stmts, "parse_stmts math_module_stmts");

	struct Ast_node* dot0 = ast_node_get(cu.root, 1);
	assert_ptr(dot0, "ptr dot0");
	expect_int_equal(dot0->type, ast_type_dot, "dot dot0");

	struct Ast_node* tu = dot0->tu;
	assert_ptr(tu, "ptr tu");
	expect_int_equal(tu->type, ast_type_type, "type tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_float, "float td");
	expect_str(&td->name, "Float64", "Float64 td");

	struct Ast_node* dot1 = ast_node_get(dot0, 0);
	assert_ptr(dot1, "ptr dot1");
	expect_int_equal(dot1->type, ast_type_dot, "id dot1");

	struct Ast_node* base_id_1 = ast_node_get(dot1, 0);
	assert_ptr(base_id_1, "ptr base_id_1");
	expect_int_equal(base_id_1->type, ast_type_id, "id base_id_1");
	expect_str(&base_id_1->value, "base", "base base_id_1");

	struct Ast_node* math_id_1 = ast_node_get(dot1, 1);
	assert_ptr(math_id_1, "ptr math_id_1");
	expect_int_equal(math_id_1->type, ast_type_id, "id math_id_1");
	expect_str(&math_id_1->value, "math", "math math_id_1");

	struct Ast_node* pi_id = ast_node_get(dot0, 1);
	assert_ptr(pi_id, "ptr pi_id");
	expect_int_equal(pi_id->type, ast_type_id, "id pi_id");
	expect_str(&pi_id->value, "pi", "pi pi_id");

    parse_teardown(&cu);
}

void test_parse_module_expected_identifier()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("module end", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expected identifier after module");

    parse_teardown(&cu);
}

void test_parse_module_expected_end()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("module math", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expected end");

    parse_teardown(&cu);
}

void test_parse_module_duplicate_declaration()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let foo::Int64; module foo end", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "variable already used: foo");

    parse_teardown(&cu);
}

void test_parse_dot_error_expected_term()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("module math let pi::Float64 = 3.14 end; math.", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expected term after dot");

    parse_teardown(&cu);
}

void test_parse_dot_error_left_non_module()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; module math let pi::Float64 = 3.14 end; true.1", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "dot operand is not a module or struct");

    parse_teardown(&cu);
}

void test_parse_dot_error_right_not_identifier()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; module math let pi::Float64 = 3.14 end; math.\"hello\"", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "operand of dot operator not an identifier");

    parse_teardown(&cu);
}

void test_parse_struct()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("struct Person\n"
                "  firstName::[100 const]UInt8\n"
                "  lastName::[100 const]UInt8\n"
                "  age::Int64\n"
                "end\n"
                "let p::Person = Person(\"John\", \"Smith\", 45)\n"
                "p.firstName\n", &cu);
	expect_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	/* struct */
	struct Ast_node* st = ast_node_get(cu.root, 0);
	assert_ptr(st, "ptr st");
	expect_int_equal(st->type, ast_type_struct, "struct st");

	/* firstName */
	struct Ast_node* d0 = ast_node_get(st, 0);
	assert_ptr(d0, "ptr d0");
	expect_int_equal(d0->type, ast_type_declaration, "declaration d0");

	struct Ast_node* id0 = ast_node_get(d0, 0);
	assert_ptr(id0, "ptr id0");
	expect_int_equal(id0->type, ast_type_id, "id id0");
	expect_str(&id0->value, "firstName", "firstName id0");

	struct Ast_node* t0 = ast_node_get(d0, 1);
	assert_ptr(t0, "ptr t0");
	expect_int_equal(t0->type, ast_type_type, "type t0");

	struct type_def* td0 = t0->td;
	assert_ptr(td0, "ptr td0");
	expect_int_equal(td0->type, type_integer, "type td0");
	expect_str(&td0->name, "UInt8", "name td0");

	/* lastName */
	struct Ast_node* d1 = ast_node_get(st, 1);
	assert_ptr(d1, "ptr d1");
	expect_int_equal(d1->type, ast_type_declaration, "declaration d1");

	struct Ast_node* id1 = ast_node_get(d1, 0);
	assert_ptr(id1, "ptr id1");
	expect_int_equal(id1->type, ast_type_id, "id id1");
	expect_str(&id1->value, "lastName", "lastName id1");

	struct Ast_node* t1 = ast_node_get(d1, 1);
	assert_ptr(t1, "ptr t0");
	expect_int_equal(t1->type, ast_type_type, "type t1");

	struct type_def* td1 = t1->td;
	assert_ptr(td1, "ptr td1");
	expect_int_equal(td1->type, type_integer, "type td1");
	expect_str(&td1->name, "UInt8", "name td1");

	/* age */
	struct Ast_node* d2 = ast_node_get(st, 2);
	assert_ptr(d2, "ptr d2");
	expect_int_equal(d2->type, ast_type_declaration, "declaration d2");

	struct Ast_node* id2 = ast_node_get(d2, 0);
	assert_ptr(id2, "ptr id2");
	expect_int_equal(id2->type, ast_type_id, "id id2");
	expect_str(&id2->value, "age", "age id2");

	struct Ast_node* t2 = ast_node_get(d2, 1);
	assert_ptr(t2, "ptr t2");
	expect_int_equal(t2->type, ast_type_type, "type t2");

	struct type_def* td2 = t2->td;
	assert_ptr(td2, "ptr td2");
	expect_int_equal(td2->type, type_integer, "integer td2");
	expect_str(&td2->name, "Int64", "Int64 td2");

    /* let */
	struct Ast_node* let = ast_node_get(cu.root, 1);
	assert_ptr(let, "ptr let");
	expect_int_equal(let->type, ast_type_let, "type let");

	struct Ast_node* let_lseq = ast_node_get(let, 0);
	assert_ptr(let_lseq, "ptr let_lseq");
	expect_int_equal(let_lseq->type, ast_type_let_lseq, "let_lseq let_lseq");

	struct Ast_node* let_type = ast_node_get(let, 1);
	assert_ptr(let_type, "ptr let_type");
	expect_int_equal(let_type->type, ast_type_type, "type let_type");

    struct type_def* td = let_type->td;
    assert_ptr(td, "ptr td");
    expect_int_equal(td->type, type_struct, "struct td");
    expect_str(&td->name, "Person", "Person td");

    struct Ast_node* let_rseq = ast_node_get(let, 2);
    assert_ptr(let_rseq, "ptr let_rseq");
    expect_int_equal(let_rseq->type, ast_type_let_rseq, "let_rseq let_rseq");

	struct Ast_node* p = ast_node_get(let_lseq, 0);
	assert_ptr(p, "ptr p");
	expect_int_equal(p->type, ast_type_id, "id p");
	expect_str(&p->value, "p", "p p");

	/* constructor call */
	struct Ast_node* call = ast_node_get(let_rseq, 0);
	assert_ptr(call, "ptr call");
	expect_int_equal(call->type, ast_type_call, "call call");

	struct Ast_node* call_id = ast_node_get(call, 0);
	assert_ptr(call_id, "ptr call_id");
	expect_int_equal(call_id->type, ast_type_id, "id call_id");
	expect_str(&call_id->value, "Person", "Person call_id");

	struct Ast_node* cseq = ast_node_get(call, 1);
	assert_ptr(cseq, "ptr cseq");
	expect_int_equal(cseq->type, ast_type_cseq, "cseq cseq");

	struct Ast_node* a0 = ast_node_get(cseq, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, ast_type_string, "string a0");
	expect_str(&a0->value, "John", "John a0");

	struct Ast_node* a1 = ast_node_get(cseq, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, ast_type_string, "string a1");
	expect_str(&a1->value, "Smith", "Smith a1");

	struct Ast_node* a2 = ast_node_get(cseq, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, ast_type_number, "string a2");
	expect_str(&a2->value, "45", "45 a2");

    parse_teardown(&cu);
}

void test_parse_struct_error_not_field()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup(
            "struct Person\n"
            "  firstName::[100 const]UInt8\n"
            "  lastName::[100 const]UInt8\n"
            "  age::Int64\n"
            "end\n"
            "let p::Person = Person(\"John\", \"Smith\", 45)\n"
            "p.abc\n",
            &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "variable not a field of struct: abc");

    parse_teardown(&cu);
}

void test_parse_struct_error_expected_identifier()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("struct end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expected identifier");

    parse_teardown(&cu);
}

void test_parse_struct_error_expected_end()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("struct Person", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expected end");

    parse_teardown(&cu);
}

void test_parse_struct_error_expected_end2()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("struct Person x::Int64", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "expected end");

    parse_teardown(&cu);
}

void test_parse_struct_error_duplicate()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let Person::Int64 = 1; struct Person end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "valid");
	expect_source_error(&cu.el, "duplicate variable in scope: Person");

    parse_teardown(&cu);
}

void test_parse_stmts_newline_for_range()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("for\ni\n::\nInt64\n=\n0\n:\n10 i end", &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_stmts_newline_for_iteration()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let v::Vector{Int64}; for\nx\n::\nInt64\nin\nv x end", &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_let()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup("let a::Int32", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    assert_ptr(cu.root, "ptr cu.root");
    assert_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

    struct Ast_node* let = ast_node_get(cu.root, 0);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, ast_type_let, "type let");

    struct Ast_node* let_lseq = ast_node_get(let, 0);
    assert_ptr(let_lseq, "ptr let_lseq");
    expect_int_equal(let_lseq->type, ast_type_let_lseq, "type let_lseq");

    struct Ast_node* id = ast_node_get(let_lseq, 0);
    assert_ptr(id, "ptr id");
    expect_int_equal(id->type, ast_type_id, "id id");
    expect_str(&id->value, "a", "a");

    struct Ast_node* tu = ast_node_get(let, 1);
    assert_ptr(tu, "ptr tu");

    struct type_def* td = tu->td;
    assert_ptr(td, "ptr td");
    expect_str(&td->name, "Int32", "Int32 td");

    /* destroy ps{} cu.root cu.root{} */
    parse_teardown(&cu);
}

void test_parse_let2()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup("let a::Int32 = 1", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "parse valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

    struct Ast_node* let = ast_node_get(cu.root, 0);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, ast_type_let, "type let");

    struct Ast_node* let_lseq = ast_node_get(let, 0);
    assert_ptr(let_lseq, "ptr let_lseq");
    expect_int_equal(let_lseq->type, ast_type_let_lseq, "type let_lseq");

    struct Ast_node* id = ast_node_get(let_lseq, 0);
    assert_ptr(id, "ptr id");
    expect_int_equal(id->type, ast_type_id, "id");
    expect_str(&id->value, "a", "a");

    struct Ast_node* tu = ast_node_get(let, 1);
    assert_ptr(tu, "ptr tu");

    struct type_def* td = tu->td;
    assert_ptr(td, "ptr td");
    expect_str(&td->name, "Int32", "Int32 td");

    struct Ast_node* let_rseq = ast_node_get(let, 2);
    assert_ptr(let_rseq, "ptr let_rseq");
    expect_int_equal(let_rseq->type, ast_type_let_rseq, "type let_rseq");

    struct Ast_node* value = ast_node_get(let_rseq, 0);
    assert_ptr(value, "ptr value");
    expect_int_equal(value->type, ast_type_number, "number");
    expect_str(&value->value, "1", "1");

    /* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_let3()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a,b::Int32 = 1,2", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "parse valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

    struct Ast_node* let = ast_node_get(cu.root, 0);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, ast_type_let, "type let");

    struct Ast_node* let_lseq = ast_node_get(let, 0);
    assert_ptr(let_lseq, "ptr let_lseq");
    expect_int_equal(let_lseq->type, ast_type_let_lseq, "type let_lseq");

    struct Ast_node* id0 = ast_node_get(let_lseq, 0);
    assert_ptr(id0, "ptr id0");
    expect_int_equal(id0->type, ast_type_id, "id id0");
    expect_str(&id0->value, "a", "value id0");

    struct Ast_node* tu = ast_node_get(let, 1);
    assert_ptr(tu, "ptr tu");

    struct type_def* td = tu->td;
    assert_ptr(td, "ptr td");
    expect_str(&td->name, "Int32", "Int32 td");

    struct Ast_node* id2 = ast_node_get(let_lseq, 1);
    assert_ptr(id2, "ptr id2");
    expect_int_equal(id2->type, ast_type_id, "type id2");
    expect_str(&id2->value, "b", "value id2");

    struct Ast_node* let_rseq = ast_node_get(let, 2);
    assert_ptr(let_rseq, "ptr let_rseq");
    expect_int_equal(let_rseq->type, ast_type_let_rseq, "type let_rseq");

    struct Ast_node* value0 = ast_node_get(let_rseq, 0);
    assert_ptr(value0, "ptr value0");
    expect_int_equal(value0->type, ast_type_number, "type value0");
    expect_str(&value0->value, "1", "value value0");

    struct Ast_node* value1 = ast_node_get(let_rseq, 1);
    assert_ptr(value1, "ptr value1");
    expect_int_equal(value1->type, ast_type_number, "type value1");
    expect_str(&value1->value, "2", "value value1");

    parse_teardown(&cu);
}

void test_parse_let_expected_declaration()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup("let", &cu);
    assert_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "expected variable(s) after let");

    /* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_extern()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("extern foo(a::Int64)\n"
                "foo(1)\n",
                &cu);
    assert_no_errors(&cu.el);
    assert_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, ast_type_stmts, "type cu.root");

    struct Ast_node* f = ast_node_get(cu.root, 0);
    assert_ptr(f, "ptr f");
    expect_int_equal(f->type, ast_type_extern, "type f");

    struct Ast_node* tu = f->tu;
    assert_ptr(tu, "ptr tu");

    struct type_def* td = tu->td;
    assert_ptr(td, "ptr td");
    expect_int_equal(td->type, type_function, "type td");
    expect_str(&td->name, "Function", "name td");

    struct Ast_node* proto = ast_node_get(f, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, ast_type_prototype, "type proto");

    struct Ast_node* fid = ast_node_get(proto, 0);
    assert_ptr(fid, "ptr fid");
    expect_int_equal(fid->type, ast_type_id, "id");

    struct Ast_node* dseq = ast_node_get(proto, 1);
    assert_ptr(dseq, "ptr dseq");
    expect_int_equal(dseq->type, ast_type_dseq, "dseq dseq");

    struct Ast_node* dret = ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, ast_type_dret, "dret dret");

    struct Ast_node* call = ast_node_get(cu.root, 1);
    expect_ptr(call, "ptr call");
    expect_int_equal(call->type, ast_type_call, "type call");

    parse_teardown(&cu);
}

void test_parse_stmts_mut() {
    test_name(__func__);
    struct comp_unit cu;

    parse_setup("let mut x::Int64 = 10\n"
                "x = 5\n"
                "x",
                &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "parse_setup valid");

    expect_int_equal(cu.root->type, ast_type_stmts, "type cu.root");

    struct Ast_node *let = ast_node_get(cu.root, 0);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, ast_type_let, "type let");

    struct Ast_node *let_lseq = ast_node_get(let, 0);
    assert_ptr(let_lseq, "ptr let_lseq");
    expect_int_equal(let_lseq->type, ast_type_let_lseq, "type let_lseq");

    struct Ast_node *let_lhs = ast_node_get(let_lseq, 0);
    assert_ptr(let_lhs, "ptr lhs");
    expect_int_equal(let_lhs->type, ast_type_id, "type lhs");
    expect_true(let_lhs->to.is_mut, "is_mut lhs");

    struct Ast_node *assign = ast_node_get(cu.root, 1);
    assert_ptr(assign, "ptr assign");
    expect_int_equal(assign->type, ast_type_assign, "type assign");

    assert_ptr(assign->tu, "ptr assign->tu");
    expect_true(assign->tu->to.is_mut, "is_mut assign");

    struct Ast_node *x = ast_node_get(assign, 0);
    assert_ptr(x, "ptr x");
    expect_int_equal(x->type, ast_type_id, "type id");

    expect_ptr(x->tu, "ptr x->tu");
    expect_true(x->tu->to.is_mut, "is_mut x->tu");

    parse_teardown(&cu);
}

void test_parse_stmts_error_mut()
{
    test_name(__func__);
    struct comp_unit cu;

    parse_setup("let x::Int64 = 10\n"
                "x = 5\n"
                "x",
                &cu);
    expect_false(cu.valid, "parse_setup valid");
    expect_has_errors(&cu.el);
    struct error* e = expect_source_error(&cu.el, "immutable variable changed in assignment");
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
	test_parse_module();
	test_parse_module_nested();
	test_parse_module_expected_identifier();
	test_parse_module_expected_end();
	test_parse_module_duplicate_declaration();
	test_parse_dot_error_expected_term();
	test_parse_dot_error_left_non_module();
	test_parse_dot_error_right_not_identifier();
	test_parse_struct();
	test_parse_struct_error_not_field();
	test_parse_struct_error_expected_identifier();
	test_parse_struct_error_expected_end();
	test_parse_struct_error_expected_end2();
	test_parse_struct_error_duplicate();
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
