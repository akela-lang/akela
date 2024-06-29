#include "zinc/unit_test.h"
#include "akela/parse_tools.h"
#include "test_parse.h"
#include "zinc/error_unit_test.h"

void test_parse_function_no_inputs_no_outputs()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo() let x: i64; x+1; 5+4 end", &cu);
    assert_no_errors(&cu.el);
    assert_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* f = Ast_node_get(cu.root, 0);
    assert_ptr(f, "ptr f");
    expect_int_equal(f->type, Ast_type_function, "function");

    Type_use* tu = f->tu;
    assert_ptr(tu, "ptr tu");

    struct type_def* td = tu->td;
    assert_ptr(td, "ptr td");
    expect_int_equal(td->type, type_function, "function td");
    expect_str(&td->name, "Function", "Function td");

    Ast_node* proto = Ast_node_get(f, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, Ast_type_prototype, "type proto");

    Ast_node* fid = Ast_node_get(proto, 0);
    assert_ptr(fid, "ptr a");
    expect_int_equal(fid->type, Ast_type_id, "id");

    Ast_node* dseq = Ast_node_get(proto, 1);
    assert_ptr(dseq, "ptr dseq");
    expect_int_equal(dseq->type, Ast_type_dseq, "dseq dseq");

    Ast_node* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, Ast_type_dret, "dret dret");

    Ast_node* stmts = Ast_node_get(f, 1);
    assert_ptr(stmts, "ptr b");
    expect_int_equal(stmts->type, Ast_type_stmts, "parse_stmts b");

    Ast_node* add0 = Ast_node_get(stmts, 1);
    assert_ptr(add0, "ptr d");
    expect_int_equal(add0->type, Ast_type_plus, "plus");

    Ast_node* x = Ast_node_get(add0, 0);
    assert_ptr(x, "ptr x");
    expect_int_equal(x->type, Ast_type_id, "id x");
    expect_str(&x->value, "x", "x");

    Ast_node* number1 = Ast_node_get(add0, 1);
    assert_ptr(number1, "ptr number1");
    expect_int_equal(number1->type, Ast_type_number, "number number1");
    expect_str(&number1->value, "1", "1 number1");

    Ast_node* add1 = Ast_node_get(stmts, 2);
    assert_ptr(add1, "ptr add1");
    expect_int_equal(add1->type, Ast_type_plus, "plus add1");

    Ast_node* number5 = Ast_node_get(add1, 0);
    assert_ptr(number5, "ptr number5");
    expect_int_equal(number5->type, Ast_type_number, "number number5");
    expect_str(&number5->value, "5", "5 number5");

    Ast_node* number4 = Ast_node_get(add1, 1);
    assert_ptr(number4, "ptr number4");
    expect_int_equal(number4->type, Ast_type_number, "number number4");
    expect_str(&number4->value, "4", "4 number4");

    parse_teardown(&cu);
}

void test_parse_function_input()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(x: i64) x+1; 5+4 end", &cu);
    assert_no_errors(&cu.el);
    assert_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* f_node = Ast_node_get(cu.root, 0);
    assert_ptr(f_node, "ptr f_node");
    expect_int_equal(f_node->type, Ast_type_function, "function f_node");

    Ast_node* proto = Ast_node_get(f_node, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, Ast_type_prototype, "type proto");

    Ast_node* f_id = Ast_node_get(proto, 0);
    assert_ptr(f_id, "ptr f_id");
    expect_int_equal(f_id->type, Ast_type_id, "id f_id");

    Ast_node* dseq = Ast_node_get(proto, 1);
    assert_ptr(dseq, "ptr dseq");
    expect_int_equal(dseq->type, Ast_type_dseq, "dseq");

    Ast_node* param0 = Ast_node_get(dseq, 0);
    assert_ptr(param0, "ptr seq_dec");
    expect_int_equal(param0->type, Ast_type_declaration, "declaration param0");

    Ast_node* x = Ast_node_get(param0, 0);
    assert_ptr(x, "ptr x");
    expect_int_equal(x->type, Ast_type_id, "id x");
    expect_str(&x->value, "x", "x x");

    Ast_node* x_type_node = Ast_node_get(param0, 1);
    assert_ptr(x_type_node, "ptr tu_x");

    Type_use* x_tu = x_type_node->tu;
    assert_ptr(x_tu, "ptr x_tu");

    struct type_def* x_td = x_tu->td;
    assert_ptr(x_td, "ptr x_td");
    expect_str(&x_td->name, "i64", "i64 tu_x");

    Ast_node* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, Ast_type_dret, "dret dret");

    Ast_node* f_stmts = Ast_node_get(f_node, 1);
    assert_ptr(f_stmts, "ptr b");
    expect_int_equal(f_stmts->type, Ast_type_stmts, "type f_stmts");

    Ast_node* d = Ast_node_get(f_stmts, 0);
    assert_ptr(d, "ptr d");
    expect_int_equal(d->type, Ast_type_plus, "plus");

    Ast_node* e = Ast_node_get(d, 0);
    assert_ptr(e, "ptr e");
    expect_int_equal(e->type, Ast_type_id, "id e");
    expect_str(&e->value, "x", "x");

    Ast_node* f = Ast_node_get(d, 1);
    assert_ptr(f, "ptr f");
    expect_int_equal(f->type, Ast_type_number, "number f");
    expect_str(&f->value, "1", "1");

    Ast_node* g = Ast_node_get(f_stmts, 1);
    assert_ptr(g, "ptr g");
    expect_int_equal(g->type, Ast_type_plus, "plus");

    Ast_node* h = Ast_node_get(g, 0);
    assert_ptr(h, "ptr h");
    expect_int_equal(h->type, Ast_type_number, "number h");
    expect_str(&h->value, "5", "5");

    Ast_node* i = Ast_node_get(g, 1);
    assert_ptr(i, "ptr i");
    expect_int_equal(i->type, Ast_type_number, "number i");
    expect_str(&i->value, "4", "4");

    parse_teardown(&cu);
}

