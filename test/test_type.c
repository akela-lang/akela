#include "alba/type_def.h"
#include "zinc/unit_test.h"
#include "alba/ast.h"

void test_type_use1()
{
	test_name(__func__);

	struct ast_node* n;
	ast_node_create(&n);
	n->type = ast_type_type;

	struct ast_node* a;
	ast_node_create(&a);
	a->type = ast_type_type;
	ast_node_add(n, a);

	struct ast_node* b;
	ast_node_create(&b);
	b->type = ast_type_type;
	ast_node_add(n, b);

	assert_ptr(n, "ptr n");

	struct ast_node* n0 = ast_node_get(n, 0);
	assert_ptr(n0, "ptr a");
	expect_ptr_equal(n0, a, "a");

	struct ast_node* n1 = ast_node_get(n, 1);
	assert_ptr(n1, "ptr b");
	expect_ptr_equal(n1, b, "b");

	ast_node_destroy(n);
}


void test_type()
{
	test_type_use1();
}
