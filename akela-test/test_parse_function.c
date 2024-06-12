#include <stdbool.h>
#include "zinc/unit_test.h"
#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "test_parse.h"
#include "akela/type_def.h"
#include "akela/comp_unit.h"
#include "zinc/error_unit_test.h"

void test_parse_function_no_inputs_no_outputs()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo() let x::Int64; x+1; 5+4 end", &cu);
    assert_no_errors(&cu.el);
    assert_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

    Ast_node* f = Ast_node_get(cu.root, 0);
    assert_ptr(f, "ptr f");
    expect_int_equal(f->type, ast_type_function, "function");

    Ast_node* tu = f->tu;
    assert_ptr(tu, "ptr tu");

    struct type_def* td = tu->td;
    assert_ptr(td, "ptr td");
    expect_int_equal(td->type, type_function, "function td");
    expect_str(&td->name, "Function", "Function td");

    Ast_node* proto = Ast_node_get(f, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, ast_type_prototype, "type proto");

    Ast_node* fid = Ast_node_get(proto, 0);
    assert_ptr(fid, "ptr a");
    expect_int_equal(fid->type, ast_type_id, "id");

    Ast_node* dseq = Ast_node_get(proto, 1);
    assert_ptr(dseq, "ptr dseq");
    expect_int_equal(dseq->type, ast_type_dseq, "dseq dseq");

    Ast_node* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, ast_type_dret, "dret dret");

    Ast_node* stmts = Ast_node_get(f, 1);
    assert_ptr(stmts, "ptr b");
    expect_int_equal(stmts->type, ast_type_stmts, "parse_stmts b");

    Ast_node* add0 = Ast_node_get(stmts, 1);
    assert_ptr(add0, "ptr d");
    expect_int_equal(add0->type, ast_type_plus, "plus");

    Ast_node* x = Ast_node_get(add0, 0);
    assert_ptr(x, "ptr x");
    expect_int_equal(x->type, ast_type_id, "id x");
    expect_str(&x->value, "x", "x");

    Ast_node* number1 = Ast_node_get(add0, 1);
    assert_ptr(number1, "ptr number1");
    expect_int_equal(number1->type, ast_type_number, "number number1");
    expect_str(&number1->value, "1", "1 number1");

    Ast_node* add1 = Ast_node_get(stmts, 2);
    assert_ptr(add1, "ptr add1");
    expect_int_equal(add1->type, ast_type_plus, "plus add1");

    Ast_node* number5 = Ast_node_get(add1, 0);
    assert_ptr(number5, "ptr number5");
    expect_int_equal(number5->type, ast_type_number, "number number5");
    expect_str(&number5->value, "5", "5 number5");

    Ast_node* number4 = Ast_node_get(add1, 1);
    assert_ptr(number4, "ptr number4");
    expect_int_equal(number4->type, ast_type_number, "number number4");
    expect_str(&number4->value, "4", "4 number4");

    parse_teardown(&cu);
}