void test_parse_function_multiple_inputs()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(x: i32, y: i32) x+1; 5+4 end", &cu);
    assert_no_errors(&cu.el);
    assert_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* f = Ast_node_get(cu.root, 0);
    assert_ptr(f, "cu.root");
    expect_int_equal(f->type, Ast_type_function, "function");

    Ast_node* proto = Ast_node_get(f, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, Ast_type_prototype, "type proto");

    Ast_node* a = Ast_node_get(proto, 0);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_id, "id");

    Ast_node* seq = Ast_node_get(proto, 1);
    assert_ptr(seq, "ptr seq");
    expect_int_equal(seq->type, Ast_type_dseq, "seq");

    Ast_node* seq_a = Ast_node_get(seq, 0);
    assert_ptr(seq_a, "ptr seq a");
    expect_int_equal(seq_a->type, Ast_type_declaration, "declaration seq_a");

    Ast_node* dec_id0 = Ast_node_get(seq_a, 0);
    assert_ptr(dec_id0, "ptr dec_id0");
    expect_int_equal(dec_id0->type, Ast_type_id, "id dec_id0");

    Ast_node* seq_b = Ast_node_get(seq, 1);
    assert_ptr(seq_b, "ptr seq b");
    expect_int_equal(seq_b->type, Ast_type_declaration, "declaration seq_b");

    Ast_node* dec_id1 = Ast_node_get(seq_b, 0);
    assert_ptr(dec_id1, "ptr dec_id1");
    expect_int_equal(dec_id1->type, Ast_type_id, "id dec_id1");

    Ast_node* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, Ast_type_dret, "dret dret");

    Ast_node* b = Ast_node_get(f, 1);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_stmts, "parse_stmts");

    Ast_node* d = Ast_node_get(b, 0);
    assert_ptr(d, "ptr d");
    expect_int_equal(d->type, Ast_type_plus, "plus");

    Ast_node* e = Ast_node_get(d, 0);
    assert_ptr(e, "ptr e");
    expect_int_equal(e->type, Ast_type_id, "id e");
    expect_str(&e->value, "x", "x");

    Ast_node* number1 = Ast_node_get(d, 1);
    assert_ptr(number1, "ptr f");
    expect_int_equal(number1->type, Ast_type_number, "number number1");
    expect_str(&number1->value, "1", "1 number1");

    Ast_node* g = Ast_node_get(b, 1);
    assert_ptr(g, "ptr g");
    expect_int_equal(g->type, Ast_type_plus, "plus");

    Ast_node* h = Ast_node_get(g, 0);
    assert_ptr(h, "ptr h");
    expect_int_equal(h->type, Ast_type_number, "number h");
    expect_str(&h->value, "5", "5");

    Ast_node* i = Ast_node_get(g, 1);
    assert_ptr(i, "ptr i");
    expect_int_equal(i->type, Ast_type_number, "number i");
    expect_str(&i->value, "4", "4");

    parse_teardown(&cu);
}

void test_parse_function_three_inputs()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(x: i32, y: i32, z: i32)->i32 x+1; 5+4 end", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "parse valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* f = Ast_node_get(cu.root, 0);
    assert_ptr(f, "cu.root");
    expect_int_equal(f->type, Ast_type_function, "function f");

    Type_use* tu = f->tu;
    assert_ptr(tu, "ptr tu");
    expect_str(&tu->name, "foo", "name tu");

    struct type_def* td = tu->td;
    assert_ptr(td, "ptr td");
    expect_int_equal(td->type, type_function, "function td");
    expect_str(&td->name, "Function", "Function td");

    Type_use* inputs = tu->head;
    assert_ptr(inputs, "ptr inputs");
    expect_int_equal(inputs->type, Type_use_function_inputs, "type inputs");

    Type_use* outputs = inputs->next;
    assert_ptr(outputs, "ptr outputs");
    expect_int_equal(outputs->type, Type_use_function_outputs, "type outputs");

    parse_teardown(&cu);
}

void test_parse_function_error_duplicate_variable_declarations()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(x: i64) let x: i64 end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "duplicate declaration in same scope: x");

    parse_teardown(&cu);
}

void test_parse_function_return_type_error()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(x: i64)->i64 true end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "returned type does not match function return type");

    parse_teardown(&cu);
}

void test_parse_function_error_expected_left_parenthesis()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "expected left parenthesis");

    parse_teardown(&cu);
}

