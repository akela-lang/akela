#include "zinc/unit_test.h"
#include "akela/parse_tools.h"
#include "test_parse.h"
#include "zinc/error_unit_test.h"

void test_parse_function_no_inputs_no_outputs()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo() const x: Int32; x+1; 5+4 end", &cu);
    Zinc_assert_no_errors(&cu.errors);
    Zinc_assert_true(cu.valid, "parse_setup valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* f = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(f, "ptr f");
    Zinc_expect_int_equal(f->type, Ake_ast_type_function, "function");

    Ake_type_use* tu = f->tu;
    Zinc_assert_ptr(tu, "ptr tu");

    struct Ake_type_def* td = tu->td;
    Zinc_assert_ptr(td, "ptr td");
    Zinc_expect_int_equal(td->type, Ake_type_function, "function td");
    Zinc_expect_string(&td->name, "Function", "Function td");

    Ake_ast* proto = Ast_node_get(f, 0);
    Zinc_assert_ptr(proto, "ptr proto");
    Zinc_expect_int_equal(proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* fid = Ast_node_get(proto, 0);
    Zinc_assert_ptr(fid, "ptr a");
    Zinc_expect_int_equal(fid->type, Ake_ast_type_id, "id");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    Zinc_assert_ptr(dseq, "ptr dseq");
    Zinc_expect_int_equal(dseq->type, Ake_ast_type_dseq, "dseq dseq");

    Ake_ast* dret = Ast_node_get(proto, 2);
    Zinc_assert_ptr(dret, "ptr dret");
    Zinc_expect_int_equal(dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* stmts = Ast_node_get(f, 1);
    Zinc_assert_ptr(stmts, "ptr b");
    Zinc_expect_int_equal(stmts->type, Ake_ast_type_stmts, "parse_stmts b");

    Ake_ast* add0 = Ast_node_get(stmts, 1);
    Zinc_assert_ptr(add0, "ptr d");
    Zinc_expect_int_equal(add0->type, Ake_ast_type_plus, "plus");

    Ake_ast* x = Ast_node_get(add0, 0);
    Zinc_assert_ptr(x, "ptr x");
    Zinc_expect_int_equal(x->type, Ake_ast_type_id, "id x");
    Zinc_expect_string(&x->value, "x", "x");

    Ake_ast* number1 = Ast_node_get(add0, 1);
    Zinc_assert_ptr(number1, "ptr number1");
    Zinc_expect_int_equal(number1->type, Ake_ast_type_number, "number number1");
    Zinc_expect_string(&number1->value, "1", "1 number1");

    Ake_ast* add1 = Ast_node_get(stmts, 2);
    Zinc_assert_ptr(add1, "ptr add1");
    Zinc_expect_int_equal(add1->type, Ake_ast_type_plus, "plus add1");

    Ake_ast* number5 = Ast_node_get(add1, 0);
    Zinc_assert_ptr(number5, "ptr number5");
    Zinc_expect_int_equal(number5->type, Ake_ast_type_number, "number number5");
    Zinc_expect_string(&number5->value, "5", "5 number5");

    Ake_ast* number4 = Ast_node_get(add1, 1);
    Zinc_assert_ptr(number4, "ptr number4");
    Zinc_expect_int_equal(number4->type, Ake_ast_type_number, "number number4");
    Zinc_expect_string(&number4->value, "4", "4 number4");

    parse_teardown(&cu);
}

void test_parse_function_input()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(x: Int32) x+1; 5+4 end", &cu);
    Zinc_assert_no_errors(&cu.errors);
    Zinc_assert_true(cu.valid, "parse_setup valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* f_node = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(f_node, "ptr f_node");
    Zinc_expect_int_equal(f_node->type, Ake_ast_type_function, "function f_node");

    Ake_ast* proto = Ast_node_get(f_node, 0);
    Zinc_assert_ptr(proto, "ptr proto");
    Zinc_expect_int_equal(proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* f_id = Ast_node_get(proto, 0);
    Zinc_assert_ptr(f_id, "ptr f_id");
    Zinc_expect_int_equal(f_id->type, Ake_ast_type_id, "id f_id");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    Zinc_assert_ptr(dseq, "ptr dseq");
    Zinc_expect_int_equal(dseq->type, Ake_ast_type_dseq, "dseq");

    Ake_ast* param0 = Ast_node_get(dseq, 0);
    Zinc_assert_ptr(param0, "ptr seq_dec");
    Zinc_expect_int_equal(param0->type, Ake_ast_type_declaration, "declaration param0");

    Ake_ast* x = Ast_node_get(param0, 0);
    Zinc_assert_ptr(x, "ptr x");
    Zinc_expect_int_equal(x->type, Ake_ast_type_id, "id x");
    Zinc_expect_string(&x->value, "x", "x x");

    Ake_ast* x_type_node = Ast_node_get(param0, 1);
    Zinc_assert_ptr(x_type_node, "ptr tu_x");

    Ake_type_use* x_tu = x_type_node->tu;
    Zinc_assert_ptr(x_tu, "ptr x_tu");

    struct Ake_type_def* x_td = x_tu->td;
    Zinc_assert_ptr(x_td, "ptr x_td");
    Zinc_expect_string(&x_td->name, "Int32", "Int32 tu_x");

    Ake_ast* dret = Ast_node_get(proto, 2);
    Zinc_assert_ptr(dret, "ptr dret");
    Zinc_expect_int_equal(dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* f_stmts = Ast_node_get(f_node, 1);
    Zinc_assert_ptr(f_stmts, "ptr b");
    Zinc_expect_int_equal(f_stmts->type, Ake_ast_type_stmts, "type f_stmts");

    Ake_ast* d = Ast_node_get(f_stmts, 0);
    Zinc_assert_ptr(d, "ptr d");
    Zinc_expect_int_equal(d->type, Ake_ast_type_plus, "plus");

    Ake_ast* e = Ast_node_get(d, 0);
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_int_equal(e->type, Ake_ast_type_id, "id e");
    Zinc_expect_string(&e->value, "x", "x");

    Ake_ast* f = Ast_node_get(d, 1);
    Zinc_assert_ptr(f, "ptr f");
    Zinc_expect_int_equal(f->type, Ake_ast_type_number, "number f");
    Zinc_expect_string(&f->value, "1", "1");

    Ake_ast* g = Ast_node_get(f_stmts, 1);
    Zinc_assert_ptr(g, "ptr g");
    Zinc_expect_int_equal(g->type, Ake_ast_type_plus, "plus");

    Ake_ast* h = Ast_node_get(g, 0);
    Zinc_assert_ptr(h, "ptr h");
    Zinc_expect_int_equal(h->type, Ake_ast_type_number, "number h");
    Zinc_expect_string(&h->value, "5", "5");

    Ake_ast* i = Ast_node_get(g, 1);
    Zinc_assert_ptr(i, "ptr i");
    Zinc_expect_int_equal(i->type, Ake_ast_type_number, "number i");
    Zinc_expect_string(&i->value, "4", "4");

    parse_teardown(&cu);
}

void test_parse_function_multiple_inputs()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(x: Int32, y: Int32) x+1; 5+4 end", &cu);
    Zinc_assert_no_errors(&cu.errors);
    Zinc_assert_true(cu.valid, "parse_setup valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* f = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(f, "cu.root");
    Zinc_expect_int_equal(f->type, Ake_ast_type_function, "function");

    Ake_ast* proto = Ast_node_get(f, 0);
    Zinc_assert_ptr(proto, "ptr proto");
    Zinc_expect_int_equal(proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* a = Ast_node_get(proto, 0);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Ake_ast_type_id, "id");

    Ake_ast* seq = Ast_node_get(proto, 1);
    Zinc_assert_ptr(seq, "ptr seq");
    Zinc_expect_int_equal(seq->type, Ake_ast_type_dseq, "seq");

    Ake_ast* seq_a = Ast_node_get(seq, 0);
    Zinc_assert_ptr(seq_a, "ptr seq a");
    Zinc_expect_int_equal(seq_a->type, Ake_ast_type_declaration, "declaration seq_a");

    Ake_ast* dec_id0 = Ast_node_get(seq_a, 0);
    Zinc_assert_ptr(dec_id0, "ptr dec_id0");
    Zinc_expect_int_equal(dec_id0->type, Ake_ast_type_id, "id dec_id0");

    Ake_ast* seq_b = Ast_node_get(seq, 1);
    Zinc_assert_ptr(seq_b, "ptr seq b");
    Zinc_expect_int_equal(seq_b->type, Ake_ast_type_declaration, "declaration seq_b");

    Ake_ast* dec_id1 = Ast_node_get(seq_b, 0);
    Zinc_assert_ptr(dec_id1, "ptr dec_id1");
    Zinc_expect_int_equal(dec_id1->type, Ake_ast_type_id, "id dec_id1");

    Ake_ast* dret = Ast_node_get(proto, 2);
    Zinc_assert_ptr(dret, "ptr dret");
    Zinc_expect_int_equal(dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* b = Ast_node_get(f, 1);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Ake_ast_type_stmts, "parse_stmts");

    Ake_ast* d = Ast_node_get(b, 0);
    Zinc_assert_ptr(d, "ptr d");
    Zinc_expect_int_equal(d->type, Ake_ast_type_plus, "plus");

    Ake_ast* e = Ast_node_get(d, 0);
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_int_equal(e->type, Ake_ast_type_id, "id e");
    Zinc_expect_string(&e->value, "x", "x");

    Ake_ast* number1 = Ast_node_get(d, 1);
    Zinc_assert_ptr(number1, "ptr f");
    Zinc_expect_int_equal(number1->type, Ake_ast_type_number, "number number1");
    Zinc_expect_string(&number1->value, "1", "1 number1");

    Ake_ast* g = Ast_node_get(b, 1);
    Zinc_assert_ptr(g, "ptr g");
    Zinc_expect_int_equal(g->type, Ake_ast_type_plus, "plus");

    Ake_ast* h = Ast_node_get(g, 0);
    Zinc_assert_ptr(h, "ptr h");
    Zinc_expect_int_equal(h->type, Ake_ast_type_number, "number h");
    Zinc_expect_string(&h->value, "5", "5");

    Ake_ast* i = Ast_node_get(g, 1);
    Zinc_assert_ptr(i, "ptr i");
    Zinc_expect_int_equal(i->type, Ake_ast_type_number, "number i");
    Zinc_expect_string(&i->value, "4", "4");

    parse_teardown(&cu);
}

void test_parse_function_three_inputs()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(x: Int32, y: Int32, z: Int32)->Int32 x+1; 5+4 end", &cu);
    Zinc_assert_no_errors(&cu.errors);
    Zinc_expect_true(cu.valid, "parse valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* f = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(f, "cu.root");
    Zinc_expect_int_equal(f->type, Ake_ast_type_function, "function f");

    Ake_type_use* tu = f->tu;
    Zinc_assert_ptr(tu, "ptr tu");
    Zinc_expect_string(&tu->name, "foo", "name tu");

    struct Ake_type_def* td = tu->td;
    Zinc_assert_ptr(td, "ptr td");
    Zinc_expect_int_equal(td->type, Ake_type_function, "function td");
    Zinc_expect_string(&td->name, "Function", "Function td");

    Ake_type_use* inputs = tu->head;
    Zinc_assert_ptr(inputs, "ptr inputs");
    Zinc_expect_int_equal(inputs->type, Ake_type_use_function_inputs, "type inputs");

    Ake_type_use* outputs = inputs->next;
    Zinc_assert_ptr(outputs, "ptr outputs");
    Zinc_expect_int_equal(outputs->type, Ake_type_use_function_outputs, "type outputs");

    parse_teardown(&cu);
}

void test_parse_function_error_duplicate_variable_declarations()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(x: Int32) const x: Int32 end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse valid");
    Zinc_expect_source_error(&cu.errors, "duplicate declaration in same scope: x");

    parse_teardown(&cu);
}

void test_parse_function_return_type_error()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(x: Int32)->Int32 true end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse valid");
    Zinc_expect_source_error(&cu.errors, "returned type does not match function return type");

    parse_teardown(&cu);
}

void test_parse_function_error_expected_left_parenthesis()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "valid");
    Zinc_expect_source_error(&cu.errors, "expected left parenthesis");

    parse_teardown(&cu);
}

