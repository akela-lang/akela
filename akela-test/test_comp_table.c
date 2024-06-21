#include "zinc/unit_test.h"
#include "akela/comp_unit.h"
#include "zinc/memory.h"
#include "akela/comp_table.h"
#include "zinc/buffer.h"
#include "akela/comp_unit.h"
#include "zinc/error_unit_test.h"
#include "zinc/input_unicode_string.h"
#include <string.h>

void test_comp_table_compile()
{
	test_name(__func__);

	struct comp_table ct;
	comp_table_init(&ct);

    Vector* text = NULL;
    VectorCreate(&text, sizeof(char));
    VectorAdd(text, "10", 2);

    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, text);

	struct comp_unit* cu = NULL;
	malloc_safe((void**)&cu, sizeof(struct comp_unit));
	comp_unit_init(cu);
	array2buffer("|main|", &cu->path);

	comp_table_put(&ct, &cu->path, cu);

	bool valid = comp_unit_compile(cu, input, input->input_vtable);
	expect_true(valid, "valid");

	Ast_node* root = cu->root;
	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, Ast_type_stmts, "parse_stmts root");

	Ast_node* number = Ast_node_get(root, 0);
	assert_ptr(number, "ptr number");
	expect_int_equal(number->type, Ast_type_number, "number number");
	expect_str(&number->value, "10", "10 number");

    free(input);
	comp_table_destroy(&ct);
    VectorDestroy(text);
    free(text);
}

void test_comp_table_include()
{
	test_name(__func__);

	struct comp_table ct;
	comp_table_init(&ct);

	/* base */
    Vector* base_text = NULL;
    VectorCreate(&base_text, sizeof(char));
    char base_string[] = "fn sqrt(x: i64)->i64 1 end";
    VectorAdd(base_text, base_string, strlen(base_string));
    VectorAddNull(base_text);
    InputUnicodeString* base_input = NULL;
    InputUnicodeStringCreate(&base_input, base_text);

	struct comp_unit* cu_base = NULL;
	malloc_safe((void**)&cu_base, sizeof(struct comp_unit));
	comp_unit_init(cu_base);
	array2buffer("|base|", &cu_base->path);
	comp_table_put(&ct, &cu_base->path, cu_base);

	bool valid_base = comp_unit_compile(cu_base, base_input, base_input->input_vtable);

	/* test base */
	expect_no_errors(&cu_base->el);
	expect_true(valid_base, "valid_base");

	/* main */
    char main_string[] = "sqrt(25)";
    Vector* main_vector = NULL;
    VectorCreate(&main_vector, sizeof(char));
    VectorAdd(main_vector, main_string, strlen(main_string));
    VectorAddNull(main_vector);
    InputUnicodeString* main_input = NULL;
    InputUnicodeStringCreate(&main_input, main_vector);

	struct comp_unit* cu_main = NULL;
	malloc_safe((void**)&cu_main, sizeof(struct comp_unit));
	comp_unit_init(cu_main);
	array2buffer("|main|", &cu_main->path);
	comp_table_put(&ct, &cu_main->path, cu_main);

	transfer_global_symbols(&cu_base->st, &cu_main->st);

	bool valid_main = comp_unit_compile(cu_main, main_input, main_input->input_vtable);

	/* test main */
	expect_no_errors(&cu_main->el);
	expect_true(valid_main, "valid valid_main");
	Ast_node* root_main = cu_main->root;
	assert_ptr(root_main, "ptr root");
	expect_int_equal(root_main->type, Ast_type_stmts, "parse_stmts root_main");

	Ast_node* call_main = Ast_node_get(root_main, 0);
	assert_ptr(call_main, "ptr call_main");
	expect_int_equal(call_main->type, Ast_type_call, "call call_main");

	Ast_node* id_main = Ast_node_get(call_main, 0);
	assert_ptr(id_main, "ptr id_main");
	expect_int_equal(id_main->type, Ast_type_id, "id id_main");
	expect_str(&id_main->value, "sqrt", "sqrt id_main");

	Ast_node* cseq_main = Ast_node_get(call_main, 1);
	assert_ptr(cseq_main, "ptr cseq");
	expect_int_equal(cseq_main->type, Ast_type_cseq, "cseq cseq_main");

	Ast_node* number_main = Ast_node_get(cseq_main, 0);
	assert_ptr(number_main, "ptr number_main");
	expect_int_equal(number_main->type, Ast_type_number, "number number_main");
	expect_str(&number_main->value, "25", "25 number_main");

	/* destroy */
    free(base_input);
    free(main_input);
    VectorDestroy(base_text);
    free(base_text);
    VectorDestroy(main_vector);
    free(main_vector);
	comp_table_destroy(&ct);
}

void test_comp_table_include_base()
{
	test_name(__func__);

	struct comp_table ct;
	comp_table_init(&ct);

	/* main */
    char main_string[] = "sqrt(25)";
    Vector* main_vector = NULL;
    VectorCreate(&main_vector, sizeof(char));
    VectorAdd(main_vector, main_string, strlen(main_string));
    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, main_vector);

	struct comp_unit* cu_main = NULL;
	malloc_safe((void**)&cu_main, sizeof(struct comp_unit));
	comp_unit_init(cu_main);
	array2buffer("|main|", &cu_main->path);
	comp_table_put(&ct, &cu_main->path, cu_main);

	struct comp_unit* cu_base = NULL;
	bool valid = include_base(&ct, cu_main, &cu_base);
	assert_true(valid, "include_base valid");
	expect_no_errors(&cu_base->el);
	expect_true(cu_base->valid, "valid_base");

	bool valid_main = comp_unit_compile(cu_main, input, input->input_vtable);

	/* test main */
	expect_no_errors(&cu_main->el);
	expect_true(valid_main, "valid valid_main");
	Ast_node* root_main = cu_main->root;
	assert_ptr(root_main, "ptr root");
	expect_int_equal(root_main->type, Ast_type_stmts, "parse_stmts root_main");

	Ast_node* call_main = Ast_node_get(root_main, 0);
	assert_ptr(call_main, "ptr call_main");
	expect_int_equal(call_main->type, Ast_type_call, "call call_main");

	Ast_node* id = Ast_node_get(call_main, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, Ast_type_id, "id id");
	expect_str(&id->value, "sqrt", "sqrt id");

	Ast_node* cseq_main = Ast_node_get(call_main, 1);
	assert_ptr(cseq_main, "ptr cseq");
	expect_int_equal(cseq_main->type, Ast_type_cseq, "cseq cseq_main");

	Ast_node* number_main = Ast_node_get(cseq_main, 0);
	assert_ptr(number_main, "ptr number_main");
	expect_int_equal(number_main->type, Ast_type_number, "number number_main");
	expect_str(&number_main->value, "25", "25 number_main");

	/* destroy */
	VectorDestroy(main_vector);
    free(main_vector);
    free(input);
	comp_table_destroy(&ct);
}

void test_comp_table()
{
	test_comp_table_compile();
	test_comp_table_include();
	test_comp_table_include_base();
}