void test_parse_function_error_expected_right_parenthesis()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "expected right parenthesis");

    parse_teardown(&cu);
}

void test_parse_function_error_duplicate_declaration()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo() end; fn foo() end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "duplicate declaration in same scope: foo");

    parse_teardown(&cu);
}

void test_parse_function_error_identifier_reserved()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn i64() end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "identifier reserved as a type: i64");

    parse_teardown(&cu);
}

void test_parse_function_error_expected_end()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo()", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "expected end");

    parse_teardown(&cu);
}

void test_parse_return_error_outside_of_function()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("return true", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "return statement outside of function");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_error, "stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_return_error_type_does_not_match()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo()->i64 return true end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "returned type does not match function return type");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_error, "stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_stmts_newline_function()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn\nfoo\n(\na: i64,\nb: i64\n)->\ni64 1 end", &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_return()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo()->i64 return 1 end", &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_return_error_no_value()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn bar() end; fn foo()->i64 return bar() end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "return expression has no value");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_error, "stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_anonymous_function()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let mut a: fn(i32, i32, i32)\n"
        "a = fn (x: i32,y: i32,z: i32)\n"
        "    1\n"
        "  end\n",
        &cu
    );
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "parse valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* assign = Ast_node_get(cu.root, 1);
    assert_ptr(assign, "ptr assign");
    expect_int_equal(assign->type, Ast_type_assign, "assign assign");

    Ast_node* a = Ast_node_get(assign, 0);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_id, "id a");
    expect_str(&a->value, "a", "a a");

    Ast_node* f = Ast_node_get(assign, 1);
    assert_ptr(f, "ptr f");
    expect_int_equal(f->type, Ast_type_function, "type f");

    Ast_node* proto = Ast_node_get(f, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, Ast_type_prototype, "type proto");

    Ast_node* func_id = Ast_node_get(proto, 0);
    assert_ptr(func_id, "ptr func_id");
    expect_int_equal(func_id->type, Ast_type_id, "type func_id");
    expect_str(&func_id->value, "__anonymous_function_0", "value func_id");

    Ast_node* dseq = Ast_node_get(proto, 1);
    assert_ptr(dseq, "ptr dseq");
    expect_int_equal(dseq->type, Ast_type_dseq, "dseq dseq");

    Ast_node* dec_x = Ast_node_get(dseq, 0);
    assert_ptr(dec_x, "ptr dec_x");
    expect_int_equal(dec_x->type, Ast_type_declaration, "declaration dec_x");

    Ast_node* x = Ast_node_get(dec_x, 0);
    assert_ptr(x, "ptr x");
    expect_int_equal(x->type, Ast_type_id, "id x");
    expect_str(&x->value, "x", "x x");

    Ast_node* type_x = Ast_node_get(dec_x, 1);
    assert_ptr(type_x, "ptr type_x");
    expect_int_equal(type_x->type, Ast_type_type, "type type_x");

    Ast_node* dec_y = Ast_node_get(dseq, 1);
    assert_ptr(dec_y, "ptr dec_y");
    expect_int_equal(dec_y->type, Ast_type_declaration, "declaration dec_y");

    Ast_node* y = Ast_node_get(dec_y, 0);
    assert_ptr(y, "ptr y");
    expect_int_equal(y->type, Ast_type_id, "id y");
    expect_str(&y->value, "y", "y y");

    Ast_node* type_y = Ast_node_get(dec_y, 1);
    assert_ptr(type_y, "ptr type_y");
    expect_int_equal(type_y->type, Ast_type_type, "type type_y");

    Ast_node* dec_z = Ast_node_get(dseq, 2);
    assert_ptr(dec_z, "ptr dec_z");
    expect_int_equal(dec_z->type, Ast_type_declaration, "declaration dec_z");

    Ast_node* z = Ast_node_get(dec_z, 0);
    assert_ptr(z, "ptr z");
    expect_int_equal(z->type, Ast_type_id, "id z");
    expect_str(&z->value, "z", "z z");

    Ast_node* type_z = Ast_node_get(dec_z, 1);
    assert_ptr(type_z, "ptr type_z");
    expect_int_equal(type_z->type, Ast_type_type, "type type_z");

    Ast_node* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, Ast_type_dret, "dret dret");

    Ast_node* stmts = Ast_node_get(f, 1);
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, Ast_type_stmts, "stmts parse_stmts");

    Ast_node* one = Ast_node_get(stmts, 0);
    assert_ptr(one, "ptr one");
    expect_int_equal(one->type, Ast_type_number, "number one");
    expect_str(&one->value, "1", "1 one");

    parse_teardown(&cu);
}