void test_parse_function_error_expected_right_parenthesis()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "valid");
    Zinc_expect_source_error(&cu.errors, "expected right parenthesis");

    parse_teardown(&cu);
}

void test_parse_function_error_duplicate_declaration()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo() end; fn foo() end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "valid");
    Zinc_expect_source_error(&cu.errors, "duplicate declaration in same scope: foo");

    parse_teardown(&cu);
}

void test_parse_function_error_identifier_reserved()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn Int32() end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "valid");
    Zinc_expect_source_error(&cu.errors, "identifier reserved as a type: Int32");

    parse_teardown(&cu);
}

void test_parse_function_error_expected_end()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo()", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "valid");
    Zinc_expect_source_error(&cu.errors, "expected end");

    parse_teardown(&cu);
}

void test_parse_return_error_outside_of_function()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("return true", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "valid");
    Zinc_expect_source_error(&cu.errors, "return statement outside of function");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_error, "stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_return_error_type_does_not_match()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo()->Int32 return true end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "valid");
    Zinc_expect_source_error(&cu.errors, "returned type does not match function return type");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_error, "stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_stmts_newline_function()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn\nfoo\n(\na: Int32,\nb: Int32\n)->\nInt32 1 end", &cu);
    Zinc_expect_no_errors(&cu.errors);
    Zinc_expect_true(cu.valid, "valid");

    parse_teardown(&cu);
}

