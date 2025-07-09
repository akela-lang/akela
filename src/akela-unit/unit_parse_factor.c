#include <stdbool.h>
#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "unit_parse.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_factor_array_element_const(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: [4 const]Int32 = [1, 2, 3, 4]\n"
                "a[0]\n",
                &cu);
    Zinc_expect_true(test, cu.valid, "valid");
    Zinc_expect_no_errors(test, &cu.errors);

    Ake_Ast* let = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, let, "ptr let")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, let->kind, Ake_ast_type_const, "type let");

    Ake_Ast* let_type_node = Ake_ast_get(let, 1);
    if (!Zinc_expect_ptr(test, let_type_node, "ptr type")) {
	    return Zinc_assert();
    }
	Zinc_expect_int_equal(test, let_type_node->kind, Ake_ast_type_type, "type type");

	Ake_Type* let_type = let_type_node->type;
	if (!Zinc_expect_ptr(test, let_type, "ptr type")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, let_type->kind, AKE_TYPE_ARRAY, "is_array type");
	Zinc_expect_true(test, let_type->data.array.is_const, "is_const let_type");
    Zinc_expect_size_t_equal(test, let_type->data.array.dim, 4, "size let_type_dim");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor_array_element_const_error(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: [4 const]Int32 = [1, 2, 3, 4]\n"
                "a[0] = 10\n",
                &cu);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_source_error(test, &cu.errors, "immutable variable changed in assignment");
    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, AkeUnit_parse_factor_array_element_const);
		Zinc_test_register(test, AkeUnit_parse_factor_array_element_const_error);

		return;
	}

	Zinc_test_perform(test);
}
