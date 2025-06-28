#include "akela/comp_unit.h"
#include "zinc/memory.h"
#include "zinc/input_unicode_string.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_comp_unit_compile(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Zinc_vector* text = NULL;
    Zinc_vector_create(&text, sizeof(char));
    Zinc_vector_add(text, "10", 2);

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, text);

	struct Ake_comp_unit* cu = NULL;
	Zinc_malloc_safe((void**)&cu, sizeof(struct Ake_comp_unit));
	Ake_comp_unit_init(cu);

	bool valid = Ake_comp_unit_compile(cu, input, input->vtable);
	Zinc_expect_true(test, valid, "valid");
	
	Ake_Ast* root = cu->root;
	if (!Zinc_expect_ptr(test, root, "ptr root")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, root->kind, Ake_ast_type_stmts, "parse_stmts root");

	Ake_Ast* number = Ast_node_get(root, 0);
	if (!Zinc_expect_ptr(test, number, "ptr number")) {
		return Zinc_assert();
	}
	Zinc_expect_int_equal(test, number->kind, Ake_ast_type_number, "number number");
	Zinc_expect_string(test, &number->value, "10", "10 number");

    free(input);
	Ake_comp_unit_destroy(cu);
	free(cu);
    Zinc_vector_destroy(text);
    free(text);
}

void AkeUnit_comp_unit(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, AkeUnit_comp_unit_compile);

		return;
	}

	Zinc_test_perform(test);
}