void test_parse_return()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo()->Int32 return 1 end", &cu);
    Zinc_expect_no_errors(&cu.errors);
    Zinc_expect_true(cu.valid, "valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_return_error_no_value()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn bar() end; fn foo()->Int32 return bar() end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "valid");
    Zinc_expect_source_error(&cu.errors, "return expression has no value");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_error, "stmts cu.root");

    parse_teardown(&cu);
}

void test_parse_anonymous_function()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("var mut a: fn(Int32, Int32, Int32)\n"
        "a = fn (x: Int32,y: Int32,z: Int32)\n"
        "    1\n"
        "  end\n",
        &cu
    );
    Zinc_assert_no_errors(&cu.errors);
    Zinc_expect_true(cu.valid, "parse valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* assign = Ast_node_get(cu.root, 1);
    Zinc_assert_ptr(assign, "ptr assign");
    Zinc_expect_int_equal(assign->type, Ake_ast_type_assign, "assign assign");

    Ake_ast* a = Ast_node_get(assign, 0);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Ake_ast_type_id, "id a");
    Zinc_expect_string(&a->value, "a", "a a");

    Ake_ast* f = Ast_node_get(assign, 1);
    Zinc_assert_ptr(f, "ptr f");
    Zinc_expect_int_equal(f->type, Ake_ast_type_function, "type f");

    Ake_ast* proto = Ast_node_get(f, 0);
    Zinc_assert_ptr(proto, "ptr proto");
    Zinc_expect_int_equal(proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* func_id = Ast_node_get(proto, 0);
    Zinc_assert_ptr(func_id, "ptr func_id");
    Zinc_expect_int_equal(func_id->type, Ake_ast_type_id, "type func_id");
    Zinc_expect_string(&func_id->value, "__anonymous_function_0", "value func_id");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    Zinc_assert_ptr(dseq, "ptr dseq");
    Zinc_expect_int_equal(dseq->type, Ake_ast_type_dseq, "dseq dseq");

    Ake_ast* dec_x = Ast_node_get(dseq, 0);
    Zinc_assert_ptr(dec_x, "ptr dec_x");
    Zinc_expect_int_equal(dec_x->type, Ake_ast_type_declaration, "declaration dec_x");

    Ake_ast* x = Ast_node_get(dec_x, 0);
    Zinc_assert_ptr(x, "ptr x");
    Zinc_expect_int_equal(x->type, Ake_ast_type_id, "id x");
    Zinc_expect_string(&x->value, "x", "x x");

    Ake_ast* type_x = Ast_node_get(dec_x, 1);
    Zinc_assert_ptr(type_x, "ptr type_x");
    Zinc_expect_int_equal(type_x->type, Ake_ast_type_type, "type type_x");

    Ake_ast* dec_y = Ast_node_get(dseq, 1);
    Zinc_assert_ptr(dec_y, "ptr dec_y");
    Zinc_expect_int_equal(dec_y->type, Ake_ast_type_declaration, "declaration dec_y");

    Ake_ast* y = Ast_node_get(dec_y, 0);
    Zinc_assert_ptr(y, "ptr y");
    Zinc_expect_int_equal(y->type, Ake_ast_type_id, "id y");
    Zinc_expect_string(&y->value, "y", "y y");

    Ake_ast* type_y = Ast_node_get(dec_y, 1);
    Zinc_assert_ptr(type_y, "ptr type_y");
    Zinc_expect_int_equal(type_y->type, Ake_ast_type_type, "type type_y");

    Ake_ast* dec_z = Ast_node_get(dseq, 2);
    Zinc_assert_ptr(dec_z, "ptr dec_z");
    Zinc_expect_int_equal(dec_z->type, Ake_ast_type_declaration, "declaration dec_z");

    Ake_ast* z = Ast_node_get(dec_z, 0);
    Zinc_assert_ptr(z, "ptr z");
    Zinc_expect_int_equal(z->type, Ake_ast_type_id, "id z");
    Zinc_expect_string(&z->value, "z", "z z");

    Ake_ast* type_z = Ast_node_get(dec_z, 1);
    Zinc_assert_ptr(type_z, "ptr type_z");
    Zinc_expect_int_equal(type_z->type, Ake_ast_type_type, "type type_z");

    Ake_ast* dret = Ast_node_get(proto, 2);
    Zinc_assert_ptr(dret, "ptr dret");
    Zinc_expect_int_equal(dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* stmts = Ast_node_get(f, 1);
    Zinc_assert_ptr(stmts, "ptr parse_stmts");
    Zinc_expect_int_equal(stmts->type, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_ast* one = Ast_node_get(stmts, 0);
    Zinc_assert_ptr(one, "ptr one");
    Zinc_expect_int_equal(one->type, Ake_ast_type_number, "number one");
    Zinc_expect_string(&one->value, "1", "1 one");

    parse_teardown(&cu);
}

void test_parse_anonymous_function2()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup(
            "var mut a: fn (Int32, Int32, Int32)->Int32\n"
            "a = fn(x: Int32, y: Int32, z: Int32)->Int32\n"
            "  1\n"
            "end\n",
            &cu);
    Zinc_assert_no_errors(&cu.errors);
    Zinc_expect_true(cu.valid, "parse valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* assign = Ast_node_get(cu.root, 1);
    Zinc_assert_ptr(assign, "ptr assign");
    Zinc_expect_int_equal(assign->type, Ake_ast_type_assign, "assign assign");

    Ake_ast* a = Ast_node_get(assign, 0);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Ake_ast_type_id, "id a");
    Zinc_expect_string(&a->value, "a", "a a");

    Ake_ast* f = Ast_node_get(assign, 1);
    Zinc_assert_ptr(f, "ptr f");
    Zinc_expect_int_equal(f->type, Ake_ast_type_function, "type f");

    Ake_ast* proto = Ast_node_get(f, 0);
    Zinc_assert_ptr(proto, "ptr proto");
    Zinc_expect_int_equal(proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* func_id = Ast_node_get(proto, 0);
    Zinc_assert_ptr(func_id, "ptr func_id");
    Zinc_expect_int_equal(func_id->type, Ake_ast_type_id, "type func_id");
    Zinc_expect_string(&func_id->value, "__anonymous_function_0", "value func_id");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    Zinc_assert_ptr(dseq, "ptr dseq");
    Zinc_expect_int_equal(dseq->type, Ake_ast_type_dseq, "dseq dseq");

    Ake_ast* dec_x = Ast_node_get(dseq, 0);
    Zinc_assert_ptr(dec_x, "ptr dec_x");
    Zinc_expect_int_equal(dec_x->type, Ake_ast_type_declaration, "declaration dec_x");

    Ake_ast* x = Ast_node_get(dec_x, 0);
    Zinc_assert_ptr(x, "ptr x");
    Zinc_expect_int_equal(x->type, Ake_ast_type_id, "id x");
    Zinc_expect_string(&x->value, "x", "x x");

    Ake_ast* type_x = Ast_node_get(dec_x, 1);
    Zinc_assert_ptr(type_x, "ptr type_x");
    Zinc_expect_int_equal(type_x->type, Ake_ast_type_type, "type_type type_x");

    Ake_ast* dec_y = Ast_node_get(dseq, 1);
    Zinc_assert_ptr(dec_y, "ptr dec_y");
    Zinc_expect_int_equal(dec_y->type, Ake_ast_type_declaration, "declaration dec_y");

    Ake_ast* y = Ast_node_get(dec_y, 0);
    Zinc_assert_ptr(y, "ptr y");
    Zinc_expect_int_equal(y->type, Ake_ast_type_id, "id y");
    Zinc_expect_string(&y->value, "y", "y y");

    Ake_ast* type_y = Ast_node_get(dec_y, 1);
    Zinc_assert_ptr(type_y, "ptr name_y");
    Zinc_expect_int_equal(type_y->type, Ake_ast_type_type, "type type_y");

    Ake_ast* dec_z = Ast_node_get(dseq, 2);
    Zinc_assert_ptr(dec_z, "ptr dec_z");
    Zinc_expect_int_equal(dec_z->type, Ake_ast_type_declaration, "declaration dec_z");

    Ake_ast* z = Ast_node_get(dec_z, 0);
    Zinc_assert_ptr(z, "ptr z");
    Zinc_expect_int_equal(z->type, Ake_ast_type_id, "id z");
    Zinc_expect_string(&z->value, "z", "z z");

    Ake_ast* type_z = Ast_node_get(dec_z, 1);
    Zinc_assert_ptr(type_z, "ptr type_z");
    Zinc_expect_int_equal(type_z->type, Ake_ast_type_type, "type type_z");

    Ake_ast* dret = Ast_node_get(proto, 2);
    Zinc_assert_ptr(dret, "ptr dret");
    Zinc_expect_int_equal(dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* dret_type = Ast_node_get(dret, 0);
    Zinc_assert_ptr(dret_type, "ptr dret_type");
    Zinc_expect_int_equal(dret_type->type, Ake_ast_type_type, "id dret_type");

    Ake_ast* stmts = Ast_node_get(f, 1);
    Zinc_assert_ptr(stmts, "ptr parse_stmts");
    Zinc_expect_int_equal(stmts->type, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_ast* one = Ast_node_get(stmts, 0);
    Zinc_assert_ptr(one, "ptr one");
    Zinc_expect_int_equal(one->type, Ake_ast_type_number, "number one");
    Zinc_expect_string(&one->value, "1", "1 one");

    parse_teardown(&cu);
}

void test_parse_anonymous_function3()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("const a: Function; a = fn(x: Int32) const x: Int32 = 1 end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse valid");
    Zinc_expect_source_error(&cu.errors, "duplicate declaration in same scope: x");

    parse_teardown(&cu);
}

void test_parse_anonymous_function_assignment_error()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("const a: fn(Bool) = fn(x: Int32) end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse valid");
    Zinc_expect_source_error(&cu.errors, "values in assignment are not compatible");

    parse_teardown(&cu);
}

void test_parse_anonymous_function_return_error()
{
    Zinc_test_name(__func__);


    struct Ake_comp_unit cu;

    parse_setup("const f: fn()->Int32 = fn()->Int32 true end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse valid");
    Zinc_expect_source_error(&cu.errors, "returned type does not match function return type");

    parse_teardown(&cu);
}

void test_parse_anonymous_function_expected_right_paren()
{
    Zinc_test_name(__func__);


    struct Ake_comp_unit cu;

    parse_setup("fn(", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse valid");
    Zinc_expect_source_error(&cu.errors, "expected right parenthesis");

    parse_teardown(&cu);
}

void test_parse_anonymous_function_expected_end()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn()", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse valid");
    Zinc_expect_source_error(&cu.errors, "expected end");

    parse_teardown(&cu);
}

void test_parse_function_proto()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;
    parse_setup("const foo: fn (a: Int32)->Int32\n"
                "foo = fn (a: Int32)->Int32\n"
                "  a + 1\n"
                "end\n",
                &cu);
    Zinc_expect_true(cu.valid, "valid");
    Zinc_expect_no_errors(&cu.errors);

    /* const */
    Ake_ast* const_ = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(const_, "ptr const_");
    Zinc_expect_int_equal(const_->type, Ake_ast_type_const, "type const_");

    Ake_ast* const_lseq = Ast_node_get(const_, 0);
    Zinc_assert_ptr(const_lseq, "ptr let_lseq");
    Zinc_expect_int_equal(const_lseq->type, Ake_ast_type_let_lseq, "type let_lseq");

    Ake_ast* let_type = Ast_node_get(const_, 1);
    Zinc_assert_ptr(let_type, "ptr let_type");
    Zinc_expect_int_equal(let_type->type, Ake_ast_type_type, "type let_type");

    Ake_type_use* let_tu = let_type->tu;
    Zinc_assert_ptr(let_tu, "ptr let_tu");

    struct Ake_type_def* let_td = let_tu->td;
    Zinc_assert_ptr(let_td, "ptr let_td");
    Zinc_expect_int_equal(let_td->type, Ake_type_function, "type let_td");

    Ake_type_use* inputs = let_tu->head;
    Zinc_assert_ptr(inputs, "ptr inputs");
    Zinc_expect_int_equal(inputs->type, Ake_type_use_function_inputs, "type inputs");

    Ake_type_use* outputs = inputs->next;
    Zinc_assert_ptr(outputs, "ptr outputs");
    Zinc_expect_int_equal(outputs->type, Ake_type_use_function_outputs, "type outputs");

    /* assign */
    Ake_ast* assign = Ast_node_get(cu.root, 1);
    Zinc_assert_ptr(assign, "ptr assign");
    Zinc_expect_int_equal(assign->type, Ake_ast_type_assign, "type assign");

    parse_teardown(&cu);
}