void test_parse_anonymous_function2()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup(
            "let mut a: fn (i32, i32, i32)->i32\n"
            "a = fn(x: i32, y: i32, z: i32)->i32\n"
            "  1\n"
            "end\n",
            &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "parse valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* assign = Ast_node_get(cu.root, 1);
    assert_ptr(assign, "ptr assign");
    expect_int_equal(assign->type, Ast_type_assign, "assign assign");

    Ast_node* a = Ast_node_get(assign, 0);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_id, "id a");
    expect_str(&a->value, "a", "a a");

    Ast_node* f = Ast_node_get(assign, 1);
    assert_ptr(f, "ptr f");
    expect_int_equal(f->type, Ast_type_function, "type f");

    Ast_node* proto = Ast_node_get(f, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, Ast_type_prototype, "type proto");

    Ast_node* func_id = Ast_node_get(proto, 0);
    assert_ptr(func_id, "ptr func_id");
    expect_int_equal(func_id->type, Ast_type_id, "type func_id");
    expect_str(&func_id->value, "__anonymous_function_0", "value func_id");

    Ast_node* dseq = Ast_node_get(proto, 1);
    assert_ptr(dseq, "ptr dseq");
    expect_int_equal(dseq->type, Ast_type_dseq, "dseq dseq");

    Ast_node* dec_x = Ast_node_get(dseq, 0);
    assert_ptr(dec_x, "ptr dec_x");
    expect_int_equal(dec_x->type, Ast_type_declaration, "declaration dec_x");

    Ast_node* x = Ast_node_get(dec_x, 0);
    assert_ptr(x, "ptr x");
    expect_int_equal(x->type, Ast_type_id, "id x");
    expect_str(&x->value, "x", "x x");

    Ast_node* type_x = Ast_node_get(dec_x, 1);
    assert_ptr(type_x, "ptr type_x");
    expect_int_equal(type_x->type, Ast_type_type, "type_type type_x");

    Ast_node* dec_y = Ast_node_get(dseq, 1);
    assert_ptr(dec_y, "ptr dec_y");
    expect_int_equal(dec_y->type, Ast_type_declaration, "declaration dec_y");

    Ast_node* y = Ast_node_get(dec_y, 0);
    assert_ptr(y, "ptr y");
    expect_int_equal(y->type, Ast_type_id, "id y");
    expect_str(&y->value, "y", "y y");

    Ast_node* type_y = Ast_node_get(dec_y, 1);
    assert_ptr(type_y, "ptr name_y");
    expect_int_equal(type_y->type, Ast_type_type, "type type_y");

    Ast_node* dec_z = Ast_node_get(dseq, 2);
    assert_ptr(dec_z, "ptr dec_z");
    expect_int_equal(dec_z->type, Ast_type_declaration, "declaration dec_z");

    Ast_node* z = Ast_node_get(dec_z, 0);
    assert_ptr(z, "ptr z");
    expect_int_equal(z->type, Ast_type_id, "id z");
    expect_str(&z->value, "z", "z z");

    Ast_node* type_z = Ast_node_get(dec_z, 1);
    assert_ptr(type_z, "ptr type_z");
    expect_int_equal(type_z->type, Ast_type_type, "type type_z");

    Ast_node* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, Ast_type_dret, "dret dret");

    Ast_node* dret_type = Ast_node_get(dret, 0);
    assert_ptr(dret_type, "ptr dret_type");
    expect_int_equal(dret_type->type, Ast_type_type, "id dret_type");

    Ast_node* stmts = Ast_node_get(f, 1);
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, Ast_type_stmts, "stmts parse_stmts");

    Ast_node* one = Ast_node_get(stmts, 0);
    assert_ptr(one, "ptr one");
    expect_int_equal(one->type, Ast_type_number, "number one");
    expect_str(&one->value, "1", "1 one");

    parse_teardown(&cu);
}

void test_parse_anonymous_function3()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a: Function; a = fn(x: i64) let x: i64 = 1 end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "duplicate declaration in same scope: x");

    parse_teardown(&cu);
}

void test_parse_anonymous_function_assignment_error()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a: fn(bool) = fn(x: i64) end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "values in assignment are not compatible");

    parse_teardown(&cu);
}

void test_parse_anonymous_function_return_error()
{
    test_name(__func__);


    struct comp_unit cu;

    parse_setup("let f: fn()->i64 = fn()->i64 true end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "returned type does not match function return type");

    parse_teardown(&cu);
}

void test_parse_anonymous_function_expected_right_paren()
{
    test_name(__func__);


    struct comp_unit cu;

    parse_setup("fn(", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "expected right parenthesis");

    parse_teardown(&cu);
}

void test_parse_anonymous_function_expected_end()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn()", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "expected end");

    parse_teardown(&cu);
}

void test_parse_function_proto()
{
    test_name(__func__);

    struct comp_unit cu;
    parse_setup("let foo: fn (a: i64)->i64\n"
                "foo = fn (a: i64)->i64\n"
                "  a + 1\n"
                "end\n",
                &cu);
    expect_true(cu.valid, "valid");
    expect_no_errors(&cu.el);

    /* let */
    Ast_node* let = Ast_node_get(cu.root, 0);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, Ast_type_let, "type let");

    Ast_node* let_lseq = Ast_node_get(let, 0);
    assert_ptr(let_lseq, "ptr let_lseq");
    expect_int_equal(let_lseq->type, Ast_type_let_lseq, "type let_lseq");

    Ast_node* let_type = Ast_node_get(let, 1);
    assert_ptr(let_type, "ptr let_type");
    expect_int_equal(let_type->type, Ast_type_type, "type let_type");

    Type_use* let_tu = let_type->tu;
    assert_ptr(let_tu, "ptr let_tu");

    struct type_def* let_td = let_tu->td;
    assert_ptr(let_td, "ptr let_td");
    expect_int_equal(let_td->type, type_function, "type let_td");

    Type_use* inputs = let_tu->head;
    assert_ptr(inputs, "ptr inputs");
    expect_int_equal(inputs->type, Type_use_function_inputs, "type inputs");

    Type_use* outputs = inputs->next;
    assert_ptr(outputs, "ptr outputs");
    expect_int_equal(outputs->type, Type_use_function_outputs, "type outputs");

    /* assign */
    Ast_node* assign = Ast_node_get(cu.root, 1);
    assert_ptr(assign, "ptr assign");
    expect_int_equal(assign->type, Ast_type_assign, "type assign");

    parse_teardown(&cu);
}

