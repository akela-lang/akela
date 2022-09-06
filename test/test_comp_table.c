#include "zinc/unit_test.h"
#include "alba/comp_unit.h"
#include "zinc/memory.h"
#include "alba/input.h"
#include "alba/comp_table.h"
#include "zinc/buffer.h"

void test_comp_table_compile()
{
	test_name(__func__);

	struct comp_table ct;
	comp_table_init(&ct);

	struct buffer bf;
	buffer_init(&bf);
	buffer_copy_str(&bf, "10");
	struct string_data sd;
	string_data_init(&bf, &sd);

	struct comp_unit* cu = NULL;
	malloc_safe(&cu, sizeof(struct comp_unit));
	comp_unit_init(cu);
	array2buffer("|main|", &cu->path);

	comp_table_put(&ct, &cu->path, cu);

	bool valid = comp_unit_compile(cu, string_getchar, &sd);
	expect_true(valid, "valid");

	struct ast_node* root = cu->root;
	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, ast_type_stmts, "stmts root");

	struct ast_node* number = ast_node_get(root, 0);
	assert_ptr(number, "ptr number");
	expect_int_equal(number->type, ast_type_number, "number number");
	expect_str(&number->value, "10", "10 number");

	buffer_destroy(&bf);
	comp_table_destroy(&ct);
}

void test_comp_table()
{
	test_comp_table_compile();
}