void test_parse_function_error_let_assign()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;
    parse_setup("const foo: fn (a: Bool)->Int32 =\n"
                "  fn (a: Int32)->Int32\n"
                "    a + 1\n"
                "  end\n",
                &cu);
    Zinc_expect_false(cu.valid, "valid");
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_source_error(&cu.errors, "values in assignment are not compatible");

    parse_teardown(&cu);
}

void test_parse_function_error_assign()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;
    parse_setup("const a: Int32\n"
                "a = true\n",
                &cu);
    Zinc_expect_false(cu.valid, "valid");
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_source_error(&cu.errors, "values in assignment not compatible");

    parse_teardown(&cu);
}

void test_parse_call()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo() 1 end; foo()", &cu);
    Zinc_assert_no_errors(&cu.errors);
    Zinc_assert_true(cu.valid, "parse_setup valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* fd = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(fd, "ptr fd");
    Zinc_expect_int_equal(fd->type, Ake_ast_type_function, "function fd");

    Ake_ast* proto = Ast_node_get(fd, 0);
    Zinc_assert_ptr(proto, "ptr proto");
    Zinc_expect_int_equal(proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* f_id = Ast_node_get(proto, 0);
    Zinc_assert_ptr(f_id, "ptr f_id");
    Zinc_expect_int_equal(f_id->type, Ake_ast_type_id, "id f_id");
    Zinc_expect_string(&f_id->value, "foo", "foo");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    Zinc_assert_ptr(dseq, "ptr dseq");
    Zinc_expect_int_equal(dseq->type, Ake_ast_type_dseq, "dret dseq");

    Ake_ast* dret = Ast_node_get(proto, 2);
    Zinc_assert_ptr(dret, "ptr dret");
    Zinc_expect_int_equal(dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* a = Ast_node_get(cu.root, 1);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Ake_ast_type_call, "call");

    Ake_ast* b = Ast_node_get(a, 0);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Ake_ast_type_id, "id");
    Zinc_expect_string(&b->value, "foo", "foo");

    Ake_ast* c = Ast_node_get(a, 1);
    Zinc_assert_ptr(c, "ptr c");
    Zinc_expect_int_equal(c->type, Ake_ast_type_cseq, "cseq");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_call_return_type()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo()->Int32 1 end; foo() + 2", &cu);
    Zinc_assert_no_errors(&cu.errors);
    Zinc_expect_true(cu.valid, "parse_setup valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* f = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(f, "ptr f");

    Ake_ast* add = Ast_node_get(cu.root, 1);
    Zinc_assert_ptr(add, "ptr add");
    Zinc_expect_int_equal(add->type, Ake_ast_type_plus, "plus add");

    Ake_type_use* add_tu = add->tu;
    Zinc_assert_ptr(add_tu, "patr add_tu");

    struct Ake_type_def* add_td = add_tu->td;
    Zinc_assert_ptr(add_td, "ptr add_td");
    Zinc_expect_int_equal(add_td->type, Ake_type_integer, "integer add_td");
    Zinc_expect_string(&add_td->name, "Int32", "Int32 add_td");

    parse_teardown(&cu);
}

void test_parse_call_return_type_error()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo()->Bool true end; foo() + 2", &cu);
    Zinc_assert_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse_setup valid");
    Zinc_expect_source_error(&cu.errors, "addition on non-numeric operand");

    parse_teardown(&cu);
}