void test_parse_function_error_let_assign()
{
    test_name(__func__);

    struct comp_unit cu;
    parse_setup("let foo: fn (a: bool)->i64 =\n"
                "  fn (a: i64)->i64\n"
                "    a + 1\n"
                "  end\n",
                &cu);
    expect_false(cu.valid, "valid");
    expect_has_errors(&cu.el);
    expect_source_error(&cu.el, "values in assignment are not compatible");

    parse_teardown(&cu);
}

void test_parse_function_error_assign()
{
    test_name(__func__);

    struct comp_unit cu;
    parse_setup("let a: i64\n"
                "a = true\n",
                &cu);
    expect_false(cu.valid, "valid");
    expect_has_errors(&cu.el);
    expect_source_error(&cu.el, "values in assignment not compatible");

    parse_teardown(&cu);
}

void test_parse_call()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo() 1 end; foo()", &cu);
    assert_no_errors(&cu.el);
    assert_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* fd = Ast_node_get(cu.root, 0);
    assert_ptr(fd, "ptr fd");
    expect_int_equal(fd->type, Ast_type_function, "function fd");

    Ast_node* proto = Ast_node_get(fd, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, Ast_type_prototype, "type proto");

    Ast_node* f_id = Ast_node_get(proto, 0);
    assert_ptr(f_id, "ptr f_id");
    expect_int_equal(f_id->type, Ast_type_id, "id f_id");
    expect_str(&f_id->value, "foo", "foo");

    Ast_node* dseq = Ast_node_get(proto, 1);
    assert_ptr(dseq, "ptr dseq");
    expect_int_equal(dseq->type, Ast_type_dseq, "dret dseq");

    Ast_node* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, Ast_type_dret, "dret dret");

    Ast_node* a = Ast_node_get(cu.root, 1);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_call, "call");

    Ast_node* b = Ast_node_get(a, 0);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_id, "id");
    expect_str(&b->value, "foo", "foo");

    Ast_node* c = Ast_node_get(a, 1);
    assert_ptr(c, "ptr c");
    expect_int_equal(c->type, Ast_type_cseq, "cseq");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_call_return_type()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo()->i64 1 end; foo() + 2", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* f = Ast_node_get(cu.root, 0);
    assert_ptr(f, "ptr f");

    Ast_node* add = Ast_node_get(cu.root, 1);
    assert_ptr(add, "ptr add");
    expect_int_equal(add->type, Ast_type_plus, "plus add");

    Type_use* add_tu = add->tu;
    assert_ptr(add_tu, "patr add_tu");

    struct type_def* add_td = add_tu->td;
    assert_ptr(add_td, "ptr add_td");
    expect_int_equal(add_td->type, type_integer, "integer add_td");
    expect_str(&add_td->name, "i64", "i64 add_td");

    parse_teardown(&cu);
}

void test_parse_call_return_type_error()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo()->bool true end; foo() + 2", &cu);
    assert_has_errors(&cu.el);
    expect_false(cu.valid, "parse_setup valid");
    expect_source_error(&cu.el, "addition on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_call2()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(arg1: i64) arg1 end; foo(2)", &cu);
    assert_no_errors(&cu.el);
    assert_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* fd = Ast_node_get(cu.root, 0);
    assert_ptr(fd, "ptr fd");
    expect_int_equal(fd->type, Ast_type_function, "function fd");

    Ast_node* proto = Ast_node_get(fd, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, Ast_type_prototype, "type proto");

    Ast_node* fname = Ast_node_get(proto, 0);
    assert_ptr(fname, "ptr fname");
    expect_int_equal(fname->type, Ast_type_id, "id fname");
    expect_str(&fname->value, "foo", "foo fname");

    Ast_node* dseq = Ast_node_get(proto, 1);
    assert_ptr(dseq, "ptr dseq");
    expect_int_equal(dseq->type, Ast_type_dseq, "dseq dseq");

    Ast_node* dparam0 = Ast_node_get(dseq, 0);
    assert_ptr(dparam0, "ptr dparam0");
    expect_int_equal(dparam0->type, Ast_type_declaration, "declaration dparam0");

    Ast_node* param0_name = Ast_node_get(dparam0, 0);
    assert_ptr(param0_name, "ptr param0_name");
    expect_int_equal(param0_name->type, Ast_type_id, "id param0_name");
    expect_str(&param0_name->value, "arg1", "arg1");

    Ast_node* type_node = Ast_node_get(dparam0, 1);
    assert_ptr(type_node, "ptr type_node");

    Type_use* tu = type_node->tu;
    assert_ptr(tu, "ptr tu");

    struct type_def* td = tu->td;
    assert_ptr(td, "ptr td");
    expect_int_equal(td->type, type_integer, "integer td");
    expect_str(&td->name, "i64", "i64 td");

    Ast_node* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, Ast_type_dret, "dret dret");

    Ast_node* f_stmts = Ast_node_get(fd, 1);
    assert_ptr(f_stmts, "ptr f_stmts");
    expect_int_equal(f_stmts->type, Ast_type_stmts, "parse_stmts f_stmts");

    Ast_node* a = Ast_node_get(cu.root, 1);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_call, "call");

    Ast_node* b = Ast_node_get(a, 0);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_id, "id");
    expect_str(&b->value, "foo", "foo");

    Ast_node* cseq = Ast_node_get(a, 1);
    assert_ptr(cseq, "ptr cseq");
    expect_int_equal(cseq->type, Ast_type_cseq, "cseq");

    Ast_node* cseq_a = Ast_node_get(cseq, 0);
    assert_ptr(cseq_a, "ptr cseq_a");
    expect_int_equal(cseq_a->type, Ast_type_number, "cseq_a");
    expect_str(&cseq_a->value, "2", "2 cseq_a");

    parse_teardown(&cu);
}

