#include "zinc/unit_test.h"
#include "zinc/hash.h"

/* dynamic-output-none */
void test_hash1()
{
	test_name(__func__);

	struct hash_table ht;
	struct Zinc_string one;
	struct Zinc_string two;
	int one_item = 1;
	int two_item = 2;

	Zinc_string_init(&one);
	Zinc_string_init(&two);

	/* allocate one{} two{} */
	Zinc_string_add_str(&one, "one");
	Zinc_string_add_str(&two, "two");

	/* allocate ht{} */
	hash_table_init(&ht, 10);

	/* allocate ht{} */
	hash_table_add(&ht, &one, &one_item);

	int* one_item_ptr = hash_table_get(&ht, &one);
	assert_ptr(one_item_ptr, "one ptr");
	expect_int_equal(*one_item_ptr, one_item, "1");

	int* two_item_ptr = hash_table_get(&ht, &two);
	expect_null(two_item_ptr, "2 null");

	hash_table_add(&ht, &two, &two_item);
	two_item_ptr = hash_table_get(&ht, &two);
	assert_ptr(two_item_ptr, "2 ptr");
	expect_int_equal(*two_item_ptr, two_item, "2");

	/* destroy one{} two{} ht{} */
	Zinc_string_destroy(&one);
	Zinc_string_destroy(&two);
	hash_table_destroy(&ht);
}

void test_hash()
{
	test_hash1();
}
