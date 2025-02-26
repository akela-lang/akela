#include "zinc/unit_test.h"
#include "zinc/hash_map_string.h"

void test_hash1()
{
	Zinc_test_name(__func__);

	Zinc_hash_map_string ht;
	Zinc_string one;
	Zinc_string two;
	int one_item = 1;
	int two_item = 2;

	Zinc_string_init(&one);
	Zinc_string_init(&two);

	Zinc_string_add_str(&one, "one");
	Zinc_string_add_str(&two, "two");

	Zinc_hash_map_string_init(&ht, 10);

	Zinc_hash_map_string_add(&ht, &one, &one_item);

	int* one_item_ptr = Zinc_hash_map_string_get(&ht, &one);
	Zinc_assert_ptr(one_item_ptr, "one ptr");
	Zinc_expect_int_equal(*one_item_ptr, one_item, "1");

	int* two_item_ptr = Zinc_hash_map_string_get(&ht, &two);
	Zinc_expect_null(two_item_ptr, "2 null");

	Zinc_hash_map_string_add(&ht, &two, &two_item);
	two_item_ptr = Zinc_hash_map_string_get(&ht, &two);
	Zinc_assert_ptr(two_item_ptr, "2 ptr");
	Zinc_expect_int_equal(*two_item_ptr, two_item, "2");

	Zinc_string_destroy(&one);
	Zinc_string_destroy(&two);
	Zinc_hash_map_string_destroy(&ht);
}

void test_hash_map_string_remove()
{
	Zinc_test_name(__func__);

	Zinc_hash_map_string ht;
	Zinc_hash_map_string_init(&ht, 8);

	char name[100][100];
	int value[100];
	for (int i = 0; i < 100; i++) {
		value[i] = i;
		snprintf(name[i], 100, "name-%d", i);
	}

	for (int i = 0; i < 100; i++) {
		Zinc_hash_map_string_add_str(&ht, name[i], &value[i]);
	}

	for (int i = 0; i < 100; i++) {
		int* v = Zinc_hash_map_string_get_str(&ht, name[i]);
		Zinc_expect_int_equal(*v, value[i], "get");
	}

	for (int i = 0; i < 100; i++) {
		int* v = Zinc_hash_map_string_remove_str(&ht, name[i]);
		Zinc_expect_int_equal(*v, value[i], "remove");
		Zinc_expect_null(Zinc_hash_map_string_get_str(&ht, name[i]), "null");
	}

	Zinc_hash_map_string_destroy(&ht);
}

void test_hash()
{
	test_hash1();
	test_hash_map_string_remove();
}