void test_parse_call3()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(arg1: i64, arg2: i64)->i64 1 end; let x: i64; let y: i64; foo(x,y)", &cu);
    assert_no_errors(&cu.el);
    assert_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* fd = Ast_node_get(cu.root, 0);
    assert_ptr(fd, "ptr fd");
    expect_int_equal(fd->type, Ast_type_function, "function fd");

    Ast_node* proto = Ast_node_get(fd, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, Ast_type_prototype, "type proto");

    Ast_node* fname = Ast_node_get(proto, 0);
    assert_ptr(fname, "ptr fname");
    expect_int_equal(fname->type, Ast_type_id, "id fname");
    expect_str(&fname->value, "foo", "foo fname");

    Ast_node* fd_seq = Ast_node_get(proto, 1);
    assert_ptr(fd_seq, "ptr fdseq");
    expect_int_equal(fd_seq->type, Ast_type_dseq, "dseq fd_seq");

    Ast_node* fd_param0 = Ast_node_get(fd_seq, 0);
    assert_ptr(fd_param0, "ptr fd_param0");
    expect_int_equal(fd_param0->type, Ast_type_declaration, "declaration fd_param0");

    Ast_node* param0_name = Ast_node_get(fd_param0, 0);
    assert_ptr(param0_name, "ptr param0_name");
    expect_int_equal(param0_name->type, Ast_type_id, "id param0_name");
    expect_str(&param0_name->value, "arg1", "arg1");

    Ast_node* param0_type = Ast_node_get(fd_param0, 1);
    assert_ptr(param0_type, "ptr param0_id");
    expect_int_equal(param0_type->type, Ast_type_type, "type param0_id");

    Ast_node* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, Ast_type_dret, "dret dret");

    Ast_node* a = Ast_node_get(cu.root, 3);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_call, "call");

    Ast_node* b = Ast_node_get(a, 0);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_id, "id");
    expect_str(&b->value, "foo", "foo");

    Ast_node* cseq = Ast_node_get(a, 1);
    assert_ptr(cseq, "ptr cseq");
    expect_int_equal(cseq->type, Ast_type_cseq, "cseq");

    Ast_node* cseq_a = Ast_node_get(cseq, 0);
    assert_ptr(cseq_a, "ptr cseq_a");
    expect_int_equal(cseq_a->type, Ast_type_id, "cseq_a");
    expect_str(&cseq_a->value, "x", "x cseq_a");

    Ast_node* cseq_b = Ast_node_get(cseq, 1);
    assert_ptr(cseq_b, "ptr cseq_b");
    expect_int_equal(cseq_b->type, Ast_type_id, "cseq_b");
    expect_str(&cseq_b->value, "y", "y cseq_b");

    Ast_node* cseq_c = Ast_node_get(cseq, 2);
    assert_null(cseq_c, "null cseq_c");

    Ast_node* c = Ast_node_get(a, 2);
    assert_null(c, "null c");

    parse_teardown(&cu);
}

