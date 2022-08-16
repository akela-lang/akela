#include "zinc/unit_test.h"
#include "alba/ast.h"

void test_ast1()
{
	test_name(__func__);

	struct ast_node* n;
	ast_create_node(&n);
	n->type = ast_type_plus;

	struct ast_node* a;
	ast_create_node(&a);
	a->type = ast_type_number;
	buffer_copy_str(&a->value, "a");
	ast_add_child(n, a);

	struct ast_node* b;
	ast_create_node(&b);
	b->type = ast_type_number;
	buffer_copy_str(&b->value, "b");
	ast_add_child(n, b);

	assert_ptr(n, "ptr n");
	expect_int_equal(n->type, ast_type_plus, "plus");

	a = ast_get_child(n, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_number, "number");
	expect_str(&a->value, "a", "a");

	b = ast_get_child(n, 1);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_number, "number");
	expect_str(&b->value, "b", "b");

	ast_destroy(n);

}


void test_ast()
{
	test_ast1();
}
