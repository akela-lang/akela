#include "alba/type_info.h"
#include "zinc/unit_test.h"

void test_type_node1()
{
	test_name(__func__);

	struct type_node* n;
	type_node_create(&n);

	struct type_node* a;
	type_node_create(&a);
	type_node_add(n, a);

	struct type_node* b;
	type_node_create(&b);
	type_node_add(n, b);

	assert_ptr(n, "ptr n");

	struct type_node* n0 = type_node_get(n, 0);
	assert_ptr(n0, "ptr a");
	expect_ptr_equal(n0, a, "a");

	struct type_node* n1 = type_node_get(n, 1);
	assert_ptr(n1, "ptr b");
	expect_ptr_equal(n1, b, "b");

	type_node_destroy(n);
}


void test_type()
{
	test_type_node1();
}
