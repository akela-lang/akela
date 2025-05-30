#include <string.h>
#include "zinc/utf8.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_utf8_one_byte(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	char* s = "abcdefghijklmnopqrstuvwxyz0123456789+-[]()*/#^&%@!.,?{}|\\'"":;-_=";
	int num = strlen(s);

	for (int i = 0; i < num; i++) {
		Zinc_expect_false(test, ZINC_IS_EXTRA_BYTE(s[i]), "not extra byte");
		Zinc_expect_true(test, ZINC_IS_ONE_BYTE(s[i]), "is one byte");
		Zinc_expect_false(test, ZINC_IS_TWO_BYTES(s[i]), "is two byte");
		Zinc_expect_false(test, ZINC_IS_THREE_BYTES(s[i]), "is three byte");
		Zinc_expect_false(test, ZINC_IS_FOUR_BYTES(s[i]), "is four byte");
		Zinc_expect_int_equal(test, ZINC_NUM_BYTES(s[i]), 1, "num bytes");
	}
}

void Zinc_unit_utf8_two_byte(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	char* s = "ÁÉÍÓÚÜÑáéíóúüñ¡¿";
	int num = strlen(s);
	char c[5];

	int i = 0;
	while (i < num) {
		int byte_count = ZINC_NUM_BYTES(s[i]);
		Zinc_assert_true(test, byte_count > 0 && byte_count <= 4, "byte_count");

		int j = 0;
		while (j < byte_count) {
			c[j++] = s[i++];
		}
		c[j] = '\0';

		Zinc_set_error("num bytes for (%s)", c);
		Zinc_assert_int_equal(test, byte_count, 2, Zinc_error_message);

		Zinc_set_error("not extra byte for (%s)", c);
		Zinc_expect_false(test, ZINC_IS_EXTRA_BYTE(c[0]), Zinc_error_message);

		j = 1;
		while (j < byte_count) {
			Zinc_set_error("extra byte for (%s)", c);
			Zinc_expect_true(test, ZINC_IS_EXTRA_BYTE(c[j]), Zinc_error_message);
			j++;
		}

		Zinc_set_error("one byte for (%s)", c);
		Zinc_expect_false(test, ZINC_IS_ONE_BYTE(c[0]), Zinc_error_message);

		Zinc_set_error("two bytes for (%s)", c);
		Zinc_expect_true(test, ZINC_IS_TWO_BYTES(c[0]), Zinc_error_message);

		Zinc_set_error("three bytes for (%s)", c);
		Zinc_expect_false(test, ZINC_IS_THREE_BYTES(c[0]), Zinc_error_message);

		Zinc_set_error("four bytes for (%s)", c);
		Zinc_expect_false(test, ZINC_IS_FOUR_BYTES(c[0]), Zinc_error_message);
	}
}

void Zinc_unit_utf8_three_byte(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	char* s = "€";

	int byte_count = ZINC_NUM_BYTES(s[0]);
	Zinc_assert_true(test, byte_count > 0 && byte_count <= 4, "byte_count");

	Zinc_set_error("num bytes for (%s)", s);
	Zinc_assert_int_equal(test, byte_count, 3, Zinc_error_message);

	Zinc_set_error("not extra byte for (%s)", s);
	Zinc_expect_false(test, ZINC_IS_EXTRA_BYTE(s[0]), Zinc_error_message);

	int j = 1;
	while (j < byte_count) {
		Zinc_set_error("extra byte for (%s)", s);
		Zinc_expect_true(test, ZINC_IS_EXTRA_BYTE(s[j]), Zinc_error_message);
		j++;
	}

	Zinc_set_error("one byte for (%s)", s);
	Zinc_expect_false(test, ZINC_IS_ONE_BYTE(s[0]), Zinc_error_message);

	Zinc_set_error("two bytes for (%s)", s);
	Zinc_expect_false(test, ZINC_IS_TWO_BYTES(s[0]), Zinc_error_message);

	Zinc_set_error("three bytes for (%s)", s);
	Zinc_expect_true(test, ZINC_IS_THREE_BYTES(s[0]), Zinc_error_message);

	Zinc_set_error("four bytes for (%s)", s);
	Zinc_expect_false(test, ZINC_IS_FOUR_BYTES(s[0]), Zinc_error_message);
}

void Zinc_unit_utf8_four_byte(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	char* s = "𐍈";

	int byte_count = ZINC_NUM_BYTES(s[0]);
	Zinc_assert_true(test, byte_count > 0 && byte_count <= 4, "byte_count");

	Zinc_set_error("num bytes for (%s)", s);
	Zinc_assert_int_equal(test, byte_count, 4, Zinc_error_message);

	Zinc_set_error("not extra byte for (%s)", s);
	Zinc_expect_false(test, ZINC_IS_EXTRA_BYTE(s[0]), Zinc_error_message);

	int j = 1;
	while (j < byte_count) {
		Zinc_set_error("extra byte for (%s)", s);
		Zinc_expect_true(test, ZINC_IS_EXTRA_BYTE(s[j]), Zinc_error_message);
		j++;
	}

	Zinc_set_error("one byte for (%s)", s);
	Zinc_expect_false(test, ZINC_IS_ONE_BYTE(s[0]), Zinc_error_message);

	Zinc_set_error("two bytes for (%s)", s);
	Zinc_expect_false(test, ZINC_IS_TWO_BYTES(s[0]), Zinc_error_message);

	Zinc_set_error("three bytes for (%s)", s);
	Zinc_expect_false(test, ZINC_IS_THREE_BYTES(s[0]), Zinc_error_message);

	Zinc_set_error("four bytes for (%s)", s);
	Zinc_expect_true(test, ZINC_IS_FOUR_BYTES(s[0]), Zinc_error_message);
}

void Zinc_unit_utf8(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, Zinc_unit_utf8_one_byte);
		Zinc_test_register(test, Zinc_unit_utf8_two_byte);
		Zinc_test_register(test, Zinc_unit_utf8_three_byte);
		Zinc_test_register(test, Zinc_unit_utf8_four_byte);

		return;
	}

	Zinc_test_perform(test);
}
