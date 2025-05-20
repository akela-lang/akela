#include "akela/type_def.h"
#include "akela/ast.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_type_use1(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_ast* n;
    Ake_ast_create(&n);
	n->type = Ake_ast_type_type;

	Ake_ast* a;
    Ake_ast_create(&a);
	a->type = Ake_ast_type_type;
    Ake_ast_add(n, a);

	Ake_ast* b;
    Ake_ast_create(&b);
	b->type = Ake_ast_type_type;
    Ake_ast_add(n, b);

	Zinc_test_assert_ptr(test, n, "ptr n");

	Ake_ast* n0 = Ast_node_get(n, 0);
	Zinc_test_assert_ptr(test, n0, "ptr a");
	Zinc_test_expect_ptr_equal(test, n0, a, "a");

	Ake_ast* n1 = Ast_node_get(n, 1);
	Zinc_test_assert_ptr(test, n1, "ptr b");
	Zinc_test_expect_ptr_equal(test, n1, b, "b");

    Ake_ast_destroy(n);
}


void AkeUnit_type(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, AkeUnit_type_use1);

		return;
	}

	Zinc_test_perform(test);
}
