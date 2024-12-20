#include "zinc/unit_test.h"
#include "akela/ast.h"

void test_ast1()
{
	test_name(__func__);

	Cob_ast* n;
    Cob_ast_create(&n);
	n->type = Ast_type_plus;

	Cob_ast* a;
    Cob_ast_create(&a);
	a->type = Cob_ast_type_number;
	buffer_copy_str(&a->value, "a");
    Cob_ast_add(n, a);

	Cob_ast* b;
    Cob_ast_create(&b);
	b->type = Cob_ast_type_number;
	buffer_copy_str(&b->value, "b");
    Cob_ast_add(n, b);

	assert_ptr(n, "ptr n");
	expect_int_equal(n->type, Ast_type_plus, "plus");

	a = Ast_node_get(n, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Cob_ast_type_number, "number");
	expect_str(&a->value, "a", "a");

	b = Ast_node_get(n, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, Cob_ast_type_number, "number");
	expect_str(&b->value, "b", "b");

    Cob_ast_destroy(n);
}


void test_ast()
{
	test_ast1();
}