void test_parse_call2()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(arg1: Int32) arg1 end; foo(2)", &cu);
    Zinc_assert_no_errors(&cu.errors);
    Zinc_assert_true(cu.valid, "parse_setup valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* fd = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(fd, "ptr fd");
    Zinc_expect_int_equal(fd->type, Ake_ast_type_function, "function fd");

    Ake_ast* proto = Ast_node_get(fd, 0);
    Zinc_assert_ptr(proto, "ptr proto");
    Zinc_expect_int_equal(proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* fname = Ast_node_get(proto, 0);
    Zinc_assert_ptr(fname, "ptr fname");
    Zinc_expect_int_equal(fname->type, Ake_ast_type_id, "id fname");
    Zinc_expect_string(&fname->value, "foo", "foo fname");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    Zinc_assert_ptr(dseq, "ptr dseq");
    Zinc_expect_int_equal(dseq->type, Ake_ast_type_dseq, "dseq dseq");

    Ake_ast* dparam0 = Ast_node_get(dseq, 0);
    Zinc_assert_ptr(dparam0, "ptr dparam0");
    Zinc_expect_int_equal(dparam0->type, Ake_ast_type_declaration, "declaration dparam0");

    Ake_ast* param0_name = Ast_node_get(dparam0, 0);
    Zinc_assert_ptr(param0_name, "ptr param0_name");
    Zinc_expect_int_equal(param0_name->type, Ake_ast_type_id, "id param0_name");
    Zinc_expect_string(&param0_name->value, "arg1", "arg1");

    Ake_ast* type_node = Ast_node_get(dparam0, 1);
    Zinc_assert_ptr(type_node, "ptr type_node");

    Ake_type_use* tu = type_node->tu;
    Zinc_assert_ptr(tu, "ptr tu");

    struct Ake_type_def* td = tu->td;
    Zinc_assert_ptr(td, "ptr td");
    Zinc_expect_int_equal(td->type, Ake_type_integer, "integer td");
    Zinc_expect_string(&td->name, "Int32", "Int32 td");

    Ake_ast* dret = Ast_node_get(proto, 2);
    Zinc_assert_ptr(dret, "ptr dret");
    Zinc_expect_int_equal(dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* f_stmts = Ast_node_get(fd, 1);
    Zinc_assert_ptr(f_stmts, "ptr f_stmts");
    Zinc_expect_int_equal(f_stmts->type, Ake_ast_type_stmts, "parse_stmts f_stmts");

    Ake_ast* a = Ast_node_get(cu.root, 1);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Ake_ast_type_call, "call");

    Ake_ast* b = Ast_node_get(a, 0);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Ake_ast_type_id, "id");
    Zinc_expect_string(&b->value, "foo", "foo");

    Ake_ast* cseq = Ast_node_get(a, 1);
    Zinc_assert_ptr(cseq, "ptr cseq");
    Zinc_expect_int_equal(cseq->type, Ake_ast_type_cseq, "cseq");

    Ake_ast* cseq_a = Ast_node_get(cseq, 0);
    Zinc_assert_ptr(cseq_a, "ptr cseq_a");
    Zinc_expect_int_equal(cseq_a->type, Ake_ast_type_number, "cseq_a");
    Zinc_expect_string(&cseq_a->value, "2", "2 cseq_a");

    parse_teardown(&cu);
}

