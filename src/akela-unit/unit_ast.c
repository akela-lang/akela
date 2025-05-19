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

	Ake_ast* n;
    Ake_ast_create(&n);
	n->type = Ake_ast_type_plus;

	Ake_ast* a;
    Ake_ast_create(&a);
	a->type = Ake_ast_type_number;
	Zinc_string_add_str(&a->value, "a");
    Ake_ast_add(n, a);

	Ake_ast* b;
    Ake_ast_create(&b);
	b->type = Ake_ast_type_number;
	Zinc_string_add_str(&b->value, "b");
    Ake_ast_add(n, b);

	Zinc_test_assert_ptr(test, n, "ptr n");
	Zinc_test_expect_int_equal(test, n->type, Ake_ast_type_plus, "plus");

	a = Ast_node_get(n, 0);
	Zinc_test_assert_ptr(test, a, "ptr a");
	Zinc_test_expect_int_equal(test, a->type, Ake_ast_type_number, "number");
	Zinc_test_expect_string(test, &a->value, "a", "a");

	b = Ast_node_get(n, 1);
	Zinc_test_assert_ptr(test, b, "ptr b");
	Zinc_test_expect_int_equal(test, b->type, Ake_ast_type_number, "number");
	Zinc_test_expect_string(test, &b->value, "b", "b");

    Ake_ast_destroy(n);
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
