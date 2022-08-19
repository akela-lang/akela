#include "alba/type_use.h"
#include "zinc/unit_test.h"

void test_type_use1()
{
	test_name(__func__);

	struct type_use* n;
	type_use_create(&n);

	struct type_use* a;
	type_use_create(&a);
	type_use_add(n, a);

	struct type_use* b;
	type_use_create(&b);
	type_use_add(n, b);

	assert_ptr(n, "ptr n");

	struct type_use* n0 = type_use_get(n, 0);
	assert_ptr(n0, "ptr a");
	expect_ptr_equal(n0, a, "a");

	struct type_use* n1 = type_use_get(n, 1);
	assert_ptr(n1, "ptr b");
	expect_ptr_equal(n1, b, "b");

	type_use_destroy(n);
}


void test_type()
{
	test_type_use1();
}
