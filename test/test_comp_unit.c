#include "zinc/unit_test.h"
#include "akela/comp_unit.h"
#include "zinc/memory.h"
#include "akela/input.h"

void test_comp_unit_compile()
{
	test_name(__func__);

	struct buffer bf;
	buffer_init(&bf);
	buffer_copy_str(&bf, "10");
	struct string_data sd;
	string_data_init(&bf, &sd);

	struct comp_unit* cu = NULL;
	malloc_safe((void**)&cu, sizeof(struct comp_unit));
	comp_unit_init(cu);

	bool valid = comp_unit_compile(cu, (input_getchar)string_getchar, (input_data)&sd);
	expect_true(valid, "valid");
	
	struct ast_node* root = cu->root;
	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "parse_stmts root");

	struct ast_node* number = ast_node_get(root, 0);
	assert_ptr(number, "ptr number");
	expect_int_equal(number->type, ast_type_number, "number number");
	expect_str(&number->value, "10", "10 number");

	comp_unit_destroy(cu);
	free(cu);
	buffer_destroy(&bf);
}

void test_comp_unit()
{
	test_comp_unit_compile();
}
