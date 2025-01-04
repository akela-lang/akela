#include "zinc/unit_test.h"
#include "zinc/result.h"

/* static-output */
void test_result_set_error()
{
	test_name(__func__);

	enum result r;
	r = set_error("error x");
	assert_error(r, "set_error");
	expect_error_message("error x");
}

/* static-output */
void test_result()
{
	test_result_set_error();
}
