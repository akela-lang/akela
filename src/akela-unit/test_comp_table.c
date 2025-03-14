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
	Zinc_test_name(__func__);

	struct Ake_comp_table ct;
	Ake_comp_table_init(&ct);

    Zinc_vector* text = NULL;
    Zinc_vector_create(&text, sizeof(char));
    Zinc_vector_add(text, "10", 2);

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, text);

	struct Ake_comp_unit* cu = NULL;
	Zinc_malloc_safe((void**)&cu, sizeof(struct Ake_comp_unit));
	Ake_comp_unit_init(cu);
	Zinc_string_add_str2("|main|", &cu->path);

	Ake_comp_table_put(&ct, &cu->path, cu);

	bool valid = Ake_comp_unit_compile(cu, input, input->vtable);
	Zinc_expect_true(valid, "valid");

	Ake_ast* root = cu->root;
	Zinc_assert_ptr(root, "ptr root");
	Zinc_expect_int_equal(root->type, Ake_ast_type_stmts, "parse_stmts root");

	Ake_ast* number = Ast_node_get(root, 0);
	Zinc_assert_ptr(number, "ptr number");
	Zinc_expect_int_equal(number->type, Ake_ast_type_number, "number number");
	Zinc_expect_string(&number->value, "10", "10 number");

    free(input);
	Ake_comp_table_destroy(&ct);
    Zinc_vector_destroy(text);
    free(text);
}

void test_comp_table_include()
{
	Zinc_test_name(__func__);

	struct Ake_comp_table ct;
	Ake_comp_table_init(&ct);

	/* base */
    Zinc_vector* base_text = NULL;
    Zinc_vector_create(&base_text, sizeof(char));
    char base_string[] = "fn sqrt(x: Int64)->Int64 1 end";
    Zinc_vector_add(base_text, base_string, strlen(base_string));
    Zinc_vector_add_null(base_text);
    Zinc_input_unicode_string* base_input = NULL;
    Zinc_input_unicode_string_create(&base_input, base_text);

	struct Ake_comp_unit* cu_base = NULL;
	Zinc_malloc_safe((void**)&cu_base, sizeof(struct Ake_comp_unit));
	Ake_comp_unit_init(cu_base);
	Zinc_string_add_str2("|base|", &cu_base->path);
	Ake_comp_table_put(&ct, &cu_base->path, cu_base);

	bool valid_base = Ake_comp_unit_compile(cu_base, base_input, base_input->vtable);

	/* test base */
	Zinc_expect_no_errors(&cu_base->el);
	Zinc_expect_true(valid_base, "valid_base");

	/* main */
    char main_string[] = "sqrt(25)";
    Zinc_vector* main_vector = NULL;
    Zinc_vector_create(&main_vector, sizeof(char));
    Zinc_vector_add(main_vector, main_string, strlen(main_string));
    Zinc_vector_add_null(main_vector);
    Zinc_input_unicode_string* main_input = NULL;
    Zinc_input_unicode_string_create(&main_input, main_vector);

	struct Ake_comp_unit* cu_main = NULL;
	Zinc_malloc_safe((void**)&cu_main, sizeof(struct Ake_comp_unit));
	Ake_comp_unit_init(cu_main);
	Zinc_string_add_str2("|main|", &cu_main->path);
	Ake_comp_table_put(&ct, &cu_main->path, cu_main);

	Ake_transfer_global_symbols(&cu_base->st, &cu_main->st);

	bool valid_main = Ake_comp_unit_compile(cu_main, main_input, main_input->vtable);

	/* test main */
	Zinc_expect_no_errors(&cu_main->el);
	Zinc_expect_true(valid_main, "valid valid_main");
	Ake_ast* root_main = cu_main->root;
	Zinc_assert_ptr(root_main, "ptr root");
	Zinc_expect_int_equal(root_main->type, Ake_ast_type_stmts, "parse_stmts root_main");

	Ake_ast* call_main = Ast_node_get(root_main, 0);
	Zinc_assert_ptr(call_main, "ptr call_main");
	Zinc_expect_int_equal(call_main->type, Ake_ast_type_call, "call call_main");

	Ake_ast* id_main = Ast_node_get(call_main, 0);
	Zinc_assert_ptr(id_main, "ptr id_main");
	Zinc_expect_int_equal(id_main->type, Ake_ast_type_id, "id id_main");
	Zinc_expect_string(&id_main->value, "sqrt", "sqrt id_main");

	Ake_ast* cseq_main = Ast_node_get(call_main, 1);
	Zinc_assert_ptr(cseq_main, "ptr cseq");
	Zinc_expect_int_equal(cseq_main->type, Ake_ast_type_cseq, "cseq cseq_main");

	Ake_ast* number_main = Ast_node_get(cseq_main, 0);
	Zinc_assert_ptr(number_main, "ptr number_main");
	Zinc_expect_int_equal(number_main->type, Ake_ast_type_number, "number number_main");
	Zinc_expect_string(&number_main->value, "25", "25 number_main");

	/* destroy */
    free(base_input);
    free(main_input);
    Zinc_vector_destroy(base_text);
    free(base_text);
    Zinc_vector_destroy(main_vector);
    free(main_vector);
	Ake_comp_table_destroy(&ct);
}