void test_parse_call3()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(arg1: Int32, arg2: Int32)->Int32 1 end; const x: Int32; const y: Int32; foo(x,y)", &cu);
    Zinc_assert_no_errors(&cu.errors);
    Zinc_assert_true(cu.valid, "parse_setup valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* fd = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(fd, "ptr fd");
    Zinc_expect_int_equal(fd->type, Ake_ast_type_function, "function fd");

    Ake_ast* proto = Ast_node_get(fd, 0);
    Zinc_assert_ptr(proto, "ptr proto");
    Zinc_expect_int_equal(proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* fname = Ast_node_get(proto, 0);
    Zinc_assert_ptr(fname, "ptr fname");
    Zinc_expect_int_equal(fname->type, Ake_ast_type_id, "id fname");
    Zinc_expect_string(&fname->value, "foo", "foo fname");

    Ake_ast* fd_seq = Ast_node_get(proto, 1);
    Zinc_assert_ptr(fd_seq, "ptr fdseq");
    Zinc_expect_int_equal(fd_seq->type, Ake_ast_type_dseq, "dseq fd_seq");

    Ake_ast* fd_param0 = Ast_node_get(fd_seq, 0);
    Zinc_assert_ptr(fd_param0, "ptr fd_param0");
    Zinc_expect_int_equal(fd_param0->type, Ake_ast_type_declaration, "declaration fd_param0");

    Ake_ast* param0_name = Ast_node_get(fd_param0, 0);
    Zinc_assert_ptr(param0_name, "ptr param0_name");
    Zinc_expect_int_equal(param0_name->type, Ake_ast_type_id, "id param0_name");
    Zinc_expect_string(&param0_name->value, "arg1", "arg1");

    Ake_ast* param0_type = Ast_node_get(fd_param0, 1);
    Zinc_assert_ptr(param0_type, "ptr param0_id");
    Zinc_expect_int_equal(param0_type->type, Ake_ast_type_type, "type param0_id");

    Ake_ast* dret = Ast_node_get(proto, 2);
    Zinc_assert_ptr(dret, "ptr dret");
    Zinc_expect_int_equal(dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* a = Ast_node_get(cu.root, 3);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Ake_ast_type_call, "call");

    Ake_ast* b = Ast_node_get(a, 0);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Ake_ast_type_id, "id");
    Zinc_expect_string(&b->value, "foo", "foo");

    Ake_ast* cseq = Ast_node_get(a, 1);
    Zinc_assert_ptr(cseq, "ptr cseq");
    Zinc_expect_int_equal(cseq->type, Ake_ast_type_cseq, "cseq");

    Ake_ast* cseq_a = Ast_node_get(cseq, 0);
    Zinc_assert_ptr(cseq_a, "ptr cseq_a");
    Zinc_expect_int_equal(cseq_a->type, Ake_ast_type_id, "cseq_a");
    Zinc_expect_string(&cseq_a->value, "x", "x cseq_a");

    Ake_ast* cseq_b = Ast_node_get(cseq, 1);
    Zinc_assert_ptr(cseq_b, "ptr cseq_b");
    Zinc_expect_int_equal(cseq_b->type, Ake_ast_type_id, "cseq_b");
    Zinc_expect_string(&cseq_b->value, "y", "y cseq_b");

    Ake_ast* cseq_c = Ast_node_get(cseq, 2);
    Zinc_assert_null(cseq_c, "null cseq_c");

    Ake_ast* c = Ast_node_get(a, 2);
    Zinc_assert_null(c, "null c");

    parse_teardown(&cu);
}