void test_parse_call4()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup(
            "fn foo(arg0: i64, arg1: i64, arg2: i64)->i64 100 end; let x: i64; let y: i64; foo(x, y, 1)",
            &cu);
    assert_no_errors(&cu.el);
    assert_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* fd = Ast_node_get(cu.root, 0);
    assert_ptr(fd, "ptr fd");
    expect_int_equal(fd->type, Ast_type_function, "function fd");

    Ast_node* proto = Ast_node_get(fd, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, Ast_type_prototype, "type proto");

    Ast_node* fd_id = Ast_node_get(proto, 0);
    assert_ptr(fd_id, "ptr fd");
    expect_int_equal(fd_id->type, Ast_type_id, "id fd_id");
    expect_str(&fd_id->value, "foo", "foo fd_id");

    Ast_node* dseq = Ast_node_get(proto, 1);
    assert_ptr(dseq, "ptr dseq");
    expect_int_equal(dseq->type, Ast_type_dseq, "dseq dseq");

    Ast_node* dseq_param0 = Ast_node_get(dseq, 0);
    assert_ptr(dseq_param0, "ptr desq_param0");
    expect_int_equal(dseq_param0->type, Ast_type_declaration, "declaration dseq_param0");

    Ast_node* dseq_param0_id = Ast_node_get(dseq_param0, 0);
    assert_ptr(dseq_param0_id, "ptr dseq_param0_id");
    expect_int_equal(dseq_param0_id->type, Ast_type_id, "type dseq_param0_id");
    expect_str(&dseq_param0_id->value, "arg0", "arg0 dseq_param0_id");

    Ast_node* dseq_param0_type = Ast_node_get(dseq_param0, 1);
    assert_ptr(dseq_param0_type, "ptr dseq_param0_type");
    expect_int_equal(dseq_param0_type->type, Ast_type_type, "type dseq_param0_type");

    Ast_node* dseq_param1 = Ast_node_get(dseq, 1);
    assert_ptr(dseq_param1, "ptr desq_param1");
    expect_int_equal(dseq_param1->type, Ast_type_declaration, "declaration dseq_param1");

    Ast_node* dseq_param1_id = Ast_node_get(dseq_param1, 0);
    assert_ptr(dseq_param1_id, "ptr dseq_param1_id");
    expect_int_equal(dseq_param1_id->type, Ast_type_id, "type dseq_param1_id");
    expect_str(&dseq_param1_id->value, "arg1", "arg1 dseq_param1_id");

    Ast_node* dseq_param1_type_id = Ast_node_get(dseq_param1, 1);
    assert_ptr(dseq_param1_type_id, "ptr dseq_param1_type_id");
    expect_int_equal(dseq_param1_type_id->type, Ast_type_type, "type dseq_param1_type_id");

    Ast_node* dseq_param2 = Ast_node_get(dseq, 2);
    assert_ptr(dseq_param2, "ptr desq_param2");
    expect_int_equal(dseq_param2->type, Ast_type_declaration, "declaration dseq_param2");

    Ast_node* dseq_param2_id = Ast_node_get(dseq_param2, 0);
    assert_ptr(dseq_param2_id, "ptr dseq_param2_id");
    expect_int_equal(dseq_param2_id->type, Ast_type_id, "type dseq_param2_id");
    expect_str(&dseq_param2_id->value, "arg2", "arg2 dseq_param2_id");

    Ast_node* dseq_param2_type_id = Ast_node_get(dseq_param2, 1);
    assert_ptr(dseq_param2_type_id, "ptr dseq_param2_type_id");
    expect_int_equal(dseq_param2_type_id->type, Ast_type_type, "type dseq_param2_type_id");

    Ast_node* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, Ast_type_dret, "dret dret");

    Ast_node* dret_type_id = Ast_node_get(dret, 0);
    assert_ptr(dret_type_id, "ptr dret_type_id");
    expect_int_equal(dret_type_id->type, Ast_type_type, "type dret_type_id");

    Ast_node* call = Ast_node_get(cu.root, 3);
    assert_ptr(call, "ptr call");
    expect_int_equal(call->type, Ast_type_call, "call call");

    Ast_node* call_id = Ast_node_get(call, 0);
    assert_ptr(call_id, "ptr call_id");
    expect_int_equal(call_id->type, Ast_type_id, "id call_id");
    expect_str(&call_id->value, "foo", "foo call_id");

    Ast_node* cseq = Ast_node_get(call, 1);
    assert_ptr(cseq, "ptr cseq");
    expect_int_equal(cseq->type, Ast_type_cseq, "cseq");

    Ast_node* cseq_param0 = Ast_node_get(cseq, 0);
    assert_ptr(cseq_param0, "ptr cseq_param0");
    expect_int_equal(cseq_param0->type, Ast_type_id, "id cseq_param0");
    expect_str(&cseq_param0->value, "x", "x cseq_param0");

    Ast_node* cseq_param1 = Ast_node_get(cseq, 1);
    assert_ptr(cseq_param1, "ptr cseq_param1");
    expect_int_equal(cseq_param1->type, Ast_type_id, "id cseq_param1");
    expect_str(&cseq_param1->value, "y", "y cseq_param1");

    Ast_node* cseq_param2 = Ast_node_get(cseq, 2);
    assert_ptr(cseq_param2, "ptr cseq_param2");
    expect_int_equal(cseq_param2->type, Ast_type_number, "cseq_param2");
    expect_str(&cseq_param2->value, "1", "1 cseq_param2");

    parse_teardown(&cu);
}

void test_parse_call_missing_arguments()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(a: i64, b: i64) end; foo(1)", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse_setup valid");
    expect_source_error(&cu.el, "not enough arguments in function call");

    parse_teardown(&cu);
}

void test_parse_call_too_many_arguments()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(a: i64) end; foo(1, 2)", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse_setup valid");
    expect_source_error(&cu.el, "too many arguments in function call");

    parse_teardown(&cu);
}

void test_parse_call_type_error()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(a: i64) end; foo(true)", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse_setup valid");
    expect_source_error(&cu.el, "parameter and aguments types do not match");

    parse_teardown(&cu);
}