void test_comp_table_include_base()
{
	Zinc_test_name(__func__);

	struct Ake_comp_table ct;
	Ake_comp_table_init(&ct);

	/* main */
    char main_string[] = "sqrt(25)";
    Zinc_vector* main_vector = NULL;
    Zinc_vector_create(&main_vector, sizeof(char));
    Zinc_vector_add(main_vector, main_string, strlen(main_string));
    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, main_vector);

	struct Ake_comp_unit* cu_main = NULL;
	Zinc_malloc_safe((void**)&cu_main, sizeof(struct Ake_comp_unit));
	Ake_comp_unit_init(cu_main);
	Zinc_string_add_str2("|main|", &cu_main->path);
	Ake_comp_table_put(&ct, &cu_main->path, cu_main);

	struct Ake_comp_unit* cu_base = NULL;
	bool valid = Ake_include_base(&ct, cu_main, &cu_base);
	Zinc_assert_true(valid, "include_base valid");
	Zinc_expect_no_errors(&cu_base->el);
	Zinc_expect_true(cu_base->valid, "valid_base");

	bool valid_main = Ake_comp_unit_compile(cu_main, input, input->vtable);

	/* test main */
	Zinc_expect_no_errors(&cu_main->el);
	Zinc_expect_true(valid_main, "valid valid_main");
	Ake_ast* root_main = cu_main->root;
	Zinc_assert_ptr(root_main, "ptr root");
	Zinc_expect_int_equal(root_main->type, Ake_ast_type_stmts, "parse_stmts root_main");

	Ake_ast* call_main = Ast_node_get(root_main, 0);
	Zinc_assert_ptr(call_main, "ptr call_main");
	Zinc_expect_int_equal(call_main->type, Ake_ast_type_call, "call call_main");

	Ake_ast* id = Ast_node_get(call_main, 0);
	Zinc_assert_ptr(id, "ptr id");
	Zinc_expect_int_equal(id->type, Ake_ast_type_id, "id id");
	Zinc_expect_string(&id->value, "sqrt", "sqrt id");

	Ake_ast* cseq_main = Ast_node_get(call_main, 1);
	Zinc_assert_ptr(cseq_main, "ptr cseq");
	Zinc_expect_int_equal(cseq_main->type, Ake_ast_type_cseq, "cseq cseq_main");

	Ake_ast* number_main = Ast_node_get(cseq_main, 0);
	Zinc_assert_ptr(number_main, "ptr number_main");
	Zinc_expect_int_equal(number_main->type, Ake_ast_type_number, "number number_main");
	Zinc_expect_string(&number_main->value, "25", "25 number_main");

	/* destroy */
	Zinc_vector_destroy(main_vector);
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
