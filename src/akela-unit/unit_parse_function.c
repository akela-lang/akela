#include "akela/parse_tools.h"
#include "unit_parse.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_function_no_inputs_no_outputs(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() const x: Int32; x+1; 5+4 end", &cu);
    Zinc_assert_no_errors(test, &cu.errors);
    Zinc_assert_true(test, cu.valid, "AkeUnit_parse_setup valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* f = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, f, "ptr f")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f->type, Ake_ast_type_function, "function");

    Ake_type_use* tu = f->tu;
    if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

    struct Ake_type_def* td = tu->td;
    if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, td->type, Ake_type_function, "function td");
    Zinc_expect_string(test, &td->name, "Function", "Function td");

    Ake_ast* proto = Ast_node_get(f, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* fid = Ast_node_get(proto, 0);
    if (!Zinc_expect_ptr(test, fid, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fid->type, Ake_ast_type_id, "id");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    if (!Zinc_expect_ptr(test, dseq, "ptr dseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq->type, Ake_ast_type_dseq, "dseq dseq");

    Ake_ast* dret = Ast_node_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* stmts = Ast_node_get(f, 1);
    if (!Zinc_expect_ptr(test, stmts, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, stmts->type, Ake_ast_type_stmts, "parse_stmts b");

    Ake_ast* add0 = Ast_node_get(stmts, 1);
    if (!Zinc_expect_ptr(test, add0, "ptr d")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, add0->type, Ake_ast_type_plus, "plus");

    Ake_ast* x = Ast_node_get(add0, 0);
    if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, x->type, Ake_ast_type_id, "id x");
    Zinc_expect_string(test, &x->value, "x", "x");

    Ake_ast* number1 = Ast_node_get(add0, 1);
    if (!Zinc_expect_ptr(test, number1, "ptr number1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, number1->type, Ake_ast_type_number, "number number1");
    Zinc_expect_string(test, &number1->value, "1", "1 number1");

    Ake_ast* add1 = Ast_node_get(stmts, 2);
    if (!Zinc_expect_ptr(test, add1, "ptr add1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, add1->type, Ake_ast_type_plus, "plus add1");

    Ake_ast* number5 = Ast_node_get(add1, 0);
    if (!Zinc_expect_ptr(test, number5, "ptr number5")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, number5->type, Ake_ast_type_number, "number number5");
    Zinc_expect_string(test, &number5->value, "5", "5 number5");

    Ake_ast* number4 = Ast_node_get(add1, 1);
    if (!Zinc_expect_ptr(test, number4, "ptr number4")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, number4->type, Ake_ast_type_number, "number number4");
    Zinc_expect_string(test, &number4->value, "4", "4 number4");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_input(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(x: Int32) x+1; 5+4 end", &cu);
    Zinc_assert_no_errors(test, &cu.errors);
    Zinc_assert_true(test, cu.valid, "AkeUnit_parse_setup valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* f_node = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, f_node, "ptr f_node")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f_node->type, Ake_ast_type_function, "function f_node");

    Ake_ast* proto = Ast_node_get(f_node, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* f_id = Ast_node_get(proto, 0);
    if (!Zinc_expect_ptr(test, f_id, "ptr f_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f_id->type, Ake_ast_type_id, "id f_id");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    if (!Zinc_expect_ptr(test, dseq, "ptr dseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq->type, Ake_ast_type_dseq, "dseq");

    Ake_ast* param0 = Ast_node_get(dseq, 0);
    if (!Zinc_expect_ptr(test, param0, "ptr seq_dec")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, param0->type, Ake_ast_type_declaration, "declaration param0");

    Ake_ast* x = Ast_node_get(param0, 0);
    if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, x->type, Ake_ast_type_id, "id x");
    Zinc_expect_string(test, &x->value, "x", "x x");

    Ake_ast* x_type_node = Ast_node_get(param0, 1);
    if (!Zinc_expect_ptr(test, x_type_node, "ptr tu_x")) {
		return Zinc_assert();
	}

    Ake_type_use* x_tu = x_type_node->tu;
    if (!Zinc_expect_ptr(test, x_tu, "ptr x_tu")) {
		return Zinc_assert();
	}

    struct Ake_type_def* x_td = x_tu->td;
    if (!Zinc_expect_ptr(test, x_td, "ptr x_td")) {
		return Zinc_assert();
	}
    Zinc_expect_string(test, &x_td->name, "Int32", "Int32 tu_x");

    Ake_ast* dret = Ast_node_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* f_stmts = Ast_node_get(f_node, 1);
    if (!Zinc_expect_ptr(test, f_stmts, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f_stmts->type, Ake_ast_type_stmts, "type f_stmts");

    Ake_ast* d = Ast_node_get(f_stmts, 0);
    if (!Zinc_expect_ptr(test, d, "ptr d")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, d->type, Ake_ast_type_plus, "plus");

    Ake_ast* e = Ast_node_get(d, 0);
    if (!Zinc_expect_ptr(test, e, "ptr e")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, e->type, Ake_ast_type_id, "id e");
    Zinc_expect_string(test, &e->value, "x", "x");

    Ake_ast* f = Ast_node_get(d, 1);
    if (!Zinc_expect_ptr(test, f, "ptr f")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f->type, Ake_ast_type_number, "number f");
    Zinc_expect_string(test, &f->value, "1", "1");

    Ake_ast* g = Ast_node_get(f_stmts, 1);
    if (!Zinc_expect_ptr(test, g, "ptr g")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, g->type, Ake_ast_type_plus, "plus");

    Ake_ast* h = Ast_node_get(g, 0);
    if (!Zinc_expect_ptr(test, h, "ptr h")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, h->type, Ake_ast_type_number, "number h");
    Zinc_expect_string(test, &h->value, "5", "5");

    Ake_ast* i = Ast_node_get(g, 1);
    if (!Zinc_expect_ptr(test, i, "ptr i")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, i->type, Ake_ast_type_number, "number i");
    Zinc_expect_string(test, &i->value, "4", "4");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_multiple_inputs(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(x: Int32, y: Int32) x+1; 5+4 end", &cu);
    Zinc_assert_no_errors(test, &cu.errors);
    Zinc_assert_true(test, cu.valid, "AkeUnit_parse_setup valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_assert_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* f = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, f, "cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f->type, Ake_ast_type_function, "function");

    Ake_ast* proto = Ast_node_get(f, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* a = Ast_node_get(proto, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Ake_ast_type_id, "id");

    Ake_ast* seq = Ast_node_get(proto, 1);
    if (!Zinc_expect_ptr(test, seq, "ptr seq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, seq->type, Ake_ast_type_dseq, "seq");

    Ake_ast* seq_a = Ast_node_get(seq, 0);
    if (!Zinc_expect_ptr(test, seq_a, "ptr seq a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, seq_a->type, Ake_ast_type_declaration, "declaration seq_a");

    Ake_ast* dec_id0 = Ast_node_get(seq_a, 0);
    if (!Zinc_expect_ptr(test, dec_id0, "ptr dec_id0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_id0->type, Ake_ast_type_id, "id dec_id0");

    Ake_ast* seq_b = Ast_node_get(seq, 1);
    if (!Zinc_expect_ptr(test, seq_b, "ptr seq b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, seq_b->type, Ake_ast_type_declaration, "declaration seq_b");

    Ake_ast* dec_id1 = Ast_node_get(seq_b, 0);
    if (!Zinc_expect_ptr(test, dec_id1, "ptr dec_id1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_id1->type, Ake_ast_type_id, "id dec_id1");

    Ake_ast* dret = Ast_node_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* b = Ast_node_get(f, 1);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Ake_ast_type_stmts, "parse_stmts");

    Ake_ast* d = Ast_node_get(b, 0);
    if (!Zinc_expect_ptr(test, d, "ptr d")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, d->type, Ake_ast_type_plus, "plus");

    Ake_ast* e = Ast_node_get(d, 0);
    if (!Zinc_expect_ptr(test, e, "ptr e")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, e->type, Ake_ast_type_id, "id e");
    Zinc_expect_string(test, &e->value, "x", "x");

    Ake_ast* number1 = Ast_node_get(d, 1);
    if (!Zinc_expect_ptr(test, number1, "ptr f")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, number1->type, Ake_ast_type_number, "number number1");
    Zinc_expect_string(test, &number1->value, "1", "1 number1");

    Ake_ast* g = Ast_node_get(b, 1);
    if (!Zinc_expect_ptr(test, g, "ptr g")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, g->type, Ake_ast_type_plus, "plus");

    Ake_ast* h = Ast_node_get(g, 0);
    if (!Zinc_expect_ptr(test, h, "ptr h")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, h->type, Ake_ast_type_number, "number h");
    Zinc_expect_string(test, &h->value, "5", "5");

    Ake_ast* i = Ast_node_get(g, 1);
    if (!Zinc_expect_ptr(test, i, "ptr i")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, i->type, Ake_ast_type_number, "number i");
    Zinc_expect_string(test, &i->value, "4", "4");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_three_inputs(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(x: Int32, y: Int32, z: Int32)->Int32 x+1; 5+4 end", &cu);
    Zinc_assert_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "parse valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* f = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, f, "cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f->type, Ake_ast_type_function, "function f");

    Ake_type_use* tu = f->tu;
    if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}
    Zinc_expect_string(test, &tu->name, "foo", "name tu");

    struct Ake_type_def* td = tu->td;
    if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, td->type, Ake_type_function, "function td");
    Zinc_expect_string(test, &td->name, "Function", "Function td");

    Ake_type_use* inputs = tu->head;
    if (!Zinc_expect_ptr(test, inputs, "ptr inputs")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, inputs->type, Ake_type_use_function_inputs, "type inputs");

    Ake_type_use* outputs = inputs->next;
    if (!Zinc_expect_ptr(test, outputs, "ptr outputs")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, outputs->type, Ake_type_use_function_outputs, "type outputs");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_duplicate_variable_declarations(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(x: Int32) const x: Int32 end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "duplicate declaration in same scope: x");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_return_type_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(x: Int32)->Int32 true end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "returned type does not match function return type");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_expected_left_parenthesis(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "expected left parenthesis");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_expected_right_parenthesis(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "expected right parenthesis");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_duplicate_declaration(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; fn foo() end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "duplicate declaration in same scope: foo");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_identifier_reserved(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn Int32() end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "identifier reserved as a type: Int32");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_expected_end(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo()", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "expected end");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_return_error_outside_of_function(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("return true", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "return statement outside of function");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_error, "stmts cu.root");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_return_error_type_does_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo()->Int32 return true end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "returned type does not match function return type");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_error, "stmts cu.root");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_stmts_newline_function(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn\nfoo\n(\na: Int32,\nb: Int32\n)->\nInt32 1 end", &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_return(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo()->Int32 return 1 end", &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_return_error_no_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn bar() end; fn foo()->Int32 return bar() end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "return expression has no value");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_error, "stmts cu.root");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_anonymous_function(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("var mut a: fn(Int32, Int32, Int32)\n"
        "a = fn (x: Int32,y: Int32,z: Int32)\n"
        "    1\n"
        "  end\n",
        &cu
    );
    Zinc_assert_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "parse valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* assign = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, assign, "ptr assign")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign->type, Ake_ast_type_assign, "assign assign");

    Ake_ast* a = Ast_node_get(assign, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Ake_ast_type_id, "id a");
    Zinc_expect_string(test, &a->value, "a", "a a");

    Ake_ast* f = Ast_node_get(assign, 1);
    if (!Zinc_expect_ptr(test, f, "ptr f")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f->type, Ake_ast_type_function, "type f");

    Ake_ast* proto = Ast_node_get(f, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* func_id = Ast_node_get(proto, 0);
    if (!Zinc_expect_ptr(test, func_id, "ptr func_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, func_id->type, Ake_ast_type_id, "type func_id");
    Zinc_expect_string(test, &func_id->value, "__anonymous_function_0", "value func_id");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    if (!Zinc_expect_ptr(test, dseq, "ptr dseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq->type, Ake_ast_type_dseq, "dseq dseq");

    Ake_ast* dec_x = Ast_node_get(dseq, 0);
    if (!Zinc_expect_ptr(test, dec_x, "ptr dec_x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_x->type, Ake_ast_type_declaration, "declaration dec_x");

    Ake_ast* x = Ast_node_get(dec_x, 0);
    if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, x->type, Ake_ast_type_id, "id x");
    Zinc_expect_string(test, &x->value, "x", "x x");

    Ake_ast* type_x = Ast_node_get(dec_x, 1);
    if (!Zinc_expect_ptr(test, type_x, "ptr type_x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type_x->type, Ake_ast_type_type, "type type_x");

    Ake_ast* dec_y = Ast_node_get(dseq, 1);
    if (!Zinc_expect_ptr(test, dec_y, "ptr dec_y")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_y->type, Ake_ast_type_declaration, "declaration dec_y");

    Ake_ast* y = Ast_node_get(dec_y, 0);
    if (!Zinc_expect_ptr(test, y, "ptr y")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, y->type, Ake_ast_type_id, "id y");
    Zinc_expect_string(test, &y->value, "y", "y y");

    Ake_ast* type_y = Ast_node_get(dec_y, 1);
    if (!Zinc_expect_ptr(test, type_y, "ptr type_y")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type_y->type, Ake_ast_type_type, "type type_y");

    Ake_ast* dec_z = Ast_node_get(dseq, 2);
    if (!Zinc_expect_ptr(test, dec_z, "ptr dec_z")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_z->type, Ake_ast_type_declaration, "declaration dec_z");

    Ake_ast* z = Ast_node_get(dec_z, 0);
    if (!Zinc_expect_ptr(test, z, "ptr z")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, z->type, Ake_ast_type_id, "id z");
    Zinc_expect_string(test, &z->value, "z", "z z");

    Ake_ast* type_z = Ast_node_get(dec_z, 1);
    if (!Zinc_expect_ptr(test, type_z, "ptr type_z")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type_z->type, Ake_ast_type_type, "type type_z");

    Ake_ast* dret = Ast_node_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* stmts = Ast_node_get(f, 1);
    if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, stmts->type, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_ast* one = Ast_node_get(stmts, 0);
    if (!Zinc_expect_ptr(test, one, "ptr one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->type, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1 one");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_anonymous_function2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup(
            "var mut a: fn (Int32, Int32, Int32)->Int32\n"
            "a = fn(x: Int32, y: Int32, z: Int32)->Int32\n"
            "  1\n"
            "end\n",
            &cu);
    Zinc_assert_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "parse valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* assign = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, assign, "ptr assign")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign->type, Ake_ast_type_assign, "assign assign");

    Ake_ast* a = Ast_node_get(assign, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Ake_ast_type_id, "id a");
    Zinc_expect_string(test, &a->value, "a", "a a");

    Ake_ast* f = Ast_node_get(assign, 1);
    if (!Zinc_expect_ptr(test, f, "ptr f")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f->type, Ake_ast_type_function, "type f");

    Ake_ast* proto = Ast_node_get(f, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* func_id = Ast_node_get(proto, 0);
    if (!Zinc_expect_ptr(test, func_id, "ptr func_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, func_id->type, Ake_ast_type_id, "type func_id");
    Zinc_expect_string(test, &func_id->value, "__anonymous_function_0", "value func_id");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    if (!Zinc_expect_ptr(test, dseq, "ptr dseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq->type, Ake_ast_type_dseq, "dseq dseq");

    Ake_ast* dec_x = Ast_node_get(dseq, 0);
    if (!Zinc_expect_ptr(test, dec_x, "ptr dec_x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_x->type, Ake_ast_type_declaration, "declaration dec_x");

    Ake_ast* x = Ast_node_get(dec_x, 0);
    if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, x->type, Ake_ast_type_id, "id x");
    Zinc_expect_string(test, &x->value, "x", "x x");

    Ake_ast* type_x = Ast_node_get(dec_x, 1);
    if (!Zinc_expect_ptr(test, type_x, "ptr type_x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type_x->type, Ake_ast_type_type, "type_type type_x");

    Ake_ast* dec_y = Ast_node_get(dseq, 1);
    if (!Zinc_expect_ptr(test, dec_y, "ptr dec_y")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_y->type, Ake_ast_type_declaration, "declaration dec_y");

    Ake_ast* y = Ast_node_get(dec_y, 0);
    if (!Zinc_expect_ptr(test, y, "ptr y")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, y->type, Ake_ast_type_id, "id y");
    Zinc_expect_string(test, &y->value, "y", "y y");

    Ake_ast* type_y = Ast_node_get(dec_y, 1);
    if (!Zinc_expect_ptr(test, type_y, "ptr name_y")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type_y->type, Ake_ast_type_type, "type type_y");

    Ake_ast* dec_z = Ast_node_get(dseq, 2);
    if (!Zinc_expect_ptr(test, dec_z, "ptr dec_z")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_z->type, Ake_ast_type_declaration, "declaration dec_z");

    Ake_ast* z = Ast_node_get(dec_z, 0);
    if (!Zinc_expect_ptr(test, z, "ptr z")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, z->type, Ake_ast_type_id, "id z");
    Zinc_expect_string(test, &z->value, "z", "z z");

    Ake_ast* type_z = Ast_node_get(dec_z, 1);
    if (!Zinc_expect_ptr(test, type_z, "ptr type_z")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type_z->type, Ake_ast_type_type, "type type_z");

    Ake_ast* dret = Ast_node_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* dret_type = Ast_node_get(dret, 0);
    if (!Zinc_expect_ptr(test, dret_type, "ptr dret_type")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret_type->type, Ake_ast_type_type, "id dret_type");

    Ake_ast* stmts = Ast_node_get(f, 1);
    if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, stmts->type, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_ast* one = Ast_node_get(stmts, 0);
    if (!Zinc_expect_ptr(test, one, "ptr one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->type, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1 one");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_anonymous_function3(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: Function; a = fn(x: Int32) const x: Int32 = 1 end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "duplicate declaration in same scope: x");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_anonymous_function_assignment_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: fn(Bool) = fn(x: Int32) end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "values in assignment are not compatible");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_anonymous_function_return_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }


    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const f: fn()->Int32 = fn()->Int32 true end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "returned type does not match function return type");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_anonymous_function_expected_right_paren(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }


    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn(", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "expected right parenthesis");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_anonymous_function_expected_end(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn()", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "expected end");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_proto(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;
    AkeUnit_parse_setup("const foo: fn (a: Int32)->Int32\n"
                "foo = fn (a: Int32)->Int32\n"
                "  a + 1\n"
                "end\n",
                &cu);
    Zinc_expect_true(test, cu.valid, "valid");
    Zinc_expect_no_errors(test, &cu.errors);

    /* const */
    Ake_ast* const_ = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, const_, "ptr const_")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, const_->type, Ake_ast_type_const, "type const_");

    Ake_ast* const_lseq = Ast_node_get(const_, 0);
    if (!Zinc_expect_ptr(test, const_lseq, "ptr let_lseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, const_lseq->type, Ake_ast_type_let_lseq, "type let_lseq");

    Ake_ast* let_type = Ast_node_get(const_, 1);
    if (!Zinc_expect_ptr(test, let_type, "ptr let_type")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, let_type->type, Ake_ast_type_type, "type let_type");

    Ake_type_use* let_tu = let_type->tu;
    if (!Zinc_expect_ptr(test, let_tu, "ptr let_tu")) {
		return Zinc_assert();
	}

    struct Ake_type_def* let_td = let_tu->td;
    if (!Zinc_expect_ptr(test, let_td, "ptr let_td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, let_td->type, Ake_type_function, "type let_td");

    Ake_type_use* inputs = let_tu->head;
    if (!Zinc_expect_ptr(test, inputs, "ptr inputs")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, inputs->type, Ake_type_use_function_inputs, "type inputs");

    Ake_type_use* outputs = inputs->next;
    if (!Zinc_expect_ptr(test, outputs, "ptr outputs")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, outputs->type, Ake_type_use_function_outputs, "type outputs");

    /* assign */
    Ake_ast* assign = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, assign, "ptr assign")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign->type, Ake_ast_type_assign, "type assign");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_let_assign(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;
    AkeUnit_parse_setup("const foo: fn (a: Bool)->Int32 =\n"
                "  fn (a: Int32)->Int32\n"
                "    a + 1\n"
                "  end\n",
                &cu);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_source_error(test, &cu.errors, "values in assignment are not compatible");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_assign(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;
    AkeUnit_parse_setup("const a: Int32\n"
                "a = true\n",
                &cu);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_source_error(test, &cu.errors, "values in assignment not compatible");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() 1 end; foo()", &cu);
    Zinc_assert_no_errors(test, &cu.errors);
    Zinc_assert_true(test, cu.valid, "AkeUnit_parse_setup valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* fd = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, fd, "ptr fd")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd->type, Ake_ast_type_function, "function fd");

    Ake_ast* proto = Ast_node_get(fd, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* f_id = Ast_node_get(proto, 0);
    if (!Zinc_expect_ptr(test, f_id, "ptr f_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f_id->type, Ake_ast_type_id, "id f_id");
    Zinc_expect_string(test, &f_id->value, "foo", "foo");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    if (!Zinc_expect_ptr(test, dseq, "ptr dseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq->type, Ake_ast_type_dseq, "dret dseq");

    Ake_ast* dret = Ast_node_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* a = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Ake_ast_type_call, "call");

    Ake_ast* b = Ast_node_get(a, 0);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &b->value, "foo", "foo");

    Ake_ast* c = Ast_node_get(a, 1);
    if (!Zinc_expect_ptr(test, c, "ptr c")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c->type, Ake_ast_type_cseq, "cseq");

    AkeUnit_parse_teardown(&cu);
}

/* dynamic-output-none */
void AkeUnit_parse_call_return_type(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo()->Int32 1 end; foo() + 2", &cu);
    Zinc_assert_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* f = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, f, "ptr f")) {
		return Zinc_assert();
	}

    Ake_ast* add = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, add, "ptr add")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, add->type, Ake_ast_type_plus, "plus add");

    Ake_type_use* add_tu = add->tu;
    if (!Zinc_expect_ptr(test, add_tu, "patr add_tu")) {
		return Zinc_assert();
	}

    struct Ake_type_def* add_td = add_tu->td;
    if (!Zinc_expect_ptr(test, add_td, "ptr add_td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, add_td->type, Ake_type_integer, "integer add_td");
    Zinc_expect_string(test, &add_td->name, "Int32", "Int32 add_td");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call_return_type_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo()->Bool true end; foo() + 2", &cu);
    Zinc_assert_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_source_error(test, &cu.errors, "addition on non-numeric operand");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(arg1: Int32) arg1 end; foo(2)", &cu);
    Zinc_assert_no_errors(test, &cu.errors);
    Zinc_assert_true(test, cu.valid, "AkeUnit_parse_setup valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* fd = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, fd, "ptr fd")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd->type, Ake_ast_type_function, "function fd");

    Ake_ast* proto = Ast_node_get(fd, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* fname = Ast_node_get(proto, 0);
    if (!Zinc_expect_ptr(test, fname, "ptr fname")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fname->type, Ake_ast_type_id, "id fname");
    Zinc_expect_string(test, &fname->value, "foo", "foo fname");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    if (!Zinc_expect_ptr(test, dseq, "ptr dseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq->type, Ake_ast_type_dseq, "dseq dseq");

    Ake_ast* dparam0 = Ast_node_get(dseq, 0);
    if (!Zinc_expect_ptr(test, dparam0, "ptr dparam0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dparam0->type, Ake_ast_type_declaration, "declaration dparam0");

    Ake_ast* param0_name = Ast_node_get(dparam0, 0);
    if (!Zinc_expect_ptr(test, param0_name, "ptr param0_name")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, param0_name->type, Ake_ast_type_id, "id param0_name");
    Zinc_expect_string(test, &param0_name->value, "arg1", "arg1");

    Ake_ast* type_node = Ast_node_get(dparam0, 1);
    if (!Zinc_expect_ptr(test, type_node, "ptr type_node")) {
		return Zinc_assert();
	}

    Ake_type_use* tu = type_node->tu;
    if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
		return Zinc_assert();
	}

    struct Ake_type_def* td = tu->td;
    if (!Zinc_expect_ptr(test, td, "ptr td")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, td->type, Ake_type_integer, "integer td");
    Zinc_expect_string(test, &td->name, "Int32", "Int32 td");

    Ake_ast* dret = Ast_node_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* f_stmts = Ast_node_get(fd, 1);
    if (!Zinc_expect_ptr(test, f_stmts, "ptr f_stmts")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f_stmts->type, Ake_ast_type_stmts, "parse_stmts f_stmts");

    Ake_ast* a = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Ake_ast_type_call, "call");

    Ake_ast* b = Ast_node_get(a, 0);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &b->value, "foo", "foo");

    Ake_ast* cseq = Ast_node_get(a, 1);
    if (!Zinc_expect_ptr(test, cseq, "ptr cseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq->type, Ake_ast_type_cseq, "cseq");

    Ake_ast* cseq_a = Ast_node_get(cseq, 0);
    if (!Zinc_expect_ptr(test, cseq_a, "ptr cseq_a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq_a->type, Ake_ast_type_number, "cseq_a");
    Zinc_expect_string(test, &cseq_a->value, "2", "2 cseq_a");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call3(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(arg1: Int32, arg2: Int32)->Int32 1 end; const x: Int32; const y: Int32; foo(x,y)", &cu);
    Zinc_assert_no_errors(test, &cu.errors);
    Zinc_assert_true(test, cu.valid, "AkeUnit_parse_setup valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* fd = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, fd, "ptr fd")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd->type, Ake_ast_type_function, "function fd");

    Ake_ast* proto = Ast_node_get(fd, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* fname = Ast_node_get(proto, 0);
    if (!Zinc_expect_ptr(test, fname, "ptr fname")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fname->type, Ake_ast_type_id, "id fname");
    Zinc_expect_string(test, &fname->value, "foo", "foo fname");

    Ake_ast* fd_seq = Ast_node_get(proto, 1);
    if (!Zinc_expect_ptr(test, fd_seq, "ptr fdseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd_seq->type, Ake_ast_type_dseq, "dseq fd_seq");

    Ake_ast* fd_param0 = Ast_node_get(fd_seq, 0);
    if (!Zinc_expect_ptr(test, fd_param0, "ptr fd_param0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd_param0->type, Ake_ast_type_declaration, "declaration fd_param0");

    Ake_ast* param0_name = Ast_node_get(fd_param0, 0);
    if (!Zinc_expect_ptr(test, param0_name, "ptr param0_name")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, param0_name->type, Ake_ast_type_id, "id param0_name");
    Zinc_expect_string(test, &param0_name->value, "arg1", "arg1");

    Ake_ast* param0_type = Ast_node_get(fd_param0, 1);
    if (!Zinc_expect_ptr(test, param0_type, "ptr param0_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, param0_type->type, Ake_ast_type_type, "type param0_id");

    Ake_ast* dret = Ast_node_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* a = Ast_node_get(cu.root, 3);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Ake_ast_type_call, "call");

    Ake_ast* b = Ast_node_get(a, 0);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &b->value, "foo", "foo");

    Ake_ast* cseq = Ast_node_get(a, 1);
    if (!Zinc_expect_ptr(test, cseq, "ptr cseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq->type, Ake_ast_type_cseq, "cseq");

    Ake_ast* cseq_a = Ast_node_get(cseq, 0);
    if (!Zinc_expect_ptr(test, cseq_a, "ptr cseq_a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq_a->type, Ake_ast_type_id, "cseq_a");
    Zinc_expect_string(test, &cseq_a->value, "x", "x cseq_a");

    Ake_ast* cseq_b = Ast_node_get(cseq, 1);
    if (!Zinc_expect_ptr(test, cseq_b, "ptr cseq_b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq_b->type, Ake_ast_type_id, "cseq_b");
    Zinc_expect_string(test, &cseq_b->value, "y", "y cseq_b");

    Ake_ast* cseq_c = Ast_node_get(cseq, 2);
    Zinc_assert_null(test, cseq_c, "null cseq_c");

    Ake_ast* c = Ast_node_get(a, 2);
    Zinc_assert_null(test, c, "null c");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call4(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup(
            "fn foo(arg0: Int32, arg1: Int32, arg2: Int32)->Int32 100 end; const x: Int32; const y: Int32; foo(x, y, 1)",
            &cu);
    Zinc_assert_no_errors(test, &cu.errors);
    Zinc_assert_true(test, cu.valid, "AkeUnit_parse_setup valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* fd = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, fd, "ptr fd")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd->type, Ake_ast_type_function, "function fd");

    Ake_ast* proto = Ast_node_get(fd, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->type, Ake_ast_type_prototype, "type proto");

    Ake_ast* fd_id = Ast_node_get(proto, 0);
    if (!Zinc_expect_ptr(test, fd_id, "ptr fd")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd_id->type, Ake_ast_type_id, "id fd_id");
    Zinc_expect_string(test, &fd_id->value, "foo", "foo fd_id");

    Ake_ast* dseq = Ast_node_get(proto, 1);
    if (!Zinc_expect_ptr(test, dseq, "ptr dseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq->type, Ake_ast_type_dseq, "dseq dseq");

    Ake_ast* dseq_param0 = Ast_node_get(dseq, 0);
    if (!Zinc_expect_ptr(test, dseq_param0, "ptr desq_param0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param0->type, Ake_ast_type_declaration, "declaration dseq_param0");

    Ake_ast* dseq_param0_id = Ast_node_get(dseq_param0, 0);
    if (!Zinc_expect_ptr(test, dseq_param0_id, "ptr dseq_param0_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param0_id->type, Ake_ast_type_id, "type dseq_param0_id");
    Zinc_expect_string(test, &dseq_param0_id->value, "arg0", "arg0 dseq_param0_id");

    Ake_ast* dseq_param0_type = Ast_node_get(dseq_param0, 1);
    if (!Zinc_expect_ptr(test, dseq_param0_type, "ptr dseq_param0_type")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param0_type->type, Ake_ast_type_type, "type dseq_param0_type");

    Ake_ast* dseq_param1 = Ast_node_get(dseq, 1);
    if (!Zinc_expect_ptr(test, dseq_param1, "ptr desq_param1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param1->type, Ake_ast_type_declaration, "declaration dseq_param1");

    Ake_ast* dseq_param1_id = Ast_node_get(dseq_param1, 0);
    if (!Zinc_expect_ptr(test, dseq_param1_id, "ptr dseq_param1_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param1_id->type, Ake_ast_type_id, "type dseq_param1_id");
    Zinc_expect_string(test, &dseq_param1_id->value, "arg1", "arg1 dseq_param1_id");

    Ake_ast* dseq_param1_type_id = Ast_node_get(dseq_param1, 1);
    if (!Zinc_expect_ptr(test, dseq_param1_type_id, "ptr dseq_param1_type_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param1_type_id->type, Ake_ast_type_type, "type dseq_param1_type_id");

    Ake_ast* dseq_param2 = Ast_node_get(dseq, 2);
    if (!Zinc_expect_ptr(test, dseq_param2, "ptr desq_param2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param2->type, Ake_ast_type_declaration, "declaration dseq_param2");

    Ake_ast* dseq_param2_id = Ast_node_get(dseq_param2, 0);
    if (!Zinc_expect_ptr(test, dseq_param2_id, "ptr dseq_param2_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param2_id->type, Ake_ast_type_id, "type dseq_param2_id");
    Zinc_expect_string(test, &dseq_param2_id->value, "arg2", "arg2 dseq_param2_id");

    Ake_ast* dseq_param2_type_id = Ast_node_get(dseq_param2, 1);
    if (!Zinc_expect_ptr(test, dseq_param2_type_id, "ptr dseq_param2_type_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param2_type_id->type, Ake_ast_type_type, "type dseq_param2_type_id");

    Ake_ast* dret = Ast_node_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->type, Ake_ast_type_dret, "dret dret");

    Ake_ast* dret_type_id = Ast_node_get(dret, 0);
    if (!Zinc_expect_ptr(test, dret_type_id, "ptr dret_type_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret_type_id->type, Ake_ast_type_type, "type dret_type_id");

    Ake_ast* call = Ast_node_get(cu.root, 3);
    if (!Zinc_expect_ptr(test, call, "ptr call")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, call->type, Ake_ast_type_call, "call call");

    Ake_ast* call_id = Ast_node_get(call, 0);
    if (!Zinc_expect_ptr(test, call_id, "ptr call_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, call_id->type, Ake_ast_type_id, "id call_id");
    Zinc_expect_string(test, &call_id->value, "foo", "foo call_id");

    Ake_ast* cseq = Ast_node_get(call, 1);
    if (!Zinc_expect_ptr(test, cseq, "ptr cseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq->type, Ake_ast_type_cseq, "cseq");

    Ake_ast* cseq_param0 = Ast_node_get(cseq, 0);
    if (!Zinc_expect_ptr(test, cseq_param0, "ptr cseq_param0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq_param0->type, Ake_ast_type_id, "id cseq_param0");
    Zinc_expect_string(test, &cseq_param0->value, "x", "x cseq_param0");

    Ake_ast* cseq_param1 = Ast_node_get(cseq, 1);
    if (!Zinc_expect_ptr(test, cseq_param1, "ptr cseq_param1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq_param1->type, Ake_ast_type_id, "id cseq_param1");
    Zinc_expect_string(test, &cseq_param1->value, "y", "y cseq_param1");

    Ake_ast* cseq_param2 = Ast_node_get(cseq, 2);
    if (!Zinc_expect_ptr(test, cseq_param2, "ptr cseq_param2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq_param2->type, Ake_ast_type_number, "cseq_param2");
    Zinc_expect_string(test, &cseq_param2->value, "1", "1 cseq_param2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call_missing_arguments(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(a: Int32, b: Int32) end; foo(1)", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_source_error(test, &cu.errors, "not enough arguments in function call");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call_too_many_arguments(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(a: Int32) end; foo(1, 2)", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_source_error(test, &cu.errors, "too many arguments in function call");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call_type_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(a: Int32) end; foo(true)", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_source_error(test, &cu.errors, "parameter and aguments types do not match");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call_anonymous_function_type_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const foo: fn (a: Int32) = fn (a: Int32) end; foo(true)", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_source_error(test, &cu.errors, "parameter and aguments types do not match");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call_error_right_paren(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; foo(", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_source_error(test, &cu.errors, "expected right parenthesis");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call_error_function_not_declared(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("foo()", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_source_error(test, &cu.errors, "variable not declared: foo");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call_error_not_function(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const foo: Int32; foo()", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_source_error(test, &cu.errors, "not a function type");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call_error_not_enough_arguments(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(x: Int32) end; foo()", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_source_error(test, &cu.errors, "not enough arguments in function call");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call_error_too_many_arguments(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo() end; foo(1)", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_source_error(test, &cu.errors, "too many arguments in function call");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_call_error_expected_expression(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(x: Int32) end; foo(1,)", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_source_error(test, &cu.errors, "expected expression after comma");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor_newline_anonymous_function(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn\n(\na: Int32,\nb: Int32,\nc: Int32\n)->\nInt32\na+b+c\nend", &cu);
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_no_errors(test, &cu.errors);

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor_newline_anonymous_function_const(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup(
            "const foo: fn (a: Int32, b: Int32, c: Int32)->Int32 = fn\n"
            "(\n"
            "a: Int32,\n"
            "b: Int32,\n"
            "c: Int32\n"
            ")->\n"
            "Int32\n"
            "a+b+c\n"
            "end", &cu);
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_no_errors(test, &cu.errors);

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_use_fn(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: Function = fn(x: Int32) end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, 
            &cu.errors,
            "can not directly use Function to declare a function; use fn syntax to declare a function");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_require_params_name(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: fn(Int32) = fn(Int32: Int32) end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "identifier reserved as a type: Int32");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_input_type_non_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: fn(Int32) = fn(x: Bool) end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "values in assignment are not compatible");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_output_type_non_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: fn()->Int32 = fn()->Bool true end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "values in assignment are not compatible");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_input_type_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: fn(Int64) = fn(x: Int32) end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "values in assignment are not compatible");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_output_type_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: fn()->Int64 = fn()->Int32 1 end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "values in assignment are not compatible");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeUnit_parse_function_no_inputs_no_outputs);
        Zinc_test_register(test, AkeUnit_parse_function_input);
        Zinc_test_register(test, AkeUnit_parse_function_multiple_inputs);
        Zinc_test_register(test, AkeUnit_parse_function_three_inputs);
        Zinc_test_register(test, AkeUnit_parse_function_error_duplicate_variable_declarations);
        Zinc_test_register(test, AkeUnit_parse_function_return_type_error);
        Zinc_test_register(test, AkeUnit_parse_function_error_expected_left_parenthesis);
        Zinc_test_register(test, AkeUnit_parse_function_error_expected_right_parenthesis);
        Zinc_test_register(test, AkeUnit_parse_function_error_duplicate_declaration);
        Zinc_test_register(test, AkeUnit_parse_function_error_identifier_reserved);
        Zinc_test_register(test, AkeUnit_parse_function_error_expected_end);
        Zinc_test_register(test, AkeUnit_parse_return);
        Zinc_test_register(test, AkeUnit_parse_return_error_no_value);
        Zinc_test_register(test, AkeUnit_parse_return_error_outside_of_function);
        Zinc_test_register(test, AkeUnit_parse_return_error_type_does_not_match);
        Zinc_test_register(test, AkeUnit_parse_stmts_newline_function);
        Zinc_test_register(test, AkeUnit_parse_anonymous_function);
        Zinc_test_register(test, AkeUnit_parse_anonymous_function2);
        Zinc_test_register(test, AkeUnit_parse_anonymous_function3);
        Zinc_test_register(test, AkeUnit_parse_anonymous_function_return_error);
        Zinc_test_register(test, AkeUnit_parse_anonymous_function_assignment_error);
        Zinc_test_register(test, AkeUnit_parse_anonymous_function_expected_right_paren);
        Zinc_test_register(test, AkeUnit_parse_anonymous_function_expected_end);
        Zinc_test_register(test, AkeUnit_parse_function_proto);
        Zinc_test_register(test, AkeUnit_parse_function_error_let_assign);
        Zinc_test_register(test, AkeUnit_parse_function_error_assign);
        Zinc_test_register(test, AkeUnit_parse_call);
        Zinc_test_register(test, AkeUnit_parse_call_return_type);
        Zinc_test_register(test, AkeUnit_parse_call_return_type_error);
        Zinc_test_register(test, AkeUnit_parse_call2);
        Zinc_test_register(test, AkeUnit_parse_call3);
        Zinc_test_register(test, AkeUnit_parse_call4);
        Zinc_test_register(test, AkeUnit_parse_call_missing_arguments);
        Zinc_test_register(test, AkeUnit_parse_call_too_many_arguments);
        Zinc_test_register(test, AkeUnit_parse_call_type_error);
        Zinc_test_register(test, AkeUnit_parse_call_anonymous_function_type_error);
        Zinc_test_register(test, AkeUnit_parse_call_error_right_paren);
        Zinc_test_register(test, AkeUnit_parse_call_error_function_not_declared);
        Zinc_test_register(test, AkeUnit_parse_call_error_not_function);
        Zinc_test_register(test, AkeUnit_parse_call_error_not_enough_arguments);
        Zinc_test_register(test, AkeUnit_parse_call_error_too_many_arguments);
        Zinc_test_register(test, AkeUnit_parse_call_error_expected_expression);
        Zinc_test_register(test, AkeUnit_parse_factor_newline_anonymous_function);
        Zinc_test_register(test, AkeUnit_parse_factor_newline_anonymous_function_const);
        Zinc_test_register(test, AkeUnit_parse_function_error_use_fn);
        Zinc_test_register(test, AkeUnit_parse_function_error_require_params_name);
        Zinc_test_register(test, AkeUnit_parse_function_error_input_type_non_numeric);
        Zinc_test_register(test, AkeUnit_parse_function_error_output_type_non_numeric);
        Zinc_test_register(test, AkeUnit_parse_function_error_input_type_numeric);
        Zinc_test_register(test, AkeUnit_parse_function_error_output_type_numeric);

        return;
    }

    Zinc_test_perform(test);
}