void test_parse_call4()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup(
            "fn foo(arg0: Int32, arg1: Int32, arg2: Int32)->Int32 100 end; const x: Int32; const y: Int32; foo(x, y, 1)",
            &cu);
    Zinc_assert_no_errors(&cu.errors);
    Zinc_assert_true(cu.valid, "parse_setup valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* fd = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(fd, "ptr fd");
    Zinc_expect_int_equal(fd->type, Ake_ast_type_function, "function fd");

    Ake_ast* proto = Ast_node_get(fd, 0);
    Zinc_assert_ptr(proto, "ptr proto");
    Zinc_expect_int_equal(proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* fd_id = Ast_node_get(proto, 0);
    Zinc_assert_ptr(fd_id, "ptr fd");
    Zinc_expect_int_equal(fd_id->type, Ake_ast_type_id, "id fd_id");
    Zinc_expect_string(&fd_id->value, "foo", "foo fd_id");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    Zinc_assert_ptr(dseq, "ptr dseq");
    Zinc_expect_int_equal(dseq->type, Ake_ast_type_dseq, "dseq dseq");

    Ake_ast* dseq_param0 = Ast_node_get(dseq, 0);
    Zinc_assert_ptr(dseq_param0, "ptr desq_param0");
    Zinc_expect_int_equal(dseq_param0->type, Ake_ast_type_declaration, "declaration dseq_param0");

    Ake_ast* dseq_param0_id = Ast_node_get(dseq_param0, 0);
    Zinc_assert_ptr(dseq_param0_id, "ptr dseq_param0_id");
    Zinc_expect_int_equal(dseq_param0_id->type, Ake_ast_type_id, "type dseq_param0_id");
    Zinc_expect_string(&dseq_param0_id->value, "arg0", "arg0 dseq_param0_id");

    Ake_ast* dseq_param0_type = Ast_node_get(dseq_param0, 1);
    Zinc_assert_ptr(dseq_param0_type, "ptr dseq_param0_type");
    Zinc_expect_int_equal(dseq_param0_type->type, Ake_ast_type_type, "type dseq_param0_type");

    Ake_ast* dseq_param1 = Ast_node_get(dseq, 1);
    Zinc_assert_ptr(dseq_param1, "ptr desq_param1");
    Zinc_expect_int_equal(dseq_param1->type, Ake_ast_type_declaration, "declaration dseq_param1");

    Ake_ast* dseq_param1_id = Ast_node_get(dseq_param1, 0);
    Zinc_assert_ptr(dseq_param1_id, "ptr dseq_param1_id");
    Zinc_expect_int_equal(dseq_param1_id->type, Ake_ast_type_id, "type dseq_param1_id");
    Zinc_expect_string(&dseq_param1_id->value, "arg1", "arg1 dseq_param1_id");

    Ake_ast* dseq_param1_type_id = Ast_node_get(dseq_param1, 1);
    Zinc_assert_ptr(dseq_param1_type_id, "ptr dseq_param1_type_id");
    Zinc_expect_int_equal(dseq_param1_type_id->type, Ake_ast_type_type, "type dseq_param1_type_id");

    Ake_ast* dseq_param2 = Ast_node_get(dseq, 2);
    Zinc_assert_ptr(dseq_param2, "ptr desq_param2");
    Zinc_expect_int_equal(dseq_param2->type, Ake_ast_type_declaration, "declaration dseq_param2");

    Ake_ast* dseq_param2_id = Ast_node_get(dseq_param2, 0);
    Zinc_assert_ptr(dseq_param2_id, "ptr dseq_param2_id");
    Zinc_expect_int_equal(dseq_param2_id->type, Ake_ast_type_id, "type dseq_param2_id");
    Zinc_expect_string(&dseq_param2_id->value, "arg2", "arg2 dseq_param2_id");

    Ake_ast* dseq_param2_type_id = Ast_node_get(dseq_param2, 1);
    Zinc_assert_ptr(dseq_param2_type_id, "ptr dseq_param2_type_id");
    Zinc_expect_int_equal(dseq_param2_type_id->type, Ake_ast_type_type, "type dseq_param2_type_id");

    Ake_ast* dret = Ast_node_get(proto, 2);
    Zinc_assert_ptr(dret, "ptr dret");
    Zinc_expect_int_equal(dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* dret_type_id = Ast_node_get(dret, 0);
    Zinc_assert_ptr(dret_type_id, "ptr dret_type_id");
    Zinc_expect_int_equal(dret_type_id->type, Ake_ast_type_type, "type dret_type_id");

    Ake_ast* call = Ast_node_get(cu.root, 3);
    Zinc_assert_ptr(call, "ptr call");
    Zinc_expect_int_equal(call->type, Ake_ast_type_call, "call call");

    Ake_ast* call_id = Ast_node_get(call, 0);
    Zinc_assert_ptr(call_id, "ptr call_id");
    Zinc_expect_int_equal(call_id->type, Ake_ast_type_id, "id call_id");
    Zinc_expect_string(&call_id->value, "foo", "foo call_id");

    Ake_ast* cseq = Ast_node_get(call, 1);
    Zinc_assert_ptr(cseq, "ptr cseq");
    Zinc_expect_int_equal(cseq->type, Ake_ast_type_cseq, "cseq");

    Ake_ast* cseq_param0 = Ast_node_get(cseq, 0);
    Zinc_assert_ptr(cseq_param0, "ptr cseq_param0");
    Zinc_expect_int_equal(cseq_param0->type, Ake_ast_type_id, "id cseq_param0");
    Zinc_expect_string(&cseq_param0->value, "x", "x cseq_param0");

    Ake_ast* cseq_param1 = Ast_node_get(cseq, 1);
    Zinc_assert_ptr(cseq_param1, "ptr cseq_param1");
    Zinc_expect_int_equal(cseq_param1->type, Ake_ast_type_id, "id cseq_param1");
    Zinc_expect_string(&cseq_param1->value, "y", "y cseq_param1");

    Ake_ast* cseq_param2 = Ast_node_get(cseq, 2);
    Zinc_assert_ptr(cseq_param2, "ptr cseq_param2");
    Zinc_expect_int_equal(cseq_param2->type, Ake_ast_type_number, "cseq_param2");
    Zinc_expect_string(&cseq_param2->value, "1", "1 cseq_param2");

    parse_teardown(&cu);
}

void test_parse_call_missing_arguments()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(a: Int32, b: Int32) end; foo(1)", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse_setup valid");
    Zinc_expect_source_error(&cu.errors, "not enough arguments in function call");

    parse_teardown(&cu);
}

