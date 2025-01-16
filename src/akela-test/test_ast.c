#include "zinc/unit_test.h"
#include "akela/ast.h"

void test_ast1()
{
	Zinc_test_name(__func__);

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

	Zinc_assert_ptr(n, "ptr n");
	Zinc_expect_int_equal(n->type, Ake_ast_type_plus, "plus");

	a = Ast_node_get(n, 0);
	Zinc_assert_ptr(a, "ptr a");
	Zinc_expect_int_equal(a->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&a->value, "a", "a");

	b = Ast_node_get(n, 1);
	Zinc_assert_ptr(b, "ptr b");
	Zinc_expect_int_equal(b->type, Ake_ast_type_number, "number");
	Zinc_expect_str(&b->value, "b", "b");

    Ake_ast_destroy(n);
}


void test_ast()
{
	test_ast1();
}
