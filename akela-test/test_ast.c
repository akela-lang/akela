#include "zinc/unit_test.h"
#include "akela/ast.h"

void test_ast1()
{
	test_name(__func__);

	Ast_node* n;
    Ast_node_create(&n);
	n->type = Ast_type_plus;

	Ast_node* a;
    Ast_node_create(&a);
	a->type = Ast_type_number;
	buffer_copy_str(&a->value, "a");
    Ast_node_add(n, a);

	Ast_node* b;
    Ast_node_create(&b);
	b->type = Ast_type_number;
	buffer_copy_str(&b->value, "b");
    Ast_node_add(n, b);

	assert_ptr(n, "ptr n");
	expect_int_equal(n->type, Ast_type_plus, "plus");

	a = Ast_node_get(n, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ast_type_number, "number");
	expect_str(&a->value, "a", "a");

	b = Ast_node_get(n, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, Ast_type_number, "number");
	expect_str(&b->value, "b", "b");

    Ast_node_destroy(n);
}


void test_ast()
{
	test_ast1();
}