void test_parse_call_too_many_arguments()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(a: Int32) end; foo(1, 2)", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse_setup valid");
    Zinc_expect_source_error(&cu.errors, "too many arguments in function call");

    parse_teardown(&cu);
}

void test_parse_call_type_error()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(a: Int32) end; foo(true)", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse_setup valid");
    Zinc_expect_source_error(&cu.errors, "parameter and aguments types do not match");

    parse_teardown(&cu);
}

void test_parse_call_anonymous_function_type_error()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("const foo: fn (a: Int32) = fn (a: Int32) end; foo(true)", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse_setup valid");
    Zinc_expect_source_error(&cu.errors, "parameter and aguments types do not match");

    parse_teardown(&cu);
}

void test_parse_call_error_right_paren()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo() end; foo(", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse_setup valid");
    Zinc_expect_source_error(&cu.errors, "expected right parenthesis");

    parse_teardown(&cu);
}

void test_parse_call_error_function_not_declared()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("foo()", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse_setup valid");
    Zinc_expect_source_error(&cu.errors, "variable not declared: foo");

    parse_teardown(&cu);
}

void test_parse_call_error_not_function()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("const foo: Int32; foo()", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse_setup valid");
    Zinc_expect_source_error(&cu.errors, "not a function type");

    parse_teardown(&cu);
}

void test_parse_call_error_not_enough_arguments()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(x: Int32) end; foo()", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse_setup valid");
    Zinc_expect_source_error(&cu.errors, "not enough arguments in function call");

    parse_teardown(&cu);
}

void test_parse_call_error_too_many_arguments()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo() end; foo(1)", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse_setup valid");
    Zinc_expect_source_error(&cu.errors, "too many arguments in function call");

    parse_teardown(&cu);
}

void test_parse_call_error_expected_expression()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn foo(x: Int32) end; foo(1,)", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse_setup valid");
    Zinc_expect_source_error(&cu.errors, "expected expression after comma");

    parse_teardown(&cu);
}

void test_parse_factor_newline_anonymous_function()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("fn\n(\na: Int32,\nb: Int32,\nc: Int32\n)->\nInt32\na+b+c\nend", &cu);
    Zinc_expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.errors);

    parse_teardown(&cu);
}

void test_parse_factor_newline_anonymous_function_const()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup(
            "const foo: fn (a: Int32, b: Int32, c: Int32)->Int32 = fn\n"
            "(\n"
            "a: Int32,\n"
            "b: Int32,\n"
            "c: Int32\n"
            ")->\n"
            "Int32\n"
            "a+b+c\n"
            "end", &cu);
    Zinc_expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.errors);

    parse_teardown(&cu);
}

void test_parse_function_error_use_fn()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("const a: Function = fn(x: Int32) end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse valid");
    Zinc_expect_source_error(
            &cu.errors,
            "can not directly use Function to declare a function; use fn syntax to declare a function");

    parse_teardown(&cu);
}

void test_parse_function_error_require_params_name()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("const a: fn(Int32) = fn(Int32: Int32) end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse valid");
    Zinc_expect_source_error(&cu.errors, "identifier reserved as a type: Int32");

    parse_teardown(&cu);
}

void test_parse_function_error_input_type_non_numeric()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("const a: fn(Int32) = fn(x: Bool) end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse valid");
    Zinc_expect_source_error(&cu.errors, "values in assignment are not compatible");

    parse_teardown(&cu);
}

void test_parse_function_error_output_type_non_numeric()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("const a: fn()->Int32 = fn()->Bool true end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse valid");
    Zinc_expect_source_error(&cu.errors, "values in assignment are not compatible");

    parse_teardown(&cu);
}

void test_parse_function_error_input_type_numeric()
{
    Zinc_test_name(__func__);

    Ake_comp_unit cu;

    parse_setup("const a: fn(Int64) = fn(x: Int32) end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse valid");
    Zinc_expect_source_error(&cu.errors, "values in assignment are not compatible");

    parse_teardown(&cu);
}

void test_parse_function_error_output_type_numeric()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("const a: fn()->Int64 = fn()->Int32 1 end", &cu);
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_false(cu.valid, "parse valid");
    Zinc_expect_source_error(&cu.errors, "values in assignment are not compatible");

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
    test_parse_factor_newline_anonymous_function_const();
    test_parse_function_error_use_fn();
    test_parse_function_error_require_params_name();
    test_parse_function_error_input_type_non_numeric();
    test_parse_function_error_output_type_non_numeric();
    test_parse_function_error_input_type_numeric();
    test_parse_function_error_output_type_numeric();
}