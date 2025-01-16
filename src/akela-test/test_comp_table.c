#include "zinc/unit_test.h"
#include "akela/comp_unit.h"
#include "zinc/memory.h"
#include "akela/comp_table.h"
#include "zinc/zstring.h"
#include "akela/comp_unit.h"
#include "zinc/error_unit_test.h"
#include "zinc/input_unicode_string.h"
#include <string.h>

void test_comp_table_compile()
{
	test_name(__func__);

	struct Ake_comp_table ct;
	Ake_comp_table_init(&ct);

    Vector* text = NULL;
    VectorCreate(&text, sizeof(char));
    VectorAdd(text, "10", 2);

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, text);

	struct Ake_comp_unit* cu = NULL;
	Zinc_malloc_safe((void**)&cu, sizeof(struct Ake_comp_unit));
	Ake_comp_unit_init(cu);
	Zinc_string_add_str2("|main|", &cu->path);

	Ake_comp_table_put(&ct, &cu->path, cu);

	bool valid = Ake_comp_unit_compile(cu, input, input->input_vtable);
	expect_true(valid, "valid");

	Ake_ast* root = cu->root;
	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, Ake_ast_type_stmts, "parse_stmts root");

	Ake_ast* number = Ast_node_get(root, 0);
	assert_ptr(number, "ptr number");
	expect_int_equal(number->type, Ake_ast_type_number, "number number");
	expect_str(&number->value, "10", "10 number");

    free(input);
	Ake_comp_table_destroy(&ct);
    VectorDestroy(text);
    free(text);
}

void test_comp_table_include()
{
	test_name(__func__);

	struct Ake_comp_table ct;
	Ake_comp_table_init(&ct);

	/* base */
    Vector* base_text = NULL;
    VectorCreate(&base_text, sizeof(char));
    char base_string[] = "fn sqrt(x: i64)->i64 1 end";
    VectorAdd(base_text, base_string, strlen(base_string));
    VectorAddNull(base_text);
    Zinc_input_unicode_string* base_input = NULL;
    Zinc_input_unicode_string_create(&base_input, base_text);

	struct Ake_comp_unit* cu_base = NULL;
	Zinc_malloc_safe((void**)&cu_base, sizeof(struct Ake_comp_unit));
	Ake_comp_unit_init(cu_base);
	Zinc_string_add_str2("|base|", &cu_base->path);
	Ake_comp_table_put(&ct, &cu_base->path, cu_base);

	bool valid_base = Ake_comp_unit_compile(cu_base, base_input, base_input->input_vtable);

	/* test base */
	Zinc_expect_no_errors(&cu_base->el);
	expect_true(valid_base, "valid_base");

	/* main */
    char main_string[] = "sqrt(25)";
    Vector* main_vector = NULL;
    VectorCreate(&main_vector, sizeof(char));
    VectorAdd(main_vector, main_string, strlen(main_string));
    VectorAddNull(main_vector);
    Zinc_input_unicode_string* main_input = NULL;
    Zinc_input_unicode_string_create(&main_input, main_vector);

	struct Ake_comp_unit* cu_main = NULL;
	Zinc_malloc_safe((void**)&cu_main, sizeof(struct Ake_comp_unit));
	Ake_comp_unit_init(cu_main);
	Zinc_string_add_str2("|main|", &cu_main->path);
	Ake_comp_table_put(&ct, &cu_main->path, cu_main);

	Ake_transfer_global_symbols(&cu_base->st, &cu_main->st);

	bool valid_main = Ake_comp_unit_compile(cu_main, main_input, main_input->input_vtable);

	/* test main */
	Zinc_expect_no_errors(&cu_main->el);
	expect_true(valid_main, "valid valid_main");
	Ake_ast* root_main = cu_main->root;
	assert_ptr(root_main, "ptr root");
	expect_int_equal(root_main->type, Ake_ast_type_stmts, "parse_stmts root_main");

	Ake_ast* call_main = Ast_node_get(root_main, 0);
	assert_ptr(call_main, "ptr call_main");
	expect_int_equal(call_main->type, Ake_ast_type_call, "call call_main");

	Ake_ast* id_main = Ast_node_get(call_main, 0);
	assert_ptr(id_main, "ptr id_main");
	expect_int_equal(id_main->type, Ake_ast_type_id, "id id_main");
	expect_str(&id_main->value, "sqrt", "sqrt id_main");

	Ake_ast* cseq_main = Ast_node_get(call_main, 1);
	assert_ptr(cseq_main, "ptr cseq");
	expect_int_equal(cseq_main->type, Ake_ast_type_cseq, "cseq cseq_main");

	Ake_ast* number_main = Ast_node_get(cseq_main, 0);
	assert_ptr(number_main, "ptr number_main");
	expect_int_equal(number_main->type, Ake_ast_type_number, "number number_main");
	expect_str(&number_main->value, "25", "25 number_main");

	/* destroy */
    free(base_input);
    free(main_input);
    VectorDestroy(base_text);
    free(base_text);
    VectorDestroy(main_vector);
    free(main_vector);
	Ake_comp_table_destroy(&ct);
}

void test_comp_table_include_base()
{
	test_name(__func__);

	struct Ake_comp_table ct;
	Ake_comp_table_init(&ct);

	/* main */
    char main_string[] = "sqrt(25)";
    Vector* main_vector = NULL;
    VectorCreate(&main_vector, sizeof(char));
    VectorAdd(main_vector, main_string, strlen(main_string));
    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, main_vector);

	struct Ake_comp_unit* cu_main = NULL;
	Zinc_malloc_safe((void**)&cu_main, sizeof(struct Ake_comp_unit));
	Ake_comp_unit_init(cu_main);
	Zinc_string_add_str2("|main|", &cu_main->path);
	Ake_comp_table_put(&ct, &cu_main->path, cu_main);

	struct Ake_comp_unit* cu_base = NULL;
	bool valid = Ake_include_base(&ct, cu_main, &cu_base);
	assert_true(valid, "include_base valid");
	Zinc_expect_no_errors(&cu_base->el);
	expect_true(cu_base->valid, "valid_base");

	bool valid_main = Ake_comp_unit_compile(cu_main, input, input->input_vtable);

	/* test main */
	Zinc_expect_no_errors(&cu_main->el);
	expect_true(valid_main, "valid valid_main");
	Ake_ast* root_main = cu_main->root;
	assert_ptr(root_main, "ptr root");
	expect_int_equal(root_main->type, Ake_ast_type_stmts, "parse_stmts root_main");

	Ake_ast* call_main = Ast_node_get(root_main, 0);
	assert_ptr(call_main, "ptr call_main");
	expect_int_equal(call_main->type, Ake_ast_type_call, "call call_main");

	Ake_ast* id = Ast_node_get(call_main, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, Ake_ast_type_id, "id id");
	expect_str(&id->value, "sqrt", "sqrt id");

	Ake_ast* cseq_main = Ast_node_get(call_main, 1);
	assert_ptr(cseq_main, "ptr cseq");
	expect_int_equal(cseq_main->type, Ake_ast_type_cseq, "cseq cseq_main");

	Ake_ast* number_main = Ast_node_get(cseq_main, 0);
	assert_ptr(number_main, "ptr number_main");
	expect_int_equal(number_main->type, Ake_ast_type_number, "number number_main");
	expect_str(&number_main->value, "25", "25 number_main");

	/* destroy */
	VectorDestroy(main_vector);
    free(main_vector);
    free(input);
	Ake_comp_table_destroy(&ct);
}

void test_comp_table()
{
	test_comp_table_compile();
	test_comp_table_include();
	test_comp_table_include_base();
}
