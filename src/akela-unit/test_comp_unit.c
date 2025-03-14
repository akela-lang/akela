#include "zinc/unit_test.h"
#include "akela/comp_unit.h"
#include "zinc/memory.h"
#include "zinc/input_unicode_string.h"

void test_comp_unit_compile()
{
	Zinc_test_name(__func__);

    Zinc_vector* text = NULL;
    Zinc_vector_create(&text, sizeof(char));
    Zinc_vector_add(text, "10", 2);

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, text);

	struct Ake_comp_unit* cu = NULL;
	Zinc_malloc_safe((void**)&cu, sizeof(struct Ake_comp_unit));
	Ake_comp_unit_init(cu);

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
	Ake_comp_unit_destroy(cu);
	free(cu);
    Zinc_vector_destroy(text);
    free(text);
}

void test_comp_unit()
{
	test_comp_unit_compile();
}
