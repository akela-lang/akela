#include "akela/type_def.h"
#include "zinc/unit_test.h"
#include "akela/ast.h"

void test_type_use1()
{
	test_name(__func__);

	Ast_node* n;
    Ast_node_create(&n);
	n->type = Ast_type_type;

	Ast_node* a;
    Ast_node_create(&a);
	a->type = Ast_type_type;
    Ast_node_add(n, a);

	Ast_node* b;
    Ast_node_create(&b);
	b->type = Ast_type_type;
    Ast_node_add(n, b);

	assert_ptr(n, "ptr n");

	Ast_node* n0 = Ast_node_get(n, 0);
	assert_ptr(n0, "ptr a");
	expect_ptr_equal(n0, a, "a");

	Ast_node* n1 = Ast_node_get(n, 1);
	assert_ptr(n1, "ptr b");
	expect_ptr_equal(n1, b, "b");

    Ast_node_destroy(n);
}


void test_type()
{
	test_type_use1();
}
