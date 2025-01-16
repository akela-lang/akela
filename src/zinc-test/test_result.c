#include "zinc/unit_test.h"
#include "zinc/result.h"

/* static-output */
void test_result_set_error()
{
	Zinc_test_name(__func__);

	enum Zinc_result r;
	r = Zinc_set_error("error x");
	Zinc_assert_error(r, "set_error");
	Zinc_expect_error_message("error x");
}

/* static-output */
void test_result()
{
	test_result_set_error();
}
