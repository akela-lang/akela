#include "akela/parse_tools.h"
#include "unit_parse.h"
#include "zinc/test.h"
#include "zinc/expect.h"

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
    Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_error, "stmts cu.root");

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
    Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_error, "stmts cu.root");

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
    Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

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
    Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_error, "stmts cu.root");

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

    AkeUnit_parse_setup("var a: fn(Int32, Int32, Int32)\n"
        "a = fn (x: Int32,y: Int32,z: Int32)\n"
        "    1\n"
        "  end\n",
        &cu
    );
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "parse valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_Ast* assign = Ake_ast_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, assign, "ptr assign")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign->kind, Ake_ast_type_assign, "assign assign");

    Ake_Ast* a = Ake_ast_get(assign, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->kind, Ake_ast_type_id, "id a");
    Zinc_expect_string(test, &a->value, "a", "a a");

    Ake_Ast* f = Ake_ast_get(assign, 1);
    if (!Zinc_expect_ptr(test, f, "ptr f")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f->kind, Ake_ast_type_function, "type f");

    Ake_Ast* proto = Ake_ast_get(f, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->kind, Ake_ast_type_prototype, "type proto");

    Ake_Ast* func_id = Ake_ast_get(proto, 0);
    if (!Zinc_expect_ptr(test, func_id, "ptr func_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, func_id->kind, Ake_ast_type_id, "type func_id");
    Zinc_expect_string(test, &func_id->value, "__anonymous_function_0", "value func_id");

    Ake_Ast* dseq = Ake_ast_get(proto, 1);
    if (!Zinc_expect_ptr(test, dseq, "ptr dseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq->kind, Ake_ast_type_dseq, "dseq dseq");

    Ake_Ast* dec_x = Ake_ast_get(dseq, 0);
    if (!Zinc_expect_ptr(test, dec_x, "ptr dec_x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_x->kind, Ake_ast_type_declaration, "declaration dec_x");

    Ake_Ast* x = Ake_ast_get(dec_x, 0);
    if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, x->kind, Ake_ast_type_id, "id x");
    Zinc_expect_string(test, &x->value, "x", "x x");

    Ake_Ast* type_x = Ake_ast_get(dec_x, 1);
    if (!Zinc_expect_ptr(test, type_x, "ptr type_x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type_x->kind, Ake_ast_type_type, "type type_x");

    Ake_Ast* dec_y = Ake_ast_get(dseq, 1);
    if (!Zinc_expect_ptr(test, dec_y, "ptr dec_y")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_y->kind, Ake_ast_type_declaration, "declaration dec_y");

    Ake_Ast* y = Ake_ast_get(dec_y, 0);
    if (!Zinc_expect_ptr(test, y, "ptr y")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, y->kind, Ake_ast_type_id, "id y");
    Zinc_expect_string(test, &y->value, "y", "y y");

    Ake_Ast* type_y = Ake_ast_get(dec_y, 1);
    if (!Zinc_expect_ptr(test, type_y, "ptr type_y")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type_y->kind, Ake_ast_type_type, "type type_y");

    Ake_Ast* dec_z = Ake_ast_get(dseq, 2);
    if (!Zinc_expect_ptr(test, dec_z, "ptr dec_z")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_z->kind, Ake_ast_type_declaration, "declaration dec_z");

    Ake_Ast* z = Ake_ast_get(dec_z, 0);
    if (!Zinc_expect_ptr(test, z, "ptr z")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, z->kind, Ake_ast_type_id, "id z");
    Zinc_expect_string(test, &z->value, "z", "z z");

    Ake_Ast* type_z = Ake_ast_get(dec_z, 1);
    if (!Zinc_expect_ptr(test, type_z, "ptr type_z")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type_z->kind, Ake_ast_type_type, "type type_z");

    Ake_Ast* dret = Ake_ast_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->kind, Ake_ast_type_dret, "dret dret");

    Ake_Ast* stmts = Ake_ast_get(f, 1);
    if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, stmts->kind, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_Ast* one = Ake_ast_get(stmts, 0);
    if (!Zinc_expect_ptr(test, one, "ptr one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->kind, Ake_ast_type_number, "number one");
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
            "var a: fn (Int32, Int32, Int32)->Int32\n"
            "a = fn(x: Int32, y: Int32, z: Int32)->Int32\n"
            "  1\n"
            "end\n",
            &cu);
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "parse valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_Ast* assign = Ake_ast_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, assign, "ptr assign")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign->kind, Ake_ast_type_assign, "assign assign");

    Ake_Ast* a = Ake_ast_get(assign, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->kind, Ake_ast_type_id, "id a");
    Zinc_expect_string(test, &a->value, "a", "a a");

    Ake_Ast* f = Ake_ast_get(assign, 1);
    if (!Zinc_expect_ptr(test, f, "ptr f")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f->kind, Ake_ast_type_function, "type f");

    Ake_Ast* proto = Ake_ast_get(f, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->kind, Ake_ast_type_prototype, "type proto");

    Ake_Ast* func_id = Ake_ast_get(proto, 0);
    if (!Zinc_expect_ptr(test, func_id, "ptr func_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, func_id->kind, Ake_ast_type_id, "type func_id");
    Zinc_expect_string(test, &func_id->value, "__anonymous_function_0", "value func_id");

    Ake_Ast* dseq = Ake_ast_get(proto, 1);
    if (!Zinc_expect_ptr(test, dseq, "ptr dseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq->kind, Ake_ast_type_dseq, "dseq dseq");

    Ake_Ast* dec_x = Ake_ast_get(dseq, 0);
    if (!Zinc_expect_ptr(test, dec_x, "ptr dec_x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_x->kind, Ake_ast_type_declaration, "declaration dec_x");

    Ake_Ast* x = Ake_ast_get(dec_x, 0);
    if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, x->kind, Ake_ast_type_id, "id x");
    Zinc_expect_string(test, &x->value, "x", "x x");

    Ake_Ast* type_x = Ake_ast_get(dec_x, 1);
    if (!Zinc_expect_ptr(test, type_x, "ptr type_x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type_x->kind, Ake_ast_type_type, "type_type type_x");

    Ake_Ast* dec_y = Ake_ast_get(dseq, 1);
    if (!Zinc_expect_ptr(test, dec_y, "ptr dec_y")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_y->kind, Ake_ast_type_declaration, "declaration dec_y");

    Ake_Ast* y = Ake_ast_get(dec_y, 0);
    if (!Zinc_expect_ptr(test, y, "ptr y")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, y->kind, Ake_ast_type_id, "id y");
    Zinc_expect_string(test, &y->value, "y", "y y");

    Ake_Ast* type_y = Ake_ast_get(dec_y, 1);
    if (!Zinc_expect_ptr(test, type_y, "ptr name_y")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type_y->kind, Ake_ast_type_type, "type type_y");

    Ake_Ast* dec_z = Ake_ast_get(dseq, 2);
    if (!Zinc_expect_ptr(test, dec_z, "ptr dec_z")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dec_z->kind, Ake_ast_type_declaration, "declaration dec_z");

    Ake_Ast* z = Ake_ast_get(dec_z, 0);
    if (!Zinc_expect_ptr(test, z, "ptr z")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, z->kind, Ake_ast_type_id, "id z");
    Zinc_expect_string(test, &z->value, "z", "z z");

    Ake_Ast* type_z = Ake_ast_get(dec_z, 1);
    if (!Zinc_expect_ptr(test, type_z, "ptr type_z")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type_z->kind, Ake_ast_type_type, "type type_z");

    Ake_Ast* dret = Ake_ast_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->kind, Ake_ast_type_dret, "dret dret");

    Ake_Ast* dret_type = Ake_ast_get(dret, 0);
    if (!Zinc_expect_ptr(test, dret_type, "ptr dret_type")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret_type->kind, Ake_ast_type_type, "id dret_type");

    Ake_Ast* stmts = Ake_ast_get(f, 1);
    if (!Zinc_expect_ptr(test, stmts, "ptr parse_stmts")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, stmts->kind, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_Ast* one = Ake_ast_get(stmts, 0);
    if (!Zinc_expect_ptr(test, one, "ptr one")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, one->kind, Ake_ast_type_number, "number one");
    Zinc_expect_string(test, &one->value, "1", "1 one");

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
    AkeUnit_parse_setup("var foo: fn (a: Int32)->Int32\n"
                "foo = fn (a: Int32)->Int32\n"
                "  a + 1\n"
                "end\n",
                &cu);
    Zinc_expect_true(test, cu.valid, "valid");
    Zinc_expect_no_errors(test, &cu.errors);

    /* const */
    Ake_Ast* _const_ = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, _const_, "ptr const_")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, _const_->kind, Ake_ast_type_var, "type const_");

    Ake_Ast* id_node = Ake_ast_get(_const_, 0);
    if (!Zinc_expect_ptr(test, id_node, "ptr id_node")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, id_node->kind, Ake_ast_type_id, "type id_node");

    Ake_Ast* let_type_node = Ake_ast_get(_const_, 1);
    if (!Zinc_expect_ptr(test, let_type_node, "ptr let_type")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, let_type_node->kind, Ake_ast_type_type, "type let_type");

    Ake_Type* let_type = let_type_node->type;
    if (!Zinc_expect_ptr(test, let_type, "ptr let_tu")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, let_type->kind, AKE_TYPE_FUNCTION, "type let_td");

    /* assign */
    Ake_Ast* assign = Ake_ast_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, assign, "ptr assign")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, assign->kind, Ake_ast_type_assign, "type assign");

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

    Ake_Ast* fd = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, fd, "ptr fd")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd->kind, Ake_ast_type_function, "function fd");

    Ake_Ast* proto = Ake_ast_get(fd, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->kind, Ake_ast_type_prototype, "type proto");

    Ake_Ast* f_id = Ake_ast_get(proto, 0);
    if (!Zinc_expect_ptr(test, f_id, "ptr f_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f_id->kind, Ake_ast_type_id, "id f_id");
    Zinc_expect_string(test, &f_id->value, "foo", "foo");

    Ake_Ast* dseq = Ake_ast_get(proto, 1);
    if (!Zinc_expect_ptr(test, dseq, "ptr dseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq->kind, Ake_ast_type_dseq, "dret dseq");

    Ake_Ast* dret = Ake_ast_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->kind, Ake_ast_type_dret, "dret dret");

    Ake_Ast* a = Ake_ast_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->kind, Ake_ast_type_call, "call");

    Ake_Ast* b = Ake_ast_get(a, 0);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->kind, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &b->value, "foo", "foo");

    Ake_Ast* c = Ake_ast_get(a, 1);
    if (!Zinc_expect_ptr(test, c, "ptr c")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c->kind, Ake_ast_type_cseq, "cseq");

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
    if (!Zinc_expect_no_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_Ast* f = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, f, "ptr f")) {
		return Zinc_assert();
	}

    Ake_Ast* add = Ake_ast_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, add, "ptr add")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, add->kind, Ake_ast_type_plus, "plus add");

    Ake_Type* add_type = add->type;
    if (!Zinc_expect_ptr(test, add_type, "patr add_tu")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, add_type->kind, AKE_TYPE_INTEGER, "integer add_td");
    Zinc_expect_string(test, &add_type->name, "Int32", "Int32 add_td");

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
    if (!Zinc_expect_has_errors(test, &cu.errors)) {
		return Zinc_assert();
	}
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

    Ake_Ast* fd = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, fd, "ptr fd")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd->kind, Ake_ast_type_function, "function fd");

    Ake_Ast* proto = Ake_ast_get(fd, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->kind, Ake_ast_type_prototype, "type proto");

    Ake_Ast* fname = Ake_ast_get(proto, 0);
    if (!Zinc_expect_ptr(test, fname, "ptr fname")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fname->kind, Ake_ast_type_id, "id fname");
    Zinc_expect_string(test, &fname->value, "foo", "foo fname");

    Ake_Ast* dseq = Ake_ast_get(proto, 1);
    if (!Zinc_expect_ptr(test, dseq, "ptr dseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq->kind, Ake_ast_type_dseq, "dseq dseq");

    Ake_Ast* dparam0 = Ake_ast_get(dseq, 0);
    if (!Zinc_expect_ptr(test, dparam0, "ptr dparam0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dparam0->kind, Ake_ast_type_declaration, "declaration dparam0");

    Ake_Ast* param0_name = Ake_ast_get(dparam0, 0);
    if (!Zinc_expect_ptr(test, param0_name, "ptr param0_name")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, param0_name->kind, Ake_ast_type_id, "id param0_name");
    Zinc_expect_string(test, &param0_name->value, "arg1", "arg1");

    Ake_Ast* type_node = Ake_ast_get(dparam0, 1);
    if (!Zinc_expect_ptr(test, type_node, "ptr type_node")) {
		return Zinc_assert();
	}

    Ake_Type* type = type_node->type;
    if (!Zinc_expect_ptr(test, type, "ptr tu")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, type->kind, AKE_TYPE_INTEGER, "integer td");
    Zinc_expect_string(test, &type->name, "Int32", "Int32 td");

    Ake_Ast* dret = Ake_ast_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->kind, Ake_ast_type_dret, "dret dret");

    Ake_Ast* f_stmts = Ake_ast_get(fd, 1);
    if (!Zinc_expect_ptr(test, f_stmts, "ptr f_stmts")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, f_stmts->kind, Ake_ast_type_stmts, "parse_stmts f_stmts");

    Ake_Ast* a = Ake_ast_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->kind, Ake_ast_type_call, "call");

    Ake_Ast* b = Ake_ast_get(a, 0);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->kind, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &b->value, "foo", "foo");

    Ake_Ast* cseq = Ake_ast_get(a, 1);
    if (!Zinc_expect_ptr(test, cseq, "ptr cseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq->kind, Ake_ast_type_cseq, "cseq");

    Ake_Ast* cseq_a = Ake_ast_get(cseq, 0);
    if (!Zinc_expect_ptr(test, cseq_a, "ptr cseq_a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq_a->kind, Ake_ast_type_number, "cseq_a");
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

    Ake_Ast* fd = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, fd, "ptr fd")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd->kind, Ake_ast_type_function, "function fd");

    Ake_Ast* proto = Ake_ast_get(fd, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->kind, Ake_ast_type_prototype, "type proto");

    Ake_Ast* fname = Ake_ast_get(proto, 0);
    if (!Zinc_expect_ptr(test, fname, "ptr fname")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fname->kind, Ake_ast_type_id, "id fname");
    Zinc_expect_string(test, &fname->value, "foo", "foo fname");

    Ake_Ast* fd_seq = Ake_ast_get(proto, 1);
    if (!Zinc_expect_ptr(test, fd_seq, "ptr fdseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd_seq->kind, Ake_ast_type_dseq, "dseq fd_seq");

    Ake_Ast* fd_param0 = Ake_ast_get(fd_seq, 0);
    if (!Zinc_expect_ptr(test, fd_param0, "ptr fd_param0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd_param0->kind, Ake_ast_type_declaration, "declaration fd_param0");

    Ake_Ast* param0_name = Ake_ast_get(fd_param0, 0);
    if (!Zinc_expect_ptr(test, param0_name, "ptr param0_name")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, param0_name->kind, Ake_ast_type_id, "id param0_name");
    Zinc_expect_string(test, &param0_name->value, "arg1", "arg1");

    Ake_Ast* param0_type = Ake_ast_get(fd_param0, 1);
    if (!Zinc_expect_ptr(test, param0_type, "ptr param0_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, param0_type->kind, Ake_ast_type_type, "type param0_id");

    Ake_Ast* dret = Ake_ast_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->kind, Ake_ast_type_dret, "dret dret");

    Ake_Ast* a = Ake_ast_get(cu.root, 3);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->kind, Ake_ast_type_call, "call");

    Ake_Ast* b = Ake_ast_get(a, 0);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->kind, Ake_ast_type_id, "id");
    Zinc_expect_string(test, &b->value, "foo", "foo");

    Ake_Ast* cseq = Ake_ast_get(a, 1);
    if (!Zinc_expect_ptr(test, cseq, "ptr cseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq->kind, Ake_ast_type_cseq, "cseq");

    Ake_Ast* cseq_a = Ake_ast_get(cseq, 0);
    if (!Zinc_expect_ptr(test, cseq_a, "ptr cseq_a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq_a->kind, Ake_ast_type_id, "cseq_a");
    Zinc_expect_string(test, &cseq_a->value, "x", "x cseq_a");

    Ake_Ast* cseq_b = Ake_ast_get(cseq, 1);
    if (!Zinc_expect_ptr(test, cseq_b, "ptr cseq_b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq_b->kind, Ake_ast_type_id, "cseq_b");
    Zinc_expect_string(test, &cseq_b->value, "y", "y cseq_b");

    Ake_Ast* cseq_c = Ake_ast_get(cseq, 2);
    if (!Zinc_expect_null(test, cseq_c, "null cseq_c")) {
		return Zinc_assert();
	}

    Ake_Ast* c = Ake_ast_get(a, 2);
    if (!Zinc_expect_null(test, c, "null c")) {
		return Zinc_assert();
	}

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

    Ake_Ast* fd = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, fd, "ptr fd")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd->kind, Ake_ast_type_function, "function fd");

    Ake_Ast* proto = Ake_ast_get(fd, 0);
    if (!Zinc_expect_ptr(test, proto, "ptr proto")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, proto->kind, Ake_ast_type_prototype, "type proto");

    Ake_Ast* fd_id = Ake_ast_get(proto, 0);
    if (!Zinc_expect_ptr(test, fd_id, "ptr fd")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, fd_id->kind, Ake_ast_type_id, "id fd_id");
    Zinc_expect_string(test, &fd_id->value, "foo", "foo fd_id");

    Ake_Ast* dseq = Ake_ast_get(proto, 1);
    if (!Zinc_expect_ptr(test, dseq, "ptr dseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq->kind, Ake_ast_type_dseq, "dseq dseq");

    Ake_Ast* dseq_param0 = Ake_ast_get(dseq, 0);
    if (!Zinc_expect_ptr(test, dseq_param0, "ptr desq_param0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param0->kind, Ake_ast_type_declaration, "declaration dseq_param0");

    Ake_Ast* dseq_param0_id = Ake_ast_get(dseq_param0, 0);
    if (!Zinc_expect_ptr(test, dseq_param0_id, "ptr dseq_param0_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param0_id->kind, Ake_ast_type_id, "type dseq_param0_id");
    Zinc_expect_string(test, &dseq_param0_id->value, "arg0", "arg0 dseq_param0_id");

    Ake_Ast* dseq_param0_type = Ake_ast_get(dseq_param0, 1);
    if (!Zinc_expect_ptr(test, dseq_param0_type, "ptr dseq_param0_type")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param0_type->kind, Ake_ast_type_type, "type dseq_param0_type");

    Ake_Ast* dseq_param1 = Ake_ast_get(dseq, 1);
    if (!Zinc_expect_ptr(test, dseq_param1, "ptr desq_param1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param1->kind, Ake_ast_type_declaration, "declaration dseq_param1");

    Ake_Ast* dseq_param1_id = Ake_ast_get(dseq_param1, 0);
    if (!Zinc_expect_ptr(test, dseq_param1_id, "ptr dseq_param1_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param1_id->kind, Ake_ast_type_id, "type dseq_param1_id");
    Zinc_expect_string(test, &dseq_param1_id->value, "arg1", "arg1 dseq_param1_id");

    Ake_Ast* dseq_param1_type_id = Ake_ast_get(dseq_param1, 1);
    if (!Zinc_expect_ptr(test, dseq_param1_type_id, "ptr dseq_param1_type_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param1_type_id->kind, Ake_ast_type_type, "type dseq_param1_type_id");

    Ake_Ast* dseq_param2 = Ake_ast_get(dseq, 2);
    if (!Zinc_expect_ptr(test, dseq_param2, "ptr desq_param2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param2->kind, Ake_ast_type_declaration, "declaration dseq_param2");

    Ake_Ast* dseq_param2_id = Ake_ast_get(dseq_param2, 0);
    if (!Zinc_expect_ptr(test, dseq_param2_id, "ptr dseq_param2_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param2_id->kind, Ake_ast_type_id, "type dseq_param2_id");
    Zinc_expect_string(test, &dseq_param2_id->value, "arg2", "arg2 dseq_param2_id");

    Ake_Ast* dseq_param2_type_id = Ake_ast_get(dseq_param2, 1);
    if (!Zinc_expect_ptr(test, dseq_param2_type_id, "ptr dseq_param2_type_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dseq_param2_type_id->kind, Ake_ast_type_type, "type dseq_param2_type_id");

    Ake_Ast* dret = Ake_ast_get(proto, 2);
    if (!Zinc_expect_ptr(test, dret, "ptr dret")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret->kind, Ake_ast_type_dret, "dret dret");

    Ake_Ast* dret_type_id = Ake_ast_get(dret, 0);
    if (!Zinc_expect_ptr(test, dret_type_id, "ptr dret_type_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, dret_type_id->kind, Ake_ast_type_type, "type dret_type_id");

    Ake_Ast* call = Ake_ast_get(cu.root, 3);
    if (!Zinc_expect_ptr(test, call, "ptr call")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, call->kind, Ake_ast_type_call, "call call");

    Ake_Ast* call_id = Ake_ast_get(call, 0);
    if (!Zinc_expect_ptr(test, call_id, "ptr call_id")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, call_id->kind, Ake_ast_type_id, "id call_id");
    Zinc_expect_string(test, &call_id->value, "foo", "foo call_id");

    Ake_Ast* cseq = Ake_ast_get(call, 1);
    if (!Zinc_expect_ptr(test, cseq, "ptr cseq")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq->kind, Ake_ast_type_cseq, "cseq");

    Ake_Ast* cseq_param0 = Ake_ast_get(cseq, 0);
    if (!Zinc_expect_ptr(test, cseq_param0, "ptr cseq_param0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq_param0->kind, Ake_ast_type_id, "id cseq_param0");
    Zinc_expect_string(test, &cseq_param0->value, "x", "x cseq_param0");

    Ake_Ast* cseq_param1 = Ake_ast_get(cseq, 1);
    if (!Zinc_expect_ptr(test, cseq_param1, "ptr cseq_param1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq_param1->kind, Ake_ast_type_id, "id cseq_param1");
    Zinc_expect_string(test, &cseq_param1->value, "y", "y cseq_param1");

    Ake_Ast* cseq_param2 = Ake_ast_get(cseq, 2);
    if (!Zinc_expect_ptr(test, cseq_param2, "ptr cseq_param2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, cseq_param2->kind, Ake_ast_type_number, "cseq_param2");
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

        Zinc_test_register(test, AkeUnit_parse_function_error_expected_left_parenthesis);
        Zinc_test_register(test, AkeUnit_parse_function_error_expected_right_parenthesis);
        Zinc_test_register(test, AkeUnit_parse_function_error_expected_end);
        Zinc_test_register(test, AkeUnit_parse_return);
        Zinc_test_register(test, AkeUnit_parse_return_error_no_value);
        Zinc_test_register(test, AkeUnit_parse_return_error_outside_of_function);
        Zinc_test_register(test, AkeUnit_parse_return_error_type_does_not_match);
        Zinc_test_register(test, AkeUnit_parse_stmts_newline_function);
        Zinc_test_register(test, AkeUnit_parse_anonymous_function);
        Zinc_test_register(test, AkeUnit_parse_anonymous_function2);
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
        Zinc_test_register(test, AkeUnit_parse_function_error_input_type_non_numeric);
        Zinc_test_register(test, AkeUnit_parse_function_error_output_type_non_numeric);
        Zinc_test_register(test, AkeUnit_parse_function_error_input_type_numeric);
        Zinc_test_register(test, AkeUnit_parse_function_error_output_type_numeric);

        return;
    }

    Zinc_test_perform(test);
}