#include "zinc/assert.h"
#include "alba/dag.h"

void test_dag1()
{
	test_name(__func__);

	struct dag_node* n;
	dag_create_node(&n);
	n->type = dag_type_plus;

	struct dag_node* a;
	dag_create_node(&a);
	a->type = dag_type_number;
	buffer_copy_str(&a->value, "a");
	dag_add_child(n, a);

	struct dag_node* b;
	dag_create_node(&b);
	b->type = dag_type_number;
	buffer_copy_str(&b->value, "b");
	dag_add_child(n, b);

	assert_ptr(n, "ptr n");
	expect_int_equal(n->type, dag_type_plus, "plus");

	a = dag_get_child(n, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, dag_type_number, "number");
	expect_str(&a->value, "a", "a");

	b = dag_get_child(n, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, dag_type_number, "number");
	expect_str(&b->value, "b", "b");

	dag_destroy(n);

}


void test_dag()
{
	test_dag1();
}