void test_parse_function_input()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(x::Int64) x+1; 5+4 end", &cu);
    assert_no_errors(&cu.el);
    assert_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

    Ast_node* f_node = Ast_node_get(cu.root, 0);
    assert_ptr(f_node, "ptr f_node");
    expect_int_equal(f_node->type, ast_type_function, "function f_node");

    Ast_node* proto = Ast_node_get(f_node, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, ast_type_prototype, "type proto");

    Ast_node* f_id = Ast_node_get(proto, 0);
    assert_ptr(f_id, "ptr f_id");
    expect_int_equal(f_id->type, ast_type_id, "id f_id");

    Ast_node* dseq = Ast_node_get(proto, 1);
    assert_ptr(dseq, "ptr dseq");
    expect_int_equal(dseq->type, ast_type_dseq, "dseq");

    Ast_node* param0 = Ast_node_get(dseq, 0);
    assert_ptr(param0, "ptr seq_dec");
    expect_int_equal(param0->type, ast_type_declaration, "declaration param0");

    Ast_node* x = Ast_node_get(param0, 0);
    assert_ptr(x, "ptr x");
    expect_int_equal(x->type, ast_type_id, "id x");
    expect_str(&x->value, "x", "x x");

    Ast_node* tu_x = Ast_node_get(param0, 1);
    assert_ptr(tu_x, "ptr tu_x");

    struct type_def* x_td = tu_x->td;
    assert_ptr(x_td, "ptr x_td");
    expect_str(&x_td->name, "Int64", "Int64 tu_x");

    Ast_node* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, ast_type_dret, "dret dret");

    Ast_node* f_stmts = Ast_node_get(f_node, 1);
    assert_ptr(f_stmts, "ptr b");
    expect_int_equal(f_stmts->type, ast_type_stmts, "type f_stmts");

    Ast_node* d = Ast_node_get(f_stmts, 0);
    assert_ptr(d, "ptr d");
    expect_int_equal(d->type, ast_type_plus, "plus");

    Ast_node* e = Ast_node_get(d, 0);
    assert_ptr(e, "ptr e");
    expect_int_equal(e->type, ast_type_id, "id e");
    expect_str(&e->value, "x", "x");

    Ast_node* f = Ast_node_get(d, 1);
    assert_ptr(f, "ptr f");
    expect_int_equal(f->type, ast_type_number, "number f");
    expect_str(&f->value, "1", "1");

    Ast_node* g = Ast_node_get(f_stmts, 1);
    assert_ptr(g, "ptr g");
    expect_int_equal(g->type, ast_type_plus, "plus");

    Ast_node* h = Ast_node_get(g, 0);
    assert_ptr(h, "ptr h");
    expect_int_equal(h->type, ast_type_number, "number h");
    expect_str(&h->value, "5", "5");

    Ast_node* i = Ast_node_get(g, 1);
    assert_ptr(i, "ptr i");
    expect_int_equal(i->type, ast_type_number, "number i");
    expect_str(&i->value, "4", "4");

    parse_teardown(&cu);
}

void test_parse_function_multiple_inputs()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(x::Int32, y::Int32) x+1; 5+4 end", &cu);
    assert_no_errors(&cu.el);
    assert_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    assert_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

    Ast_node* f = Ast_node_get(cu.root, 0);
    assert_ptr(f, "cu.root");
    expect_int_equal(f->type, ast_type_function, "function");

    Ast_node* proto = Ast_node_get(f, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, ast_type_prototype, "type proto");

    Ast_node* a = Ast_node_get(proto, 0);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, ast_type_id, "id");

    Ast_node* seq = Ast_node_get(proto, 1);
    assert_ptr(seq, "ptr seq");
    expect_int_equal(seq->type, ast_type_dseq, "seq");

    Ast_node* seq_a = Ast_node_get(seq, 0);
    assert_ptr(seq_a, "ptr seq a");
    expect_int_equal(seq_a->type, ast_type_declaration, "declaration seq_a");

    Ast_node* dec_id0 = Ast_node_get(seq_a, 0);
    assert_ptr(dec_id0, "ptr dec_id0");
    expect_int_equal(dec_id0->type, ast_type_id, "id dec_id0");

    Ast_node* seq_b = Ast_node_get(seq, 1);
    assert_ptr(seq_b, "ptr seq b");
    expect_int_equal(seq_b->type, ast_type_declaration, "declaration seq_b");

    Ast_node* dec_id1 = Ast_node_get(seq_b, 0);
    assert_ptr(dec_id1, "ptr dec_id1");
    expect_int_equal(dec_id1->type, ast_type_id, "id dec_id1");

    Ast_node* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, ast_type_dret, "dret dret");

    Ast_node* b = Ast_node_get(f, 1);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, ast_type_stmts, "parse_stmts");

    Ast_node* d = Ast_node_get(b, 0);
    assert_ptr(d, "ptr d");
    expect_int_equal(d->type, ast_type_plus, "plus");

    Ast_node* e = Ast_node_get(d, 0);
    assert_ptr(e, "ptr e");
    expect_int_equal(e->type, ast_type_id, "id e");
    expect_str(&e->value, "x", "x");

    Ast_node* number1 = Ast_node_get(d, 1);
    assert_ptr(number1, "ptr f");
    expect_int_equal(number1->type, ast_type_number, "number number1");
    expect_str(&number1->value, "1", "1 number1");

    Ast_node* g = Ast_node_get(b, 1);
    assert_ptr(g, "ptr g");
    expect_int_equal(g->type, ast_type_plus, "plus");

    Ast_node* h = Ast_node_get(g, 0);
    assert_ptr(h, "ptr h");
    expect_int_equal(h->type, ast_type_number, "number h");
    expect_str(&h->value, "5", "5");

    Ast_node* i = Ast_node_get(g, 1);
    assert_ptr(i, "ptr i");
    expect_int_equal(i->type, ast_type_number, "number i");
    expect_str(&i->value, "4", "4");

    parse_teardown(&cu);
}

