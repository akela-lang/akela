#include "zinc/unit_test.h"
#include "akela/comp_unit.h"
#include "zinc/memory.h"
#include "zinc/input_unicode_string.h"

void test_comp_unit_compile()
{
	test_name(__func__);

    Vector* text = NULL;
    VectorCreate(&text, sizeof(char));
    VectorAdd(text, "10", 2);

    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, text);

	struct comp_unit* cu = NULL;
	malloc_safe((void**)&cu, sizeof(struct comp_unit));
	comp_unit_init(cu);

	bool valid = comp_unit_compile(cu, input, input->input_vtable);
	expect_true(valid, "valid");
	
	Ake_ast* root = cu->root;
	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, Ake_ast_type_stmts, "parse_stmts root");

	Ake_ast* number = Ast_node_get(root, 0);
	assert_ptr(number, "ptr number");
	expect_int_equal(number->type, Ake_ast_type_number, "number number");
	expect_str(&number->value, "10", "10 number");

    free(input);
	comp_unit_destroy(cu);
	free(cu);
    VectorDestroy(text);
    free(text);
}

void test_comp_unit()
{
	test_comp_unit_compile();
}
