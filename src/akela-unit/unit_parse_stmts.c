#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "unit_parse.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

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

		Zinc_test_register(test, AkeUnit_parse_stmts_mut);
		Zinc_test_register(test, AkeUnit_parse_stmts_error_mut);

		return;
	}

	Zinc_test_perform(test);
}