void test_parse_function_three_inputs()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(x::Int32, y::Int32, z::Int32)::Int32 x+1; 5+4 end", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "parse valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

    Ast_node* f = Ast_node_get(cu.root, 0);
    assert_ptr(f, "cu.root");
    expect_int_equal(f->type, ast_type_function, "function f");

    Ast_node* tu = f->tu;
    assert_ptr(tu, "ptr tu");

    struct type_def* td = tu->td;
    assert_ptr(td, "ptr td");
    expect_int_equal(td->type, type_function, "function td");
    expect_str(&td->name, "Function", "Function td");

    Ast_node* input_tu = Ast_node_get(tu, 0);
    assert_ptr(input_tu, "ptr input_tu");

    struct type_def* input_td = input_tu->td;
    assert_ptr(input_td, "ptr input_td");
    expect_int_equal(input_td->type, type_function_input, "function_input input_td");
    expect_str(&input_td->name, "Input", "Input input_td");

    Ast_node* input0_tu = Ast_node_get(input_tu, 0);
    assert_ptr(input0_tu, "ptr input0_tu");

    struct type_def* input0_td = input0_tu->td;
    assert_ptr(input0_td, "ptr input0_td");
    expect_int_equal(input0_td->type, type_integer, "integer input0_td");
    expect_str(&input0_td->name, "Int32", "Int32 input0_td");

    Ast_node* input1_tu = Ast_node_get(input_tu, 1);
    assert_ptr(input1_tu, "ptr input1_tu");

    struct type_def* input1_td = input1_tu->td;
    assert_ptr(input1_td, "ptr input1_td");
    expect_int_equal(input1_td->type, type_integer, "integer input1_td");
    expect_str(&input1_td->name, "Int32", "Int32 input1_td");

    Ast_node* input2_tu = Ast_node_get(input_tu, 2);
    assert_ptr(input2_tu, "ptr input2_tu");

    struct type_def* input2_td = input2_tu->td;
    assert_ptr(input2_td, "ptr input2_td");
    expect_int_equal(input2_td->type, type_integer, "integer input2_td");
    expect_str(&input2_td->name, "Int32", "Int32 input2_td");

    Ast_node* output_tu = Ast_node_get(tu, 1);
    assert_ptr(output_tu, "ptr output_tu");

    struct type_def* output_td = output_tu->td;
    assert_ptr(output_td, "ptr output_td");
    expect_int_equal(output_td->type, type_function_output, "function_input output_td");
    expect_str(&output_td->name, "Output", "Output output_td");

    Ast_node* output0_tu = Ast_node_get(output_tu, 0);
    assert_ptr(output0_tu, "ptr output0_tu");

    struct type_def* output0_td = output0_tu->td;
    assert_ptr(output0_td, "ptr output0_td");
    expect_int_equal(output0_td->type, type_integer, "integer output0_td");
    expect_str(&output0_td->name, "Int32", "Int32 output0_td");

    Ast_node* proto = Ast_node_get(f, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, ast_type_prototype, "type proto");

    Ast_node* a = Ast_node_get(proto, 0);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, ast_type_id, "id");

    Ast_node* seq = Ast_node_get(proto, 1);
    assert_ptr(seq, "ptr seq");
    expect_int_equal(seq->type, ast_type_dseq, "seq");

    Ast_node* seq_a = Ast_node_get(seq, 0);
    assert_ptr(seq_a, "ptr seq a");
    expect_int_equal(seq_a->type, ast_type_declaration, "declaration seq_a");

    Ast_node* dec_a = Ast_node_get(seq_a, 0);
    assert_ptr(dec_a, "ptr dec_a");
    expect_int_equal(dec_a->type, ast_type_id, "id dec_a");

    Ast_node* seq_b = Ast_node_get(seq, 1);
    assert_ptr(seq_b, "ptr seq b");
    expect_int_equal(seq_b->type, ast_type_declaration, "declaration seq_b");

    Ast_node* dec_b = Ast_node_get(seq_b, 0);
    assert_ptr(dec_b, "ptr dec_b");
    expect_int_equal(dec_b->type, ast_type_id, "id dec_b");

    Ast_node* seq_c = Ast_node_get(seq, 2);
    assert_ptr(seq_c, "ptr seq c");
    expect_int_equal(seq_c->type, ast_type_declaration, "declaration seq_c");

    Ast_node* dec_c = Ast_node_get(seq_c, 0);
    assert_ptr(dec_c, "ptr dec_c");
    expect_int_equal(dec_c->type, ast_type_id, "id dec_c");

    Ast_node* dret = Ast_node_get(proto, 2);
    assert_ptr(dret, "ptr dret");
    expect_int_equal(dret->type, ast_type_dret, "dret dret");

    Ast_node* dret_tu = Ast_node_get(dret, 0);
    assert_ptr(dret_tu, "ptr dret_tu");
    assert_int_equal(dret_tu->type, ast_type_type, "type dret_tu");

    struct type_def* dret_td = dret_tu->td;
    assert_ptr(dret_td, "ptr dret_td");
    expect_int_equal(dret_td->type, type_integer, "integer dret_td");
    expect_str(&dret_td->name, "Int32", "Int32 dret_td");

    Ast_node* b = Ast_node_get(f, 1);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, ast_type_stmts, "parse_stmts");

    Ast_node* d = Ast_node_get(b, 0);
    assert_ptr(d, "ptr d");
    expect_int_equal(d->type, ast_type_plus, "plus");

    Ast_node* e = Ast_node_get(d, 0);
    assert_ptr(e, "ptr e");
    expect_int_equal(e->type, ast_type_id, "id e");
    expect_str(&e->value, "x", "x");

    Ast_node* number1 = Ast_node_get(d, 1);
    assert_ptr(number1, "ptr number1");
    expect_int_equal(number1->type, ast_type_number, "number number1");
    expect_str(&number1->value, "1", "1 number1");

    Ast_node* g = Ast_node_get(b, 1);
    assert_ptr(g, "ptr g");
    expect_int_equal(g->type, ast_type_plus, "plus");

    Ast_node* h = Ast_node_get(g, 0);
    assert_ptr(h, "ptr h");
    expect_int_equal(h->type, ast_type_number, "number h");
    expect_str(&h->value, "5", "5");

    Ast_node* i = Ast_node_get(g, 1);
    assert_ptr(i, "ptr i");
    expect_int_equal(i->type, ast_type_number, "number i");
    expect_str(&i->value, "4", "4");

    parse_teardown(&cu);
}

