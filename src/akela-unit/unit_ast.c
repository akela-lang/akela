#include "akela/ast.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_AstListAdd(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		return;
	}

	Ake_AstList list;
	Ake_AstListInit(&list, NULL);

	Ake_Ast* n0 = NULL;
	Ake_AstCreate(&n0);
	Ake_AstListAdd(&list, n0);

	Ake_Ast* n1 = NULL;
	Ake_AstCreate(&n1);
	Ake_AstListAdd(&list, n1);

	Ake_Ast* n2 = NULL;
	Ake_AstCreate(&n2);
	Ake_AstListAdd(&list, n2);

	Zinc_expect_ptr_equal(test, list.head, n0, "head list");
	Zinc_expect_ptr_equal(test, n0->prev, NULL, "prev n0");
	Zinc_expect_ptr_equal(test, n0->next, n1, "next n0");
	Zinc_expect_ptr_equal(test, n1->prev, n0, "prev n1");
	Zinc_expect_ptr_equal(test, n1->next, n2, "next n1");
	Zinc_expect_ptr_equal(test, n2->prev, n1, "prev n2");
	Zinc_expect_ptr_equal(test, n2->next, NULL, "next n2");
	Zinc_expect_ptr_equal(test, list.tail, n2, "tail list");

	Ake_AstListDestroy(&list);
}

void AkeUnit_Ast(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, AkeUnit_AstListAdd);

		return;
	}

	Zinc_test_perform(test);
}
