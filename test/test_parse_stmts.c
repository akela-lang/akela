#include "zinc/unit_test.h"
#include "alba/unit_test_compiler.h"
#include "alba/parse_tools.h"
#include "alba/ast.h"
#include "test_parse.h"
#include "alba/input.h"
#include "alba/type_def.h"
#include "alba/type_use.h"

/* dynamic-output-none */
void test_parse_assign()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var a::Int64; a = 1", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* assign = ast_node_get(root, 1);
	assert_ptr(assign, "ptr assign");
	assert_int_equal(assign->type, ast_type_assign, "assign");

	struct ast_node* left = ast_node_get(assign, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "a", "a");

	struct ast_node* right = ast_node_get(assign, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "1", "1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_assign2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var a::Int64; a = 1 + 2", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* assign = ast_node_get(root, 1);
	assert_ptr(assign, "ptr assign");
	assert_int_equal(assign->type, ast_type_assign, "assign");

	struct ast_node* left = ast_node_get(assign, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_id, "id");
	expect_str(&left->value, "a", "a");

	struct ast_node* right = ast_node_get(assign, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_plus, "plus");

	struct ast_node* left2 = ast_node_get(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_number, "number");
	expect_str(&left2->value, "1", "1");

	struct ast_node* right2 = ast_node_get(right, 1);
	assert_ptr(right2, "right");
	expect_int_equal(right2->type, ast_type_number, "number2");
	expect_str(&right2->value, "2", "2");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_stmts()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var i::Int64; var x::Int64; i + 1; x * 1", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr1");
	expect_int_equal(root->type, ast_type_stmts, "stmts");

	struct ast_node* left = ast_node_get(root, 2);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_plus, "plus");

	struct ast_node* left2 = ast_node_get(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, ast_type_id, "id");
	expect_str(&left2->value, "i", "i");

	struct ast_node* right = ast_node_get(left, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "1", "1");

	struct ast_node* right2 = ast_node_get(root, 3);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, ast_type_mult, "mult");

	struct ast_node* left3 = ast_node_get(right2, 0);
	assert_ptr(left3, "left3");
	expect_int_equal(left3->type, ast_type_id, "id");
	expect_str(&left3->value, "x", "x");

	struct ast_node* right3 = ast_node_get(right2, 1);
	assert_ptr(right3, "right3");
	expect_int_equal(right3->type, ast_type_number, "number2");
	expect_str(&right3->value, "1", "1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_stmts2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var x::Int64; x+1\n5-4\n", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "root");
	expect_int_equal(root->type, ast_type_stmts, "stmts");

	struct ast_node* a = ast_node_get(root, 1);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_plus, "plus a");

	struct ast_node* b = ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	struct ast_node* c = ast_node_get(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, ast_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	struct ast_node* d = ast_node_get(root, 2);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, ast_type_minus, "minus d");

	struct ast_node* e = ast_node_get(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, ast_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	struct ast_node* f = ast_node_get(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, ast_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_stmts3()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var x::Int64; var y::Int64; var z::Int64; x+1; 5-4; y+z", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "root");
	expect_int_equal(root->type, ast_type_stmts, "stmts");

	struct ast_node* a = ast_node_get(root, 3);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_plus, "plus a");

	struct ast_node* b = ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	struct ast_node* c = ast_node_get(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, ast_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	struct ast_node* d = ast_node_get(root, 4);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, ast_type_minus, "minus d");

	struct ast_node* e = ast_node_get(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, ast_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	struct ast_node* f = ast_node_get(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, ast_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

	struct ast_node* g = ast_node_get(root, 5);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, ast_type_plus, "plus g");

	struct ast_node* h = ast_node_get(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, ast_type_id, "id h");
	expect_str(&h->value, "y", "y h");

	struct ast_node* i = ast_node_get(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, ast_type_id, "id i");
	expect_str(&i->value, "z", "z c");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_stmts4()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var x::Int64; var y::Int64; var z::Int64; x+1; 5-4; y+z", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "root");
	expect_int_equal(root->type, ast_type_stmts, "stmts");

	struct ast_node* a = ast_node_get(root, 3);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_plus, "plus a");

	struct ast_node* b = ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id b");
	expect_str(&b->value, "x", "x b");

	struct ast_node* c = ast_node_get(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, ast_type_number, "number c");
	expect_str(&c->value, "1", "1 c");

	struct ast_node* d = ast_node_get(root, 4);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, ast_type_minus, "minus d");

	struct ast_node* e = ast_node_get(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, ast_type_number, "number e");
	expect_str(&e->value, "5", "5 b");

	struct ast_node* f = ast_node_get(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, ast_type_number, "number f");
	expect_str(&f->value, "4", "4 c");

	struct ast_node* g = ast_node_get(root, 5);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, ast_type_plus, "plus g");

	struct ast_node* h = ast_node_get(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, ast_type_id, "id h");
	expect_str(&h->value, "y", "y h");

	struct ast_node* i = ast_node_get(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, ast_type_id, "id i");
	expect_str(&i->value, "z", "z c");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_stmts5()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("if true 1 else 2 end", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	struct ast_node* if_stmt = check_stmts(root, "stmts root");
	assert_ptr(if_stmt, "ptr if_stmt");
	expect_int_equal(if_stmt->type, ast_type_if, "if if_stmt");

	struct ast_node* cb = ast_node_get(if_stmt, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, ast_type_conditional_branch, "conditional branch cb");

	struct ast_node* cond = ast_node_get(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "true", "true cond");

	struct ast_node* stmts_cb = ast_node_get(cb, 1);
	assert_ptr(stmts_cb, "ptr stmts_cb");
	expect_int_equal(stmts_cb->type, ast_type_stmts, "stmts stmts_cb");

	struct ast_node* num0 = ast_node_get(stmts_cb, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, ast_type_number, "number num0");
	expect_str(&num0->value, "1", "1 num0");

	struct ast_node* db = ast_node_get(if_stmt, 1);
	assert_ptr(db, "ptr db");
	expect_int_equal(db->type, ast_type_default_branch, "default branch db");

	struct ast_node* stmts_db = ast_node_get(db, 0);
	assert_ptr(stmts_db, "ptr stmts_db");
	expect_int_equal(stmts_db->type, ast_type_stmts, "stmts stmts_db");

	struct ast_node* num1 = ast_node_get(stmts_db, 0);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, ast_type_number, "number num1");
	expect_str(&num1->value, "2", "2 num1");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_stmts_type()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("1; true; \"hello\"", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct type_use* tu = root->tu;
	assert_ptr(tu, "ptr tu");
	
	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_string, "string td");
	expect_str(&td->name, "String", "String td");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_function()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("function foo() var x::Int64; x+1; 5+4 end", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* f = ast_node_get(root, 0);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, ast_type_function, "function");

	struct type_use* tu = f->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_function, "function td");
	expect_str(&td->name, "Function", "Function td");

	struct ast_node* fid = ast_node_get(f, 0);
	assert_ptr(fid, "ptr a");
	expect_int_equal(fid->type, ast_type_id, "id");

	struct ast_node* dseq = ast_node_get(f, 1);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, ast_type_dseq, "dseq dseq");

	struct ast_node* dret = ast_node_get(f, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* stmts = ast_node_get(f, 3);
	assert_ptr(stmts, "ptr b");
	expect_int_equal(stmts->type, ast_type_stmts, "stmts b");

	struct ast_node* add0 = ast_node_get(stmts, 1);
	assert_ptr(add0, "ptr d");
	expect_int_equal(add0->type, ast_type_plus, "plus");

	struct ast_node* x = ast_node_get(add0, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct ast_node* number1 = ast_node_get(add0, 1);
	assert_ptr(number1, "ptr number1");
	expect_int_equal(number1->type, ast_type_number, "number number1");
	expect_str(&number1->value, "1", "1 number1");

	struct ast_node* add1 = ast_node_get(stmts, 2);
	assert_ptr(add1, "ptr add1");
	expect_int_equal(add1->type, ast_type_plus, "plus add1");

	struct ast_node* number5 = ast_node_get(add1, 0);
	assert_ptr(number5, "ptr number5");
	expect_int_equal(number5->type, ast_type_number, "number number5");
	expect_str(&number5->value, "5", "5 number5");

	struct ast_node* number4 = ast_node_get(add1, 1);
	assert_ptr(number4, "ptr number4");
	expect_int_equal(number4->type, ast_type_number, "number number4");
	expect_str(&number4->value, "4", "4 number4");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_function2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("function foo(x::Int64) x+1; 5+4 end", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* f_node = ast_node_get(root, 0);
	assert_ptr(f_node, "ptr f_node");
	expect_int_equal(f_node->type, ast_type_function, "function f_node");

	struct ast_node* f_id = ast_node_get(f_node, 0);
	assert_ptr(f_id, "ptr f_id");
	expect_int_equal(f_id->type, ast_type_id, "id f_id");

	struct ast_node* dseq = ast_node_get(f_node, 1);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, ast_type_dseq, "dseq");

	struct ast_node* param0 = ast_node_get(dseq, 0);
	assert_ptr(param0, "ptr seq_dec");
	expect_int_equal(param0->type, ast_type_declaration, "declaration param0");

	struct ast_node* x = ast_node_get(param0, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x x");

	struct ast_node* type_x = ast_node_get(param0, 1);
	assert_ptr(type_x, "ptr type_x");
	expect_int_equal(type_x->type, ast_type_type, "type type_x");

	struct type_use* tu_x = type_x->tu;
	assert_ptr(tu_x, "ptr tu_x");

	struct type_def* x_td = tu_x->td;
	expect_str(&x_td->name, "Int64", "Int64 tu_x");

	struct ast_node* dret = ast_node_get(f_node, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* b = ast_node_get(f_node, 3);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_stmts, "stmts");

	struct ast_node* c = ast_node_get(f_node, 4);
	assert_null(c, "ptr c");

	struct ast_node* d = ast_node_get(b, 0);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, ast_type_plus, "plus");

	struct ast_node* e = ast_node_get(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, ast_type_id, "id e");
	expect_str(&e->value, "x", "x");

	struct ast_node* f = ast_node_get(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, ast_type_number, "number f");
	expect_str(&f->value, "1", "1");

	struct ast_node* g = ast_node_get(b, 1);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, ast_type_plus, "plus");

	struct ast_node* h = ast_node_get(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, ast_type_number, "number h");
	expect_str(&h->value, "5", "5");

	struct ast_node* i = ast_node_get(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, ast_type_number, "number i");
	expect_str(&i->value, "4", "4");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_function3()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("function foo(x::Int32, y::Int32) x+1; 5+4 end", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "root");
	expect_int_equal(root->type, ast_type_function, "function");

	struct ast_node* a = ast_node_get(root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_id, "id");

	struct ast_node* seq = ast_node_get(root, 1);
	assert_ptr(seq, "ptr seq");
	expect_int_equal(seq->type, ast_type_dseq, "seq");

	struct ast_node* seq_a = ast_node_get(seq, 0);
	assert_ptr(seq_a, "ptr seq a");
	expect_int_equal(seq_a->type, ast_type_declaration, "declaration seq_a");

	struct ast_node* dec_id0 = ast_node_get(seq_a, 0);
	assert_ptr(dec_id0, "ptr dec_id0");
	expect_int_equal(dec_id0->type, ast_type_id, "id dec_id0");

	struct ast_node* seq_b = ast_node_get(seq, 1);
	assert_ptr(seq_b, "ptr seq b");
	expect_int_equal(seq_b->type, ast_type_declaration, "declaration seq_b");

	struct ast_node* dec_id1 = ast_node_get(seq_b, 0);
	assert_ptr(dec_id1, "ptr dec_id1");
	expect_int_equal(dec_id1->type, ast_type_id, "id dec_id1");

	struct ast_node* dret = ast_node_get(root, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* b = ast_node_get(root, 3);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_stmts, "stmts");

	struct ast_node* c = ast_node_get(root, 4);
	assert_null(c, "ptr c");

	struct ast_node* d = ast_node_get(b, 0);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, ast_type_plus, "plus");

	struct ast_node* e = ast_node_get(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, ast_type_id, "id e");
	expect_str(&e->value, "x", "x");

	struct ast_node* f = ast_node_get(d, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, ast_type_number, "number f");
	expect_str(&f->value, "1", "1");

	struct ast_node* g = ast_node_get(b, 1);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, ast_type_plus, "plus");

	struct ast_node* h = ast_node_get(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, ast_type_number, "number h");
	expect_str(&h->value, "5", "5");

	struct ast_node* i = ast_node_get(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, ast_type_number, "number i");
	expect_str(&i->value, "4", "4");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_function4()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("function foo(x::Int32, y::Int32, z::Int32)::Int32 x+1; 5+4 end", &ps, &root);
	assert_no_errors(ps.el);
	expect_true(valid, "parse valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* f = ast_node_get(root, 0);
	assert_ptr(f, "root");
	expect_int_equal(f->type, ast_type_function, "function f");

	struct type_use* tu = f->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_function, "function td");
	expect_str(&td->name, "Function", "Function td");

	struct type_use* input_tu = type_use_get(tu, 0);
	assert_ptr(input_tu, "ptr input_tu");

	struct type_def* input_td = input_tu->td;
	assert_ptr(input_td, "ptr input_td");
	expect_int_equal(input_td->type, type_function_input, "function_input input_td");
	expect_str(&input_td->name, "Input", "Input input_td");

	struct type_use* input0_tu = type_use_get(input_tu, 0);
	assert_ptr(input0_tu, "ptr input0_tu");

	struct type_def* input0_td = input0_tu->td;
	assert_ptr(input0_td, "ptr input0_td");
	expect_int_equal(input0_td->type, type_integer, "integer input0_td");
	expect_str(&input0_td->name, "Int32", "Int32 input0_td");

	struct type_use* input1_tu = type_use_get(input_tu, 1);
	assert_ptr(input1_tu, "ptr input1_tu");

	struct type_def* input1_td = input1_tu->td;
	assert_ptr(input1_td, "ptr input1_td");
	expect_int_equal(input1_td->type, type_integer, "integer input1_td");
	expect_str(&input1_td->name, "Int32", "Int32 input1_td");

	struct type_use* input2_tu = type_use_get(input_tu, 2);
	assert_ptr(input2_tu, "ptr input2_tu");

	struct type_def* input2_td = input2_tu->td;
	assert_ptr(input2_td, "ptr input2_td");
	expect_int_equal(input2_td->type, type_integer, "integer input2_td");
	expect_str(&input2_td->name, "Int32", "Int32 input2_td");

	struct type_use* output_tu = type_use_get(tu, 1);
	assert_ptr(output_tu, "ptr output_tu");

	struct type_def* output_td = output_tu->td;
	assert_ptr(output_td, "ptr output_td");
	expect_int_equal(output_td->type, type_function_output, "function_input output_td");
	expect_str(&output_td->name, "Output", "Output output_td");

	struct type_use* output0_tu = type_use_get(output_tu, 0);
	assert_ptr(output0_tu, "ptr output0_tu");

	struct type_def* output0_td = output0_tu->td;
	assert_ptr(output0_td, "ptr output0_td");
	expect_int_equal(output0_td->type, type_integer, "integer output0_td");
	expect_str(&output0_td->name, "Int32", "Int32 output0_td");

	struct ast_node* a = ast_node_get(f, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_id, "id");

	struct ast_node* seq = ast_node_get(f, 1);
	assert_ptr(seq, "ptr seq");
	expect_int_equal(seq->type, ast_type_dseq, "seq");

	struct ast_node* seq_a = ast_node_get(seq, 0);
	assert_ptr(seq_a, "ptr seq a");
	expect_int_equal(seq_a->type, ast_type_declaration, "declaration seq_a");

	struct ast_node* dec_a = ast_node_get(seq_a, 0);
	assert_ptr(dec_a, "ptr dec_a");
	expect_int_equal(dec_a->type, ast_type_id, "id dec_a");

	struct ast_node* seq_b = ast_node_get(seq, 1);
	assert_ptr(seq_b, "ptr seq b");
	expect_int_equal(seq_b->type, ast_type_declaration, "declaration seq_b");

	struct ast_node* dec_b = ast_node_get(seq_b, 0);
	assert_ptr(dec_b, "ptr dec_b");
	expect_int_equal(dec_b->type, ast_type_id, "id dec_b");

	struct ast_node* seq_c = ast_node_get(seq, 2);
	assert_ptr(seq_c, "ptr seq c");
	expect_int_equal(seq_c->type, ast_type_declaration, "declaration seq_c");

	struct ast_node* dec_c = ast_node_get(seq_c, 0);
	assert_ptr(dec_c, "ptr dec_c");
	expect_int_equal(dec_c->type, ast_type_id, "id dec_c");

	struct ast_node* dret = ast_node_get(f, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* dret_type = ast_node_get(dret, 0);
	assert_ptr(dret_type, "ptr dret_type");
	expect_int_equal(dret_type->type, ast_type_type, "type dret_type");

	struct type_use* dret_tu = dret_type->tu;
	assert_ptr(dret_tu, "ptr dret_tu");

	struct type_def* dret_td = dret_tu->td;
	assert_ptr(dret_td, "ptr dret_td");
	expect_int_equal(dret_td->type, type_integer, "integer dret_td");
	expect_str(&dret_td->name, "Int32", "Int32 dret_td");

	struct ast_node* b = ast_node_get(f, 3);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_stmts, "stmts");

	struct ast_node* d = ast_node_get(b, 0);
	assert_ptr(d, "ptr d");
	expect_int_equal(d->type, ast_type_plus, "plus");

	struct ast_node* e = ast_node_get(d, 0);
	assert_ptr(e, "ptr e");
	expect_int_equal(e->type, ast_type_id, "id e");
	expect_str(&e->value, "x", "x");

	struct ast_node* number1 = ast_node_get(d, 1);
	assert_ptr(number1, "ptr number1");
	expect_int_equal(number1->type, ast_type_number, "number number1");
	expect_str(&number1->value, "1", "1 number1");

	struct ast_node* g = ast_node_get(b, 1);
	assert_ptr(g, "ptr g");
	expect_int_equal(g->type, ast_type_plus, "plus");

	struct ast_node* h = ast_node_get(g, 0);
	assert_ptr(h, "ptr h");
	expect_int_equal(h->type, ast_type_number, "number h");
	expect_str(&h->value, "5", "5");

	struct ast_node* i = ast_node_get(g, 1);
	assert_ptr(i, "ptr i");
	expect_int_equal(i->type, ast_type_number, "number i");
	expect_str(&i->value, "4", "4");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_function5()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("function foo(x::Int64) var x::Int64 end", &ps, &root);
	expect_has_errors(ps.el);
	expect_false(valid, "parse valid");
	expect_compile_error(ps.el, "duplicate declaration in same scope: x");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_function_return_type_error()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("function foo(x::Int64)::Int64 true end", &ps, &root);
	expect_has_errors(ps.el);
	expect_false(valid, "parse valid");
	expect_compile_error(ps.el, "returned type does not match function return type");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_function_not_global()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("if true function foo() 1 end end", &ps, &root);
	assert_has_errors(ps.el);
	assert_false(valid, "parse valid");
	assert_compile_error(ps.el, "function declaration is not in global scope");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_if()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("if true var x::Int64; var y::Int64; 10 + 20; x * y end", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	root = check_stmts(root, "stmts root");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_if, "if");

	struct ast_node* cb = ast_node_get(root, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, ast_type_conditional_branch, "conditional branch");

	struct ast_node* cond = ast_node_get(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, ast_type_boolean, "boolean");
	expect_str(&cond->value, "true", "true");

	struct ast_node* stmts = ast_node_get(cb, 1);
	assert_ptr(stmts, "ptr stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "stmts");

	struct ast_node* plus = ast_node_get(stmts, 2);
	assert_ptr(plus, "ptr plus");
	expect_int_equal(plus->type, ast_type_plus, "plus");

	struct ast_node* num0 = ast_node_get(plus, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, ast_type_number, "number 0");
	expect_str(&num0->value, "10", "10");

	struct ast_node* num1 = ast_node_get(plus, 1);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, ast_type_number, "number 1");
	expect_str(&num1->value, "20", "20");

	struct ast_node* mult = ast_node_get(stmts, 3);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, ast_type_mult, "mult");

	struct ast_node* x = ast_node_get(mult, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct ast_node* y = ast_node_get(mult, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_elseif()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var x::Int64; var y::Int64; if true 10 + 20; x * y elseif true 1; 2 end", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");

	struct ast_node* if_stmt = ast_node_get(root, 2);
	assert_ptr(if_stmt, "ptr if_stmt");
	expect_int_equal(if_stmt->type, ast_type_if, "if");

	struct ast_node* cb = ast_node_get(if_stmt, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, ast_type_conditional_branch, "conditional branch");

	struct ast_node* cond = ast_node_get(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "true", "true");

	struct ast_node* stmts = ast_node_get(cb, 1);
	assert_ptr(stmts, "ptr stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "stmts");

	struct ast_node* plus = ast_node_get(stmts, 0);
	assert_ptr(plus, "ptr plus");
	expect_int_equal(plus->type, ast_type_plus, "plus");

	struct ast_node* num0 = ast_node_get(plus, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, ast_type_number, "number 0");
	expect_str(&num0->value, "10", "10");

	struct ast_node* num1 = ast_node_get(plus, 1);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, ast_type_number, "number 1");
	expect_str(&num1->value, "20", "20");

	struct ast_node* mult = ast_node_get(stmts, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, ast_type_mult, "mult");

	struct ast_node* x = ast_node_get(mult, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct ast_node* y = ast_node_get(mult, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

	struct ast_node* cb2 = ast_node_get(if_stmt, 1);
	assert_ptr(cb2, "ptr cb2");
	expect_int_equal(cb2->type, ast_type_conditional_branch, "conditional branch cb2");

	struct ast_node* cond2 = ast_node_get(cb2, 0);
	assert_ptr(cond2, "ptr cond2");
	expect_int_equal(cond2->type, ast_type_boolean, "boolean cond2");
	expect_str(&cond2->value, "true", "true cond2");

	struct ast_node* stmts2 = ast_node_get(cb2, 1);
	assert_ptr(stmts2, "ptr stmts2");
	expect_int_equal(stmts2->type, ast_type_stmts, "stmts stmts2");

	struct ast_node* num2 = ast_node_get(stmts2, 0);
	assert_ptr(num2, "ptr num2");
	expect_int_equal(num2->type, ast_type_number, "number num2");
	expect_str(&num2->value, "1", "1 num2");

	struct ast_node* num3 = ast_node_get(stmts2, 1);
	assert_ptr(num3, "ptr num3");
	expect_int_equal(num3->type, ast_type_number, "number num3");
	expect_str(&num3->value, "2", "2 num3");


	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_elseif2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var x::Int64; var y::Int64; if true 10 + 20; x * y elseif true 1; 2 elseif true x; y end", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* if_stmt = ast_node_get(root, 2);
	assert_ptr(if_stmt, "ptr root");
	expect_int_equal(if_stmt->type, ast_type_if, "if if_stmt");

	struct ast_node* cb = ast_node_get(if_stmt, 0);
	assert_ptr(cb, "ptr cb");
	expect_int_equal(cb->type, ast_type_conditional_branch, "conditional branch");

	struct ast_node* cond = ast_node_get(cb, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "true", "true");

	struct ast_node* stmts = ast_node_get(cb, 1);
	assert_ptr(stmts, "ptr stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "stmts");

	struct ast_node* plus = ast_node_get(stmts, 0);
	assert_ptr(plus, "ptr plus");
	expect_int_equal(plus->type, ast_type_plus, "plus");

	struct ast_node* num0 = ast_node_get(plus, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, ast_type_number, "number 0");
	expect_str(&num0->value, "10", "10");

	struct ast_node* num1 = ast_node_get(plus, 1);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, ast_type_number, "number 1");
	expect_str(&num1->value, "20", "20");

	struct ast_node* mult = ast_node_get(stmts, 1);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, ast_type_mult, "mult");

	struct ast_node* x = ast_node_get(mult, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct ast_node* y = ast_node_get(mult, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

	/* first elseif */
	struct ast_node* cb2 = ast_node_get(if_stmt, 1);
	assert_ptr(cb2, "ptr cb2");
	expect_int_equal(cb2->type, ast_type_conditional_branch, "conditional branch cb2");

	struct ast_node* cond2 = ast_node_get(cb2, 0);
	assert_ptr(cond2, "ptr cond2");
	expect_int_equal(cond2->type, ast_type_boolean, "boolean cond2");
	expect_str(&cond2->value, "true", "true cond2");

	struct ast_node* stmts2 = ast_node_get(cb2, 1);
	assert_ptr(stmts2, "ptr stmts2");
	expect_int_equal(stmts2->type, ast_type_stmts, "stmts stmts2");

	struct ast_node* num2 = ast_node_get(stmts2, 0);
	assert_ptr(num2, "ptr num2");
	expect_int_equal(num2->type, ast_type_number, "number num2");
	expect_str(&num2->value, "1", "1 num2");

	struct ast_node* num3 = ast_node_get(stmts2, 1);
	assert_ptr(num3, "ptr num3");
	expect_int_equal(num3->type, ast_type_number, "number num3");
	expect_str(&num3->value, "2", "2 num3");

	/* second elseif */
	struct ast_node* cb3 = ast_node_get(if_stmt, 2);
	assert_ptr(cb3, "ptr cb3");
	expect_int_equal(cb3->type, ast_type_conditional_branch, "conditional branch cb3");

	struct ast_node* cond3 = ast_node_get(cb3, 0);
	assert_ptr(cond3, "ptr cond3");
	expect_int_equal(cond3->type, ast_type_boolean, "boolean cond3");
	expect_str(&cond3->value, "true", "true cond3");

	struct ast_node* stmts3 = ast_node_get(cb3, 1);
	assert_ptr(stmts3, "ptr stmts3");
	expect_int_equal(stmts3->type, ast_type_stmts, "stmts stmts3");

	struct ast_node* x2 = ast_node_get(stmts3, 0);
	assert_ptr(x2, "ptr x2");
	expect_int_equal(x2->type, ast_type_id, "id x2");
	expect_str(&x2->value, "x", "x x2");

	struct ast_node* y2 = ast_node_get(stmts3, 1);
	assert_ptr(y2, "ptr y2");
	expect_int_equal(y2->type, ast_type_id, "id y2");
	expect_str(&y2->value, "y", "y y2");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_else()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var x::Int64; var y::Int64; if false 10 else x; y end", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* if_stmt = ast_node_get(root, 2);
	assert_ptr(if_stmt, "ptr if_stmt");
	assert_int_equal(if_stmt->type, ast_type_if, "if if_stmt");

	struct ast_node* cb0 = ast_node_get(if_stmt, 0);
	assert_ptr(cb0, "ptr cb0");
	expect_int_equal(cb0->type, ast_type_conditional_branch, "conditional branch cb0");

	struct ast_node* cond = ast_node_get(cb0, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "false", "false cond");

	struct ast_node* stmts0 = ast_node_get(cb0, 1);
	assert_ptr(stmts0, "ptr stmts 0");
	expect_int_equal(stmts0->type, ast_type_stmts, "stmts stmts0");

	struct ast_node* num = ast_node_get(stmts0, 0);
	assert_ptr(num, "ptr num");
	expect_int_equal(num->type, ast_type_number, "number num");
	expect_str(&num->value, "10", "10 num");

	struct ast_node* cb1 = ast_node_get(if_stmt, 1);
	assert_ptr(cb1, "ptr cb1");
	expect_int_equal(cb1->type, ast_type_default_branch, "default branch cb1");

	struct ast_node* stmts1 = ast_node_get(cb1, 0);
	assert_ptr(stmts1, "ptr stmts1");
	expect_int_equal(stmts1->type, ast_type_stmts, "stmts stmts1");

	struct ast_node* x = ast_node_get(stmts1, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct ast_node* y = ast_node_get(stmts1, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_else2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var x::Int64; var y::Int64; if false 10 elseif false 20 else x; y end", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* if_stmt = ast_node_get(root, 2);
	assert_ptr(if_stmt, "ptr root");
	assert_int_equal(if_stmt->type, ast_type_if, "if");

	/* if */
	struct ast_node* cb0 = ast_node_get(if_stmt, 0);
	assert_ptr(cb0, "ptr cb0");
	expect_int_equal(cb0->type, ast_type_conditional_branch, "conditional branch cb0");

	struct ast_node* cond0 = ast_node_get(cb0, 0);
	assert_ptr(cond0, "ptr cond0");
	expect_int_equal(cond0->type, ast_type_boolean, "boolean cond0");
	expect_str(&cond0->value, "false", "false cond0");

	struct ast_node* stmts0 = ast_node_get(cb0, 1);
	assert_ptr(stmts0, "ptr stmts 0");
	expect_int_equal(stmts0->type, ast_type_stmts, "stmts stmts0");

	struct ast_node* num0 = ast_node_get(stmts0, 0);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, ast_type_number, "number num0");
	expect_str(&num0->value, "10", "10 num0");

	/* elseif */
	struct ast_node* cb1 = ast_node_get(if_stmt, 1);
	assert_ptr(cb1, "ptr cb1");
	expect_int_equal(cb1->type, ast_type_conditional_branch, "conditional branch cb1");

	struct ast_node* cond1 = ast_node_get(cb1, 0);
	assert_ptr(cond1, "ptr cond");
	expect_int_equal(cond1->type, ast_type_boolean, "boolean cond1");
	expect_str(&cond1->value, "false", "false cond1");

	struct ast_node* stmts1 = ast_node_get(cb1, 1);
	assert_ptr(stmts1, "ptr stmts1");
	expect_int_equal(stmts1->type, ast_type_stmts, "stmts stmts1");

	struct ast_node* num1 = ast_node_get(stmts1, 0);
	expect_int_equal(num1->type, ast_type_number, "number num1");
	expect_str(&num1->value, "20", "20 num1");

	/* else */
	struct ast_node* db = ast_node_get(if_stmt, 2);
	assert_ptr(db, "ptr db");
	expect_int_equal(db->type, ast_type_default_branch, "default branch db");

	struct ast_node* stmts2 = ast_node_get(db, 0);
	assert_ptr(stmts2, "ptr stmts2");
	expect_int_equal(stmts2->type, ast_type_stmts, "stmts stmts2");

	struct ast_node* x = ast_node_get(stmts2, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x");

	struct ast_node* y = ast_node_get(stmts2, 1);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_while()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("while true 1 end", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	struct ast_node* node = check_stmts(root, "stmts root");
	assert_ptr(node, "ptr node");
	expect_int_equal(node->type, ast_type_while, "while node");

	struct ast_node* cond = ast_node_get(node, 0);
	assert_ptr(cond, "ptr cond");
	expect_int_equal(cond->type, ast_type_boolean, "boolean cond");
	expect_str(&cond->value, "true", "true cond");

	struct ast_node* stmts = ast_node_get(node, 1);
	assert_ptr(stmts, "ptr stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "stmts stmts");

	struct ast_node* num = ast_node_get(stmts, 0);
	assert_ptr(num, "ptr num");
	expect_int_equal(num->type, ast_type_number, "number num");
	expect_str(&num->value, "1", "1 num");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_for_range()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("for i::Int64 = 0:10 1 end", &ps, &root);
	assert_no_errors(ps.el);
	assert_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	/* for */
	struct ast_node* node = ast_node_get(root, 0);
	assert_ptr(node, "ptr node");
	expect_int_equal(node->type, ast_type_for_range, "for-range node");

	/* i */
	struct ast_node* dec = ast_node_get(node, 0);
	assert_ptr(dec, "ptr dec");
	expect_int_equal(dec->type, ast_type_declaration, "declaration dec");

	struct ast_node* id = ast_node_get(dec, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "i", "i id");

	/* start */
	struct ast_node* start = ast_node_get(node, 1);
	assert_ptr(start, "ptr start");
	expect_int_equal(start->type, ast_type_number, "number start");
	expect_str(&start->value, "0", "0 start");

	/* end */
	struct ast_node* end = ast_node_get(node, 2);
	assert_ptr(end, "ptr end");
	expect_int_equal(end->type, ast_type_number, "number end");
	expect_str(&end->value, "10", "10 end");

	/* stmts */
	struct ast_node* stmt0 = check_stmts(ast_node_get(node, 3), "stmts node 3");
	assert_ptr(stmt0, "ptr stmt0");
	expect_int_equal(stmt0->type, ast_type_number, "number stmt0");
	expect_str(&stmt0->value, "1", "1 stmt0");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_for_range2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("for i::Int64 = 0:10 var i::Int64 = 1 end", &ps, &root);
	expect_has_errors(ps.el);
	expect_false(valid, "parse_setup valid");
	expect_compile_error(ps.el, "duplicate declaration in same scope: i");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_for_iteration()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var list::Vector{Int64}; for i::Int64 in list i end", &ps, &root);
	expect_no_errors(ps.el);
	expect_true(valid, "parse_setup valid");

	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	/* for */
	struct ast_node* node = ast_node_get(root, 1);
	assert_ptr(node, "ptr node");
	expect_int_equal(node->type, ast_type_for_iteration, "for-iteration node");

	/* declaration */
	struct ast_node* dec = ast_node_get(node, 0);
	assert_ptr(dec, "ptr dec");
	expect_int_equal(dec->type, ast_type_declaration, "declaration dec");

	/* id */
	struct ast_node* id = ast_node_get(dec, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "i", "i id");

	struct ast_node* i_type = ast_node_get(dec, 1);
	assert_ptr(i_type, "ptr i_type");
	expect_int_equal(i_type->type, ast_type_type, "type i_type");

	struct type_use* i_tu = i_type->tu;
	assert_ptr(i_tu, "ptr i_tu");

	struct type_def* i_td = i_tu->td;
	assert_ptr(i_td, "ptr i_td");
	expect_int_equal(i_td->type, type_integer, "integer i_td");
	expect_str(&i_td->name, "Int64", "Int64 i_td");

	/* expr */
	struct ast_node* expr = ast_node_get(node, 1);
	assert_ptr(expr, "ptr expr");
	expect_int_equal(expr->type, ast_type_id, "id expr");
	expect_str(&expr->value, "list", "id list");

	/* stmts */
	struct ast_node* stmts0 = ast_node_get(node, 2);
	assert_ptr(stmts0, "ptr stmts0");
	expect_int_equal(stmts0->type, ast_type_stmts, "stmts stmts0");

	/* i */
	struct ast_node* id2 = ast_node_get(stmts0, 0);
	assert_ptr(id2, "ptr id2");
	expect_int_equal(id2->type, ast_type_id, "id id2");
	expect_str(&id2->value, "i", "i id2");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

void test_parse_for_iteration2()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;

	/* allocate ps{} root root{} */
	bool valid = parse_setup("var list::Vector{Int64}; for i::Int64 in list var i::Int64 = 1 end", &ps, &root);
	expect_has_errors(ps.el);
	expect_false(valid, "parse_setup valid");
	expect_compile_error(ps.el, "duplicate declaration in same scope: i");

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_line_col()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("2 *", &ps, &root);
	assert_false(valid, "parse not valid");
	assert_has_errors(ps.el);
	struct compile_error* e = assert_compile_error(ps.el, "expected term after operator");
	expect_compile_error_fields(e, 1, 4, 3);

	struct buffer bf;
	buffer_init(&bf);

	/* allocate bf */
	enum result r = format_error(e, (input_getchar)string_getchar, (input_seek)string_seek, ps.sns->lc->d, &bf);
	assert_ok(r, "format_error");
	expect_str(&bf, "expected term after operator\n*", "format_error");

	/* destroy bf */
	buffer_destroy(&bf);

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
	parse_teardown(&ps);
}

/* dynamic-output-none */
void test_parse_source()
{
	test_name(__func__);

	struct ast_node* root;
	struct parse_state ps;
	bool valid;

	/* allocate ps{} root root{} */
	valid = parse_setup("1\n2 *", &ps, &root);
	assert_false(valid, "parse valid");
	assert_has_errors(ps.el);
	struct compile_error* e = assert_compile_error(ps.el, "expected term after operator");
	expect_compile_error_fields(e, 2, 4, 5);

	struct buffer bf;
	buffer_init(&bf);

	/* allocate bf */
	enum result r = format_error(e, (input_getchar)string_getchar, (input_seek)string_seek, ps.sns->lc->d, &bf);
	assert_ok(r, "format_error");
	expect_str(&bf, "expected term after operator\n*", "format_error");

	/* destroy bf */
	buffer_destroy(&bf);

	/* destroy ps{} root root{} */
	ast_node_destroy(root);
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
	test_parse_stmts_type();
	test_parse_function();
	test_parse_function2();
	test_parse_function3();
	test_parse_function4();
	test_parse_function5();
	test_parse_function_return_type_error();
	test_parse_function_not_global();
	test_parse_if();
	test_parse_elseif();
	test_parse_elseif2();
	test_parse_else();
	test_parse_else2();
	test_parse_while();
	test_parse_for_range();
	test_parse_for_range2();
	test_parse_for_iteration();
	test_parse_for_iteration2();
	test_parse_line_col();
	test_parse_source();
}