void test_parse_function_error_duplicate_variable_declarations()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(x::Int64) let x::Int64 end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "parse valid");
    expect_source_error(&cu.el, "duplicate declaration in same scope: x");

    parse_teardown(&cu);
}

void test_parse_function_return_type_error()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo(x::Int64)::Int64 true end", &cu);
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

    parse_setup("fn Int64() end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "identifier reserved as a type: Int64");

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
    expect_int_equal(cu.root->type, ast_type_error, "stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_return_error_type_does_not_match()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo()::Int64 return true end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "returned type does not match function return type");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, ast_type_error, "stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_stmts_newline_function()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn\nfoo\n(\na::Int64,\nb::Int64\n)::\nInt64 1 end", &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_return()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn foo()::Int64 return 1 end", &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_return_error_no_value()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("fn bar() end; fn foo()::Int64 return bar() end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "return expression has no value");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, ast_type_error, "stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_function_proto()
{
    test_name(__func__);

    struct comp_unit cu;
    parse_setup("let foo::fn (a::Int64)::Int64\n"
                "foo = fn (a::Int64)::Int64\n"
                "  a + 1\n"
                "end\n",
                &cu);
    expect_true(cu.valid, "valid");
    expect_no_errors(&cu.el);

    Ast_node* let = Ast_node_get(cu.root, 0);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, ast_type_let, "type let");

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
    test_parse_function_proto();
}