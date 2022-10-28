#include "zinc/unit_test.h"
#include "alba/unit_test_compiler.h"
#include "alba/comp_unit.h"
#include "zinc/memory.h"
#include "alba/input.h"
#include "alba/comp_table.h"
#include "zinc/buffer.h"
#include "alba/comp_unit.h"

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
	malloc_safe((void**)&cu, sizeof(struct comp_unit));
	comp_unit_init(cu);
	array2buffer("|main|", &cu->path);

	comp_table_put(&ct, &cu->path, cu);

	bool valid = comp_unit_compile(cu, (input_getchar)string_getchar, (input_data)&sd);
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

void test_comp_table_include()
{
	test_name(__func__);

	struct comp_table ct;
	comp_table_init(&ct);

	/* base */
	struct buffer bf_base;
	buffer_init(&bf_base);
	buffer_copy_str(&bf_base, "function sqrt(x::Int64)::Int64 1 end");
	struct string_data sd_base;
	string_data_init(&bf_base, &sd_base);

	struct comp_unit* cu_base = NULL;
	malloc_safe((void**)&cu_base, sizeof(struct comp_unit));
	comp_unit_init(cu_base);
	array2buffer("|base|", &cu_base->path);
	comp_table_put(&ct, &cu_base->path, cu_base);

	bool valid_base = comp_unit_compile(cu_base, (input_getchar)string_getchar, (input_data)&sd_base);

	/* test base */
	expect_no_errors(&cu_base->el);
	expect_true(valid_base, "valid_base");

	/* main */
	struct buffer bf_main;
	buffer_init(&bf_main);
	buffer_copy_str(&bf_main, "sqrt(25)");
	struct string_data sd;
	string_data_init(&bf_main, &sd);

	struct comp_unit* cu_main = NULL;
	malloc_safe((void**)&cu_main, sizeof(struct comp_unit));
	comp_unit_init(cu_main);
	array2buffer("|main|", &cu_main->path);
	comp_table_put(&ct, &cu_main->path, cu_main);

	transfer_global_symbols(&cu_base->st, &cu_main->st);

	bool valid_main = comp_unit_compile(cu_main, (input_getchar)string_getchar, (input_data)&sd);

	/* test main */
	expect_no_errors(&cu_main->el);
	expect_true(valid_main, "valid valid_main");
	struct ast_node* root_main = cu_main->root;
	assert_ptr(root_main, "ptr root");
	expect_int_equal(root_main->type, ast_type_stmts, "stmts root_main");

	struct ast_node* call_main = ast_node_get(root_main, 0);
	assert_ptr(call_main, "ptr call_main");
	expect_int_equal(call_main->type, ast_type_call, "call call_main");

	struct ast_node* id_main = ast_node_get(call_main, 0);
	assert_ptr(id_main, "ptr id_main");
	expect_int_equal(id_main->type, ast_type_id, "id id_main");
	expect_str(&id_main->value, "sqrt", "sqrt id_main");

	struct ast_node* cseq_main = ast_node_get(call_main, 1);
	assert_ptr(cseq_main, "ptr cseq");
	expect_int_equal(cseq_main->type, ast_type_cseq, "cseq cseq_main");

	struct ast_node* number_main = ast_node_get(cseq_main, 0);
	assert_ptr(number_main, "ptr number_main");
	expect_int_equal(number_main->type, ast_type_number, "number number_main");
	expect_str(&number_main->value, "25", "25 number_main");

	/* destroy */
	buffer_destroy(&bf_base);
	buffer_destroy(&bf_main);
	comp_table_destroy(&ct);
}

void test_comp_table_include_base()
{
	test_name(__func__);

	struct comp_table ct;
	comp_table_init(&ct);

	/* main */
	struct buffer bf_main;
	buffer_init(&bf_main);
	buffer_copy_str(&bf_main, "sqrt(25)");
	struct string_data sd;
	string_data_init(&bf_main, &sd);

	struct comp_unit* cu_main = NULL;
	malloc_safe((void**)&cu_main, sizeof(struct comp_unit));
	comp_unit_init(cu_main);
	array2buffer("|main|", &cu_main->path);
	comp_table_put(&ct, &cu_main->path, cu_main);

	struct comp_unit* cu_base = NULL;
	bool valid = include_base(&ct, cu_main, &cu_base);
	assert_no_errors(&cu_main->el);
	assert_true(valid, "include_base valid");

	/* test base */
	expect_no_errors(&cu_base->el);
	expect_true(cu_base->valid, "valid_base");

	bool valid_main = comp_unit_compile(cu_main, (input_getchar)string_getchar, (input_data)&sd);

	/* test main */
	expect_no_errors(&cu_main->el);
	expect_true(valid_main, "valid valid_main");
	struct ast_node* root_main = cu_main->root;
	assert_ptr(root_main, "ptr root");
	expect_int_equal(root_main->type, ast_type_stmts, "stmts root_main");

	struct ast_node* call_main = ast_node_get(root_main, 0);
	assert_ptr(call_main, "ptr call_main");
	expect_int_equal(call_main->type, ast_type_call, "call call_main");

	struct ast_node* id = ast_node_get(call_main, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "sqrt", "sqrt id");

	struct ast_node* cseq_main = ast_node_get(call_main, 1);
	assert_ptr(cseq_main, "ptr cseq");
	expect_int_equal(cseq_main->type, ast_type_cseq, "cseq cseq_main");

	struct ast_node* number_main = ast_node_get(cseq_main, 0);
	assert_ptr(number_main, "ptr number_main");
	expect_int_equal(number_main->type, ast_type_number, "number number_main");
	expect_str(&number_main->value, "25", "25 number_main");

	/* destroy */
	buffer_destroy(&bf_main);
	comp_table_destroy(&ct);
}

void test_comp_table()
{
	test_comp_table_compile();
	test_comp_table_include();
	test_comp_table_include_base();
}
