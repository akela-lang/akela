#include "akela/ast.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_ast1(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Ast* n;
    Ake_AstCreate(&n);
	n->kind = Ake_ast_type_plus;

	Ake_Ast* a;
    Ake_AstCreate(&a);
	a->kind = Ake_ast_type_number;
	Zinc_string_add_str(&a->number_value, "a");
    Ake_AstAdd(n, a);

	Ake_Ast* b;
    Ake_AstCreate(&b);
	b->kind = Ake_ast_type_number;
	Zinc_string_add_str(&b->number_value, "b");
    Ake_AstAdd(n, b);

	if (!Zinc_expect_ptr(test, n, "ptr n")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, n->kind, Ake_ast_type_plus, "plus");

	a = Ake_AstGet(n, 0);
	if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a->kind, Ake_ast_type_number, "number");
	Zinc_expect_string(test, &a->number_value, "a", "a");

	b = Ake_AstGet(n, 1);
	if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, b->kind, Ake_ast_type_number, "number");
	Zinc_expect_string(test, &b->number_value, "b", "b");

    Ake_AstDestroy(n);
}


void AkeUnit_ast(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, AkeUnit_ast1);

		return;
	}

	Zinc_test_perform(test);
}
