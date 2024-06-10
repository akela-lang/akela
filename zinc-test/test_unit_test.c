#include "zinc/unit_test.h"
#include "zinc/result.h"

void test_assert_set_error()
{
	test_name(__func__);

	size_t x = 5;
	set_error("%zu", x);

	expect_strcmp(error_message, "5", "5");
}

void test_assert()
{
	test_assert_set_error();
}