void test_parse_call_anonymous_function_type_error()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let foo: fn (a: i64) = fn (a: i64) end; foo(true)", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse_setup valid");
    expect_source_error(&cu.el, "parameter and aguments types do not match");

    parse_teardown(&cu);
}

void test_parse_call_error_right_paren()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo() end; foo(", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse_setup valid");
    expect_source_error(&cu.el, "expected right parenthesis");

    parse_teardown(&cu);
}

void test_parse_call_error_function_not_declared()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("foo()", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse_setup valid");
    expect_source_error(&cu.el, "variable not declared: foo");

    parse_teardown(&cu);
}

void test_parse_call_error_not_function()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let foo: i64; foo()", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse_setup valid");
    expect_source_error(&cu.el, "not a function type");

    parse_teardown(&cu);
}

void test_parse_call_error_not_enough_arguments()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(x: i64) end; foo()", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse_setup valid");
    expect_source_error(&cu.el, "not enough arguments in function call");

    parse_teardown(&cu);
}

void test_parse_call_error_too_many_arguments()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo() end; foo(1)", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse_setup valid");
    expect_source_error(&cu.el, "too many arguments in function call");

    parse_teardown(&cu);
}

void test_parse_call_error_expected_expression()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(x: i64) end; foo(1,)", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse_setup valid");
    expect_source_error(&cu.el, "expected expression after comma");

    parse_teardown(&cu);
}

void test_parse_factor_newline_anonymous_function()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn\n(\na: i64,\nb: i64,\nc: i64\n)->\ni64\na+b+c\nend", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    parse_teardown(&cu);
}

void test_parse_factor_newline_anonymous_function_let()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup(
            "let foo: fn (a: i64, b: i64, c: i64)->i64 = fn\n"
            "(\n"
            "a: i64,\n"
            "b: i64,\n"
            "c: i64\n"
            ")->\n"
            "i64\n"
            "a+b+c\n"
            "end", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    parse_teardown(&cu);
}

void test_parse_function_error_use_fn()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a: Function = fn(x: i64) end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(
            &cu.el,
            "can not directly use Function to declare a function; use fn syntax to declare a function");

    parse_teardown(&cu);
}

void test_parse_function_error_require_params_name()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a: fn(i64) = fn(i64: i64) end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "identifier reserved as a type: i64");

    parse_teardown(&cu);
}

void test_parse_function_error_input_type_non_numeric()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a: fn(i64) = fn(x: bool) end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "values in assignment are not compatible");

    parse_teardown(&cu);
}

void test_parse_function_error_output_type_non_numeric()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a: fn()->i64 = fn()->bool true end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "values in assignment are not compatible");

    parse_teardown(&cu);
}

void test_parse_function_error_input_type_numeric()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a: fn(i64) = fn(x: i32) end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "values in assignment are not compatible");

    parse_teardown(&cu);
}

void test_parse_function_error_output_type_numeric()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let a: fn()->i64 = fn()->i32 1 end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "values in assignment are not compatible");

    parse_teardown(&cu);
}

void test_parse_function()
{
    test_parse_function_no_inputs_no_outputs();
    test_parse_function_input();
    test_parse_function_multiple_inputs();
    test_parse_function_three_inputs();
    test_parse_function_error_duplicate_variable_declarations();
    test_parse_function_return_type_error();
    test_parse_function_error_expected_left_parenthesis();
    test_parse_function_error_expected_right_parenthesis();
    test_parse_function_error_duplicate_declaration();
    test_parse_function_error_identifier_reserved();
    test_parse_function_error_expected_end();
    test_parse_return();
    test_parse_return_error_no_value();
    test_parse_return_error_outside_of_function();
    test_parse_return_error_type_does_not_match();
    test_parse_stmts_newline_function();
    test_parse_anonymous_function();
    test_parse_anonymous_function2();
    test_parse_anonymous_function3();
    test_parse_anonymous_function_return_error();
    test_parse_anonymous_function_assignment_error();
    test_parse_anonymous_function_expected_right_paren();
    test_parse_anonymous_function_expected_end();
    test_parse_function_proto();
    test_parse_function_error_let_assign();
    test_parse_function_error_assign();
    test_parse_call();
    test_parse_call_return_type();
    test_parse_call_return_type_error();
    test_parse_call2();
    test_parse_call3();
    test_parse_call4();
    test_parse_call_missing_arguments();
    test_parse_call_too_many_arguments();
    test_parse_call_type_error();
    test_parse_call_anonymous_function_type_error();
    test_parse_call_error_right_paren();
    test_parse_call_error_function_not_declared();
    test_parse_call_error_not_function();
    test_parse_call_error_not_enough_arguments();
    test_parse_call_error_too_many_arguments();
    test_parse_call_error_expected_expression();
    test_parse_factor_newline_anonymous_function();
    test_parse_factor_newline_anonymous_function_let();
    test_parse_function_error_use_fn();
    test_parse_function_error_require_params_name();
    test_parse_function_error_input_type_non_numeric();
    test_parse_function_error_output_type_non_numeric();
    test_parse_function_error_input_type_numeric();
    test_parse_function_error_output_type_numeric();
}