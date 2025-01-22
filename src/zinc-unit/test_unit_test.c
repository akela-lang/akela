#include "zinc/unit_test.h"
#include "zinc/result.h"

void test_assert_set_error()
{
	Zinc_test_name(__func__);

	size_t x = 5;
	Zinc_set_error("%zu", x);

	Zinc_expect_strcmp(Zinc_error_message, "5", "5");
}

void test_assert()
{
	test_assert_set_error();
}
