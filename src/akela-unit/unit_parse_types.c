#include "unit_parse.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_types_error_param_no_value(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(a: Int32) true end; foo(foo(1))", &cu);
	Zinc_expect_has_errors(test, &cu.errors);
	Zinc_expect_source_error(test, &cu.errors, "argument expression has no value");
	Zinc_expect_false(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types_newline_declaration(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a\n: \nInt32", &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    Ake_Ast* const_ = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, const_, "ptr let")) {
	    return Zinc_assert();
    }
    Zinc_expect_int_equal(test, const_->kind, Ake_ast_type_const, "type let");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_types(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);

		Zinc_test_register(test, AkeUnit_parse_types_error_param_no_value);
		Zinc_test_register(test, AkeUnit_parse_types_newline_declaration);

		return;
	}

	Zinc_test_perform(test);
}
