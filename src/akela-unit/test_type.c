#include "akela/type_def.h"
#include "zinc/unit_test.h"
#include "akela/ast.h"

void test_type_use1()
{
	Zinc_test_name(__func__);

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

	Zinc_assert_ptr(n, "ptr n");

	Ake_ast* n0 = Ast_node_get(n, 0);
	Zinc_assert_ptr(n0, "ptr a");
	Zinc_expect_ptr_equal(n0, a, "a");

	Ake_ast* n1 = Ast_node_get(n, 1);
	Zinc_assert_ptr(n1, "ptr b");
	Zinc_expect_ptr_equal(n1, b, "b");

    Ake_ast_destroy(n);
}


void CentUnit_type()
{
	test_type_use1();
}
