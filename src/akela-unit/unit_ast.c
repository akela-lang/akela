#include "akela/ast.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_AstAdd(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		return;
	}

	Ake_Ast* n;
    Ake_AstCreate(&n);
	n->kind = Ake_ast_type_plus;

	Ake_Ast* a;
    Ake_AstCreate(&a);
	Ake_AstSet(a, AKE_AST_NUMBER);
	Zinc_string_add_str(&a->data.number.value, "a");
    Ake_AstAdd(n, a);

	Ake_Ast* b;
    Ake_AstCreate(&b);
	Ake_AstSet(b, AKE_AST_NUMBER);
	Zinc_string_add_str(&b->data.number.value, "b");
    Ake_AstAdd(n, b);

	if (!Zinc_expect_ptr(test, n, "ptr n")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, n->kind, Ake_ast_type_plus, "plus");

	a = Ake_AstGet(n, 0);
	if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, a->kind, AKE_AST_NUMBER, "number");
	Zinc_expect_string(test, &a->data.number.value, "a", "a");

	b = Ake_AstGet(n, 1);
	if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, b->kind, AKE_AST_NUMBER, "number");
	Zinc_expect_string(test, &b->data.number.value, "b", "b");

    Ake_AstDestroy(n);
}

void AkeUnit_AstListAdd(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		return;
	}

	Ake_AstList list;
	Ake_AstListInit(&list);

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

		Zinc_test_register(test, AkeUnit_AstAdd);
		Zinc_test_register(test, AkeUnit_AstListAdd);

		return;
	}

	Zinc_test_perform(test);
}
