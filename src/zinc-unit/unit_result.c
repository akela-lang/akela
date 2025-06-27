#include "zinc/result.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_result_set_error(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Zinc_result r;
	r = Zinc_set_error("error x");
	if (!Zinc_expect_result(test, r, "set_error")) {
		return Zinc_assert();
	}
	Zinc_expect_error_message(test, "error x");
}

void Zinc_unit_result_set_error2(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	size_t x = 5;
	Zinc_set_error("%zu", x);

	Zinc_expect_strcmp(test, Zinc_error_message, "5", "5");
}

void Zinc_unit_result(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, Zinc_unit_result_set_error);
		Zinc_test_register(test, Zinc_unit_result_set_error2);

		return;
	}

	Zinc_test_perform(test);
}
