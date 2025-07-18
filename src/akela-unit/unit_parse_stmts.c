#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "unit_parse.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

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

		Zinc_test_register(test, AkeUnit_parse_extern);
		Zinc_test_register(test, AkeUnit_parse_stmts_mut);
		Zinc_test_register(test, AkeUnit_parse_stmts_error_mut);

		return;
	}

	Zinc_test_perform(test);
}
