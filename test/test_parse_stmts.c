#include "zinc/unit_test.h"
#include "assert_compiler.h"
#include "alba/parse_tools.h"
#include "alba/dag.h"
#include "test_parse.h"
#include "alba/input.h"

/* dynamic-output-none */
void test_parse_assign()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("a = 1", &ps, &root);

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_assign, "assign");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "a", "a");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_assign2()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("a = 1 + 2", &ps, &root);

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_assign, "assign");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "a", "a");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_plus, "plus");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_number, "number");
	expect_str(&left2->value, "1", "1");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right");
	expect_int_equal(right2->type, dag_type_number, "number2");
	expect_str(&right2->value, "2", "2");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_stmts()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("i + 1\nx * 1", &ps, &root);

	assert_ptr(root, "ptr1");
	expect_int_equal(root->type, dag_type_stmts, "stmts");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_plus, "plus");

	struct dag_node* left2 = dag_get_child(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_id, "id");
	expect_str(&left2->value, "i", "i");

	struct dag_node* right = dag_get_child(left, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	struct dag_node* right2 = dag_get_child(root, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_mult, "mult");

	struct dag_node* left3 = dag_get_child(right2, 0);
	assert_ptr(left3, "left3");
	expect_int_equal(left3->type, dag_type_id, "id");
	expect_str(&left3->value, "x", "x");

	struct dag_node* right3 = dag_get_child(right2, 1);
	assert_ptr(right3, "right3");
	expect_int_equal(right3->type, dag_type_number, "number2");
	expect_str(&right3->value, "1", "1");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_stmts2()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("x+1\n5-4\n", &ps, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_stmts, "stmts");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_plus, "plus a");

	struct dag_node* b = dag_get_child(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	struct dag_node* c = dag_get_child(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, dag_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	struct dag_node* d = dag_get_child(root, 1);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_minus, "minus d");

	struct dag_node* e = dag_get_child(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, dag_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	struct dag_node* f = dag_get_child(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_stmts3()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("x+1\n5-4\ny+z", &ps, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_stmts, "stmts");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_plus, "plus a");

	struct dag_node* b = dag_get_child(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	struct dag_node* c = dag_get_child(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, dag_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	struct dag_node* d = dag_get_child(root, 1);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_minus, "minus d");

	struct dag_node* e = dag_get_child(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, dag_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	struct dag_node* f = dag_get_child(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

	struct dag_node* g = dag_get_child(root, 2);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, dag_type_plus, "plus g");

	struct dag_node* h = dag_get_child(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, dag_type_id, "id h");
	expect_str(&h->value, "y", "y h");

	struct dag_node* i = dag_get_child(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, dag_type_id, "id i");
	expect_str(&i->value, "z", "z c");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_stmts4()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("x+1;5-4;y+z", &ps, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_stmts, "stmts");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_plus, "plus a");

	struct dag_node* b = dag_get_child(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	struct dag_node* c = dag_get_child(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, dag_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	struct dag_node* d = dag_get_child(root, 1);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_minus, "minus d");

	struct dag_node* e = dag_get_child(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, dag_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	struct dag_node* f = dag_get_child(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

	struct dag_node* g = dag_get_child(root, 2);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, dag_type_plus, "plus g");

	struct dag_node* h = dag_get_child(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, dag_type_id, "id h");
	expect_str(&h->value, "y", "y h");

	struct dag_node* i = dag_get_child(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, dag_type_id, "id i");
	expect_str(&i->value, "z", "z c");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_stmts5()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("if true 1 else 2 end", &ps, &root);

	struct dag_node* if_stmt = check_stmts(root, "stmts root");
	assert_ptr(if_stmt, "ptr if_stmt");
	expect_int_equal(if_stmt->type, dag_type_if, "if if_stmt");

	struct dag_node* cb = dag_get_child(if_stmt, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, dag_type_conditional_branch, "conditional branch cb");

	struct dag_node* cond = dag_get_child(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, dag_type_id, "id cond");
	expect_str(&cond->value, "true", "true cond");

	struct dag_node* stmts_cb = dag_get_child(cb, 1);
	assert_ptr(stmts_cb, "ptr stmts_cb");
	expect_int_equal(stmts_cb->type, dag_type_stmts, "stmts stmts_cb");

	struct dag_node* num0 = dag_get_child(stmts_cb, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, dag_type_number, "number num0");
	expect_str(&num0->value, "1", "1 num0");

	struct dag_node* db = dag_get_child(if_stmt, 1);
	assert_ptr(db, "ptr db");
	expect_int_equal(db->type, dag_type_default_branch, "default branch db");

	struct dag_node* stmts_db = dag_get_child(db, 0);
	assert_ptr(stmts_db, "ptr stmts_db");
	expect_int_equal(stmts_db->type, dag_type_stmts, "stmts stmts_db");

	struct dag_node* num1 = dag_get_child(stmts_db, 0);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, dag_type_number, "number num1");
	expect_str(&num1->value, "2", "2 num1");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_function()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("function foo()\nx+1\n5+4\nend", &ps, &root);

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_function, "function");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_id, "id");

	struct dag_node* dseq = dag_get_child(root, 1);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, dag_type_dseq, "dseq dseq");

	struct dag_node* b = dag_get_child(root, 2);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_stmts, "stmts");

	struct dag_node* c = dag_get_child(root, 3);
	assert_null(c, "ptr c");

	struct dag_node* d = dag_get_child(b, 0);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_plus, "plus");

	struct dag_node* e = dag_get_child(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, dag_type_id, "id e");
	expect_str(&e->value, "x", "x");

	struct dag_node* f = dag_get_child(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_number, "number f");
	expect_str(&f->value, "1", "1");

	struct dag_node* g = dag_get_child(b, 1);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, dag_type_plus, "plus");

	struct dag_node* h = dag_get_child(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, dag_type_number, "number h");
	expect_str(&h->value, "5", "5");

	struct dag_node* i = dag_get_child(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, dag_type_number, "number i");
	expect_str(&i->value, "4", "4");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_function2()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	enum result r;

	/* allocate ps{} root root{} */
	r = parse_setup("function foo(x::Int64)\nx+1\n5+4\nend", &ps, &root);
	assert_ok(r, "parse");

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_function, "function");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_id, "id");

	struct dag_node* seq = dag_get_child(root, 1);
	assert_ptr(seq, "ptr seq");
	expect_int_equal(seq->type, dag_type_dseq, "seq");

	struct dag_node* seq_dec = dag_get_child(seq, 0);
	assert_ptr(seq_dec, "ptr seq_dec");
	expect_int_equal(seq_dec->type, dag_type_declaration, "declaration seq_dec");

	struct dag_node* dec_a = dag_get_child(seq_dec, 0);
	assert_ptr(dec_a, "ptr dec_a");
	expect_int_equal(dec_a->type, dag_type_id, "id dec_a");
	expect_str(&dec_a->value, "x", "x dec_a");

	struct dag_node* dec_b = dag_get_child(seq_dec, 1);
	assert_ptr(dec_b, "ptr dec_b");
	expect_int_equal(dec_b->type, dag_type_type, "type dec_b");
	expect_str(&dec_b->value, "Int64", "Int64 dec_b");

	struct dag_node* b = dag_get_child(root, 2);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_stmts, "stmts");

	struct dag_node* c = dag_get_child(root, 3);
	assert_null(c, "ptr c");

	struct dag_node* d = dag_get_child(b, 0);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_plus, "plus");

	struct dag_node* e = dag_get_child(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, dag_type_id, "id e");
	expect_str(&e->value, "x", "x");

	struct dag_node* f = dag_get_child(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_number, "number f");
	expect_str(&f->value, "1", "1");

	struct dag_node* g = dag_get_child(b, 1);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, dag_type_plus, "plus");

	struct dag_node* h = dag_get_child(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, dag_type_number, "number h");
	expect_str(&h->value, "5", "5");

	struct dag_node* i = dag_get_child(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, dag_type_number, "number i");
	expect_str(&i->value, "4", "4");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_function3()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("function foo(x, y)\nx+1\n5+4\nend", &ps, &root);

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_function, "function");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_id, "id");

	struct dag_node* seq = dag_get_child(root, 1);
	assert_ptr(seq, "ptr seq");
	expect_int_equal(seq->type, dag_type_dseq, "seq");

	struct dag_node* seq_a = dag_get_child(seq, 0);
	assert_ptr(seq_a, "ptr seq a");
	expect_int_equal(seq_a->type, dag_type_declaration, "declaration seq_a");

	struct dag_node* dec_id0 = dag_get_child(seq_a, 0);
	assert_ptr(dec_id0, "ptr dec_id0");
	expect_int_equal(dec_id0->type, dag_type_id, "id dec_id0");

	struct dag_node* seq_b = dag_get_child(seq, 1);
	assert_ptr(seq_b, "ptr seq b");
	expect_int_equal(seq_b->type, dag_type_declaration, "declaration seq_b");

	struct dag_node* dec_id1 = dag_get_child(seq_b, 0);
	assert_ptr(dec_id1, "ptr dec_id1");
	expect_int_equal(dec_id1->type, dag_type_id, "id dec_id1");

	struct dag_node* b = dag_get_child(root, 2);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_stmts, "stmts");

	struct dag_node* c = dag_get_child(root, 3);
	assert_null(c, "ptr c");

	struct dag_node* d = dag_get_child(b, 0);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_plus, "plus");

	struct dag_node* e = dag_get_child(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, dag_type_id, "id e");
	expect_str(&e->value, "x", "x");

	struct dag_node* f = dag_get_child(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_number, "number f");
	expect_str(&f->value, "1", "1");

	struct dag_node* g = dag_get_child(b, 1);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, dag_type_plus, "plus");

	struct dag_node* h = dag_get_child(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, dag_type_number, "number h");
	expect_str(&h->value, "5", "5");

	struct dag_node* i = dag_get_child(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, dag_type_number, "number i");
	expect_str(&i->value, "4", "4");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_function4()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("function foo(x, y, z)\nx+1\n5+4\nend", &ps, &root);

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_function, "function");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_id, "id");

	struct dag_node* seq = dag_get_child(root, 1);
	assert_ptr(seq, "ptr seq");
	expect_int_equal(seq->type, dag_type_dseq, "seq");

	struct dag_node* seq_a = dag_get_child(seq, 0);
	assert_ptr(seq_a, "ptr seq a");
	expect_int_equal(seq_a->type, dag_type_declaration, "declaration seq_a");

	struct dag_node* dec_a = dag_get_child(seq_a, 0);
	assert_ptr(dec_a, "ptr dec_a");
	expect_int_equal(dec_a->type, dag_type_id, "id dec_a");

	struct dag_node* seq_b = dag_get_child(seq, 1);
	assert_ptr(seq_b, "ptr seq b");
	expect_int_equal(seq_b->type, dag_type_declaration, "declaration seq_b");

	struct dag_node* dec_b = dag_get_child(seq_b, 0);
	assert_ptr(dec_b, "ptr dec_b");
	expect_int_equal(dec_b->type, dag_type_id, "id dec_b");

	struct dag_node* seq_c = dag_get_child(seq, 2);
	assert_ptr(seq_c, "ptr seq c");
	expect_int_equal(seq_c->type, dag_type_declaration, "declaration seq_c");

	struct dag_node* dec_c = dag_get_child(seq_c, 0);
	assert_ptr(dec_c, "ptr dec_c");
	expect_int_equal(dec_c->type, dag_type_id, "id dec_c");

	struct dag_node* b = dag_get_child(root, 2);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_stmts, "stmts");

	struct dag_node* c = dag_get_child(root, 3);
	assert_null(c, "ptr c");

	struct dag_node* d = dag_get_child(b, 0);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_plus, "plus");

	struct dag_node* e = dag_get_child(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, dag_type_id, "id e");
	expect_str(&e->value, "x", "x");

	struct dag_node* f = dag_get_child(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, dag_type_number, "number f");
	expect_str(&f->value, "1", "1");

	struct dag_node* g = dag_get_child(b, 1);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, dag_type_plus, "plus");

	struct dag_node* h = dag_get_child(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, dag_type_number, "number h");
	expect_str(&h->value, "5", "5");

	struct dag_node* i = dag_get_child(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, dag_type_number, "number i");
	expect_str(&i->value, "4", "4");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_call()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("foo() + 10", &ps, &root);

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_call, "call");

	struct dag_node* b = dag_get_child(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_id, "id");
	expect_str(&b->value, "foo", "foo");

	struct dag_node* c = dag_get_child(a, 1);
	assert_null(c, "null c");

	struct dag_node* d = dag_get_child(root, 1);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_number, "number");
	expect_str(&d->value, "10", "10");

	struct dag_node* e = dag_get_child(root, 2);
	assert_null(e, "null e");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_call2()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("foo(x) + 10", &ps, &root);

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_call, "call");

	struct dag_node* b = dag_get_child(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_id, "id");
	expect_str(&b->value, "foo", "foo");

	struct dag_node* cseq = dag_get_child(a, 1);
	assert_ptr(cseq, "ptr cseq");
	expect_int_equal(cseq->type, dag_type_cseq, "cseq");

	struct dag_node* cseq_a = dag_get_child(cseq, 0);
	assert_ptr(cseq_a, "ptr cseq_a");
	expect_int_equal(cseq_a->type, dag_type_id, "cseq_a");
	expect_str(&cseq_a->value, "x", "x cseq_a");

	struct dag_node* cseq_b = dag_get_child(cseq, 1);
	assert_null(cseq_b, "null cseq_b");

	struct dag_node* c = dag_get_child(a, 2);
	assert_null(c, "null c");

	struct dag_node* d = dag_get_child(root, 1);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_number, "number");
	expect_str(&d->value, "10", "10");

	struct dag_node* e = dag_get_child(root, 2);
	assert_null(e, "null e");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_call3()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("foo(x,y) + 10", &ps, &root);

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_call, "call");

	struct dag_node* b = dag_get_child(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_id, "id");
	expect_str(&b->value, "foo", "foo");

	struct dag_node* cseq = dag_get_child(a, 1);
	assert_ptr(cseq, "ptr cseq");
	expect_int_equal(cseq->type, dag_type_cseq, "cseq");

	struct dag_node* cseq_a = dag_get_child(cseq, 0);
	assert_ptr(cseq_a, "ptr cseq_a");
	expect_int_equal(cseq_a->type, dag_type_id, "cseq_a");
	expect_str(&cseq_a->value, "x", "x cseq_a");

	struct dag_node* cseq_b = dag_get_child(cseq, 1);
	assert_ptr(cseq_b, "ptr cseq_b");
	expect_int_equal(cseq_b->type, dag_type_id, "cseq_b");
	expect_str(&cseq_b->value, "y", "y cseq_b");

	struct dag_node* cseq_c = dag_get_child(cseq, 2);
	assert_null(cseq_c, "null cseq_c");

	struct dag_node* c = dag_get_child(a, 2);
	assert_null(c, "null c");

	struct dag_node* d = dag_get_child(root, 1);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_number, "number");
	expect_str(&d->value, "10", "10");

	struct dag_node* e = dag_get_child(root, 2);
	assert_null(e, "null e");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_call4()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("foo(x,y,1) + 10", &ps, &root);

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* a = dag_get_child(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_call, "call");

	struct dag_node* b = dag_get_child(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_id, "id");
	expect_str(&b->value, "foo", "foo");

	struct dag_node* cseq = dag_get_child(a, 1);
	assert_ptr(cseq, "ptr cseq");
	expect_int_equal(cseq->type, dag_type_cseq, "cseq");

	struct dag_node* cseq_a = dag_get_child(cseq, 0);
	assert_ptr(cseq_a, "ptr cseq_a");
	expect_int_equal(cseq_a->type, dag_type_id, "cseq_a");
	expect_str(&cseq_a->value, "x", "x cseq_a");

	struct dag_node* cseq_b = dag_get_child(cseq, 1);
	assert_ptr(cseq_b, "ptr cseq_b");
	expect_int_equal(cseq_b->type, dag_type_id, "cseq_b");
	expect_str(&cseq_b->value, "y", "y cseq_b");

	struct dag_node* cseq_c = dag_get_child(cseq, 2);
	assert_ptr(cseq_c, "ptr cseq_c");
	expect_int_equal(cseq_c->type, dag_type_number, "cseq_c");
	expect_str(&cseq_c->value, "1", "1 cseq_c");

	struct dag_node* cseq_d = dag_get_child(cseq, 3);
	assert_null(cseq_d, "null cseq_d");

	struct dag_node* c = dag_get_child(a, 2);
	assert_null(c, "null c");

	struct dag_node* d = dag_get_child(root, 1);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, dag_type_number, "number");
	expect_str(&d->value, "10", "10");

	struct dag_node* e = dag_get_child(root, 2);
	assert_null(e, "null e");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_if()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("if true\n10 + 20\nx * y\nend", &ps, &root);

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_if, "if");

	struct dag_node* cb = dag_get_child(root, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, dag_type_conditional_branch, "conditional branch");

	struct dag_node* cond = dag_get_child(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, dag_type_id, "id");
	expect_str(&cond->value, "true", "true");

	struct dag_node* stmts = dag_get_child(cb, 1);
	assert_ptr(stmts, "ptr stmts");
	expect_int_equal(stmts->type, dag_type_stmts, "stmts");

	struct dag_node* plus = dag_get_child(stmts, 0);
	assert_ptr(plus, "ptr plus");
	expect_int_equal(plus->type, dag_type_plus, "plus");

	struct dag_node* num0 = dag_get_child(plus, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, dag_type_number, "number 0");
	expect_str(&num0->value, "10", "10");

	struct dag_node* num1 = dag_get_child(plus, 1);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, dag_type_number, "number 1");
	expect_str(&num1->value, "20", "20");

	struct dag_node* mult = dag_get_child(stmts, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, dag_type_mult, "mult");

	struct dag_node* x = dag_get_child(mult, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, dag_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct dag_node* y = dag_get_child(mult, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, dag_type_id, "id y");
	expect_str(&y->value, "y", "y");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_elseif()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("if true\n10 + 20\nx * y\nelseif true\n1\n2\nend", &ps, &root);

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_if, "if");

	struct dag_node* cb = dag_get_child(root, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, dag_type_conditional_branch, "conditional branch");

	struct dag_node* cond = dag_get_child(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, dag_type_id, "id");
	expect_str(&cond->value, "true", "true");

	struct dag_node* stmts = dag_get_child(cb, 1);
	assert_ptr(stmts, "ptr stmts");
	expect_int_equal(stmts->type, dag_type_stmts, "stmts");

	struct dag_node* plus = dag_get_child(stmts, 0);
	assert_ptr(plus, "ptr plus");
	expect_int_equal(plus->type, dag_type_plus, "plus");

	struct dag_node* num0 = dag_get_child(plus, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, dag_type_number, "number 0");
	expect_str(&num0->value, "10", "10");

	struct dag_node* num1 = dag_get_child(plus, 1);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, dag_type_number, "number 1");
	expect_str(&num1->value, "20", "20");

	struct dag_node* mult = dag_get_child(stmts, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, dag_type_mult, "mult");

	struct dag_node* x = dag_get_child(mult, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, dag_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct dag_node* y = dag_get_child(mult, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, dag_type_id, "id y");
	expect_str(&y->value, "y", "y");

	struct dag_node* cb2 = dag_get_child(root, 1);
	assert_ptr(cb2, "ptr cb2");
	expect_int_equal(cb2->type, dag_type_conditional_branch, "conditional branch cb2");

	struct dag_node* cond2 = dag_get_child(cb2, 0);
	assert_ptr(cond2, "ptr cond2");
	expect_int_equal(cond2->type, dag_type_id, "id cond2");
	expect_str(&cond2->value, "true", "true cond2");

	struct dag_node* stmts2 = dag_get_child(cb2, 1);
	assert_ptr(stmts2, "ptr stmts2");
	expect_int_equal(stmts2->type, dag_type_stmts, "stmts stmts2");

	struct dag_node* num2 = dag_get_child(stmts2, 0);
	assert_ptr(num2, "ptr num2");
	expect_int_equal(num2->type, dag_type_number, "number num2");
	expect_str(&num2->value, "1", "1 num2");

	struct dag_node* num3 = dag_get_child(stmts2, 1);
	assert_ptr(num3, "ptr num3");
	expect_int_equal(num3->type, dag_type_number, "number num3");
	expect_str(&num3->value, "2", "2 num3");


	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_elseif2()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("if true\n10 + 20\nx * y\nelseif true\n1\n2\nelseif true\nx\ny\nend", &ps, &root);

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_if, "if");

	struct dag_node* cb = dag_get_child(root, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, dag_type_conditional_branch, "conditional branch");

	struct dag_node* cond = dag_get_child(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, dag_type_id, "id");
	expect_str(&cond->value, "true", "true");

	struct dag_node* stmts = dag_get_child(cb, 1);
	assert_ptr(stmts, "ptr stmts");
	expect_int_equal(stmts->type, dag_type_stmts, "stmts");

	struct dag_node* plus = dag_get_child(stmts, 0);
	assert_ptr(plus, "ptr plus");
	expect_int_equal(plus->type, dag_type_plus, "plus");

	struct dag_node* num0 = dag_get_child(plus, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, dag_type_number, "number 0");
	expect_str(&num0->value, "10", "10");

	struct dag_node* num1 = dag_get_child(plus, 1);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, dag_type_number, "number 1");
	expect_str(&num1->value, "20", "20");

	struct dag_node* mult = dag_get_child(stmts, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, dag_type_mult, "mult");

	struct dag_node* x = dag_get_child(mult, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, dag_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct dag_node* y = dag_get_child(mult, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, dag_type_id, "id y");
	expect_str(&y->value, "y", "y");

	/* first elseif */
	struct dag_node* cb2 = dag_get_child(root, 1);
	assert_ptr(cb2, "ptr cb2");
	expect_int_equal(cb2->type, dag_type_conditional_branch, "conditional branch cb2");

	struct dag_node* cond2 = dag_get_child(cb2, 0);
	assert_ptr(cond2, "ptr cond2");
	expect_int_equal(cond2->type, dag_type_id, "id cond2");
	expect_str(&cond2->value, "true", "true cond2");

	struct dag_node* stmts2 = dag_get_child(cb2, 1);
	assert_ptr(stmts2, "ptr stmts2");
	expect_int_equal(stmts2->type, dag_type_stmts, "stmts stmts2");

	struct dag_node* num2 = dag_get_child(stmts2, 0);
	assert_ptr(num2, "ptr num2");
	expect_int_equal(num2->type, dag_type_number, "number num2");
	expect_str(&num2->value, "1", "1 num2");

	struct dag_node* num3 = dag_get_child(stmts2, 1);
	assert_ptr(num3, "ptr num3");
	expect_int_equal(num3->type, dag_type_number, "number num3");
	expect_str(&num3->value, "2", "2 num3");

	/* second elseif */
	struct dag_node* cb3 = dag_get_child(root, 2);
	assert_ptr(cb3, "ptr cb3");
	expect_int_equal(cb3->type, dag_type_conditional_branch, "conditional branch cb3");

	struct dag_node* cond3 = dag_get_child(cb3, 0);
	assert_ptr(cond3, "ptr cond3");
	expect_int_equal(cond3->type, dag_type_id, "id cond3");
	expect_str(&cond3->value, "true", "true cond3");

	struct dag_node* stmts3 = dag_get_child(cb3, 1);
	assert_ptr(stmts3, "ptr stmts3");
	expect_int_equal(stmts3->type, dag_type_stmts, "stmts stmts3");

	struct dag_node* x2 = dag_get_child(stmts3, 0);
	assert_ptr(x2, "ptr x2");
	expect_int_equal(x2->type, dag_type_id, "id x2");
	expect_str(&x2->value, "x", "x x2");

	struct dag_node* y2 = dag_get_child(stmts3, 1);
	assert_ptr(y2, "ptr y2");
	expect_int_equal(y2->type, dag_type_id, "id y2");
	expect_str(&y2->value, "y", "y y2");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_else()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("if false\n10\nelse\nx\ny\nend", &ps, &root);

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_if, "if");

	struct dag_node* cb0 = dag_get_child(root, 0);
	assert_ptr(cb0, "ptr cb0");
	expect_int_equal(cb0->type, dag_type_conditional_branch, "conditional branch cb0");

	struct dag_node* cond = dag_get_child(cb0, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, dag_type_id, "id cond");
	expect_str(&cond->value, "false", "false cond");

	struct dag_node* stmts0 = dag_get_child(cb0, 1);
	assert_ptr(stmts0, "ptr stmts 0");
	expect_int_equal(stmts0->type, dag_type_stmts, "stmts stmts0");

	struct dag_node* num = dag_get_child(stmts0, 0);
	assert_ptr(num, "ptr num");
	expect_int_equal(num->type, dag_type_number, "number num");
	expect_str(&num->value, "10", "10 num");

	struct dag_node* cb1 = dag_get_child(root, 1);
	assert_ptr(cb1, "ptr cb1");
	expect_int_equal(cb1->type, dag_type_default_branch, "default branch cb1");

	struct dag_node* stmts1 = dag_get_child(cb1, 0);
	assert_ptr(stmts1, "ptr stmts1");
	expect_int_equal(stmts1->type, dag_type_stmts, "stmts stmts1");

	struct dag_node* x = dag_get_child(stmts1, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, dag_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct dag_node* y = dag_get_child(stmts1, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, dag_type_id, "id y");
	expect_str(&y->value, "y", "y");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_else2()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("if false\n10\nelseif false\n20\nelse\nx\ny\nend", &ps, &root);

	root = check_stmts(root, "stmts root");

	/* top */
	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_if, "if");

	/* if */
	struct dag_node* cb0 = dag_get_child(root, 0);
	assert_ptr(cb0, "ptr cb0");
	expect_int_equal(cb0->type, dag_type_conditional_branch, "conditional branch cb0");

	struct dag_node* cond0 = dag_get_child(cb0, 0);
	assert_ptr(cond0, "ptr cond0");
	expect_int_equal(cond0->type, dag_type_id, "id cond0");
	expect_str(&cond0->value, "false", "false cond0");

	struct dag_node* stmts0 = dag_get_child(cb0, 1);
	assert_ptr(stmts0, "ptr stmts 0");
	expect_int_equal(stmts0->type, dag_type_stmts, "stmts stmts0");

	struct dag_node* num0 = dag_get_child(stmts0, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, dag_type_number, "number num0");
	expect_str(&num0->value, "10", "10 num0");

	/* elseif */
	struct dag_node* cb1 = dag_get_child(root, 1);
	assert_ptr(cb1, "ptr cb1");
	expect_int_equal(cb1->type, dag_type_conditional_branch, "conditional branch cb1");

	struct dag_node* cond1 = dag_get_child(cb1, 0);
	assert_ptr(cond1, "ptr cond");
	expect_int_equal(cond1->type, dag_type_id, "id cond1");
	expect_str(&cond1->value, "false", "false cond1");

	struct dag_node* stmts1 = dag_get_child(cb1, 1);
	assert_ptr(stmts1, "ptr stmts1");
	expect_int_equal(stmts1->type, dag_type_stmts, "stmts stmts1");

	struct dag_node* num1 = dag_get_child(stmts1, 0);
	expect_int_equal(num1->type, dag_type_number, "number num1");
	expect_str(&num1->value, "20", "20 num1");

	/* else */
	struct dag_node* db = dag_get_child(root, 2);
	assert_ptr(db, "ptr db");
	expect_int_equal(db->type, dag_type_default_branch, "default branch db");

	struct dag_node* stmts2 = dag_get_child(db, 0);
	assert_ptr(stmts2, "ptr stmts2");
	expect_int_equal(stmts2->type, dag_type_stmts, "stmts stmts2");

	struct dag_node* x = dag_get_child(stmts2, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, dag_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct dag_node* y = dag_get_child(stmts2, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, dag_type_id, "id y");
	expect_str(&y->value, "y", "y");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_while()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("while (true) 1 end", &ps, &root);

	struct dag_node* node = check_stmts(root, "stmts root");
	assert_ptr(node, "ptr node");
	expect_int_equal(node->type, dag_type_while, "while node");

	struct dag_node* cond = dag_get_child(node, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, dag_type_id, "id cond");
	expect_str(&cond->value, "true", "true cond");

	struct dag_node* stmts = dag_get_child(node, 1);
	assert_ptr(stmts, "ptr stmts");
	expect_int_equal(stmts->type, dag_type_stmts, "stmts stmts");

	struct dag_node* num = dag_get_child(stmts, 0);
	assert_ptr(num, "ptr num");
	expect_int_equal(num->type, dag_type_number, "number num");
	expect_str(&num->value, "1", "1 num");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_for_range()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("for i = 0:10 1 end", &ps, &root);

	/* for */
	struct dag_node* node = check_stmts(root, "stmts root");
	assert_ptr(node, "ptr node");
	expect_int_equal(node->type, dag_type_for_range, "for-range node");

	/* i */
	struct dag_node* id = dag_get_child(node, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, dag_type_id, "id id");
	expect_str(&id->value, "i", "i id");

	/* start */
	struct dag_node* start = dag_get_child(node, 1);
	assert_ptr(start, "ptr start");
	expect_int_equal(start->type, dag_type_number, "number start");
	expect_str(&start->value, "0", "0 start");

	/* end */
	struct dag_node* end = dag_get_child(node, 2);
	assert_ptr(end, "ptr end");
	expect_int_equal(end->type, dag_type_number, "number end");
	expect_str(&end->value, "10", "10 end");

	/* stmts */
	struct dag_node* stmt0 = check_stmts(dag_get_child(node, 3), "stmts node 3");
	assert_ptr(stmt0, "ptr stmt0");
	expect_int_equal(stmt0->type, dag_type_number, "number stmt0");
	expect_str(&stmt0->value, "1", "1 stmt0");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_for_iteration()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	parse_setup("for i in list 1 end", &ps, &root);

	/* for */
	struct dag_node* node = check_stmts(root, "stmts root");
	assert_ptr(node, "ptr node");
	expect_int_equal(node->type, dag_type_for_iteration, "for-iteration node");

	/* id */
	struct dag_node* id = dag_get_child(node, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, dag_type_id, "id id");
	expect_str(&id->value, "i", "i id");

	/* expr */
	struct dag_node* expr = dag_get_child(node, 1);
	assert_ptr(expr, "ptr expr");
	expect_int_equal(expr->type, dag_type_id, "id expr");
	expect_str(&expr->value, "list", "id list");

	/* stmts */
	struct dag_node* stmts0 = dag_get_child(node, 2);
	assert_ptr(stmts0, "ptr stmts0");
	expect_int_equal(stmts0->type, dag_type_stmts, "stmts stmts0");

	/* number */
	struct dag_node* num0 = dag_get_child(stmts0, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, dag_type_number, "number num0");
	expect_str(&num0->value, "1", "1 num0");

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_line_col()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;
	enum result r;

	/* allocate ps{} root root{} */
	valid = parse_setup("* 2", &ps, &root);
	assert_false(valid, "parse not valid");
	assert_has_errors(ps.el);
	struct compile_error* e = assert_compile_error(ps.el, "expected term before operator");
	expect_compile_error_fields(e, 1, 1, 0);

	struct buffer bf;
	buffer_init(&bf);

	/* allocate bf */
	r = format_error(e, string_getchar, string_seek, ps.sns->lc->d, &bf);
	assert_ok(r, "format_error");
	expect_str(&bf, "expected term before operator\n* 2", "format_error");

	/* destroy bf */
	buffer_destroy(&bf);

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_source()
{
	test_name(__func__);

	struct dag_node* root;
	struct parse_state ps;
	bool valid;
	enum result r;

	/* allocate ps{} root root{} */
	valid = parse_setup("1\n* 2", &ps, &root);
	assert_false(valid, "parse valid");
	assert_has_errors(ps.el);
	struct compile_error* e = assert_compile_error(ps.el, "expected term before operator");
	expect_compile_error_fields(e, 2, 1, 2);

	struct buffer bf;
	buffer_init(&bf);

	/* allocate bf */
	r = format_error(e, string_getchar, string_seek, ps.sns->lc->d, &bf);
	assert_ok(r, "format_error");
	expect_str(&bf, "expected term before operator\n* 2", "format_error");

	/* destroy bf */
	buffer_destroy(&bf);

	/* destroy ps{} root root{} */
	dag_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_statements()
{
	test_parse_assign();
	test_parse_assign2();
	test_parse_stmts();
	test_parse_stmts2();
	test_parse_stmts3();
	test_parse_stmts4();
	test_parse_stmts5();
	test_parse_function();
	test_parse_function2();
	test_parse_function3();
	test_parse_function4();
	test_parse_call();
	test_parse_call2();
	test_parse_call3();
	test_parse_call4();
	test_parse_if();
	test_parse_elseif();
	test_parse_elseif2();
	test_parse_else();
	test_parse_else2();
	test_parse_while();
	test_parse_for_range();
	test_parse_for_iteration();
	test_parse_line_col();
	test_parse_source();
}
