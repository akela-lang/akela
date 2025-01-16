#include <string.h>
#include <stdio.h>
#include "zinc/unit_test.h"
#include "zinc/utf8.h"
#include "zinc/result.h"

/* dynamic-output-none */
void test_utf8_one_byte()
{
	Zinc_test_name(__func__);

	char* s = "abcdefghijklmnopqrstuvwxyz0123456789+-[]()*/#^&%@!.,?{}|\\'"":;-_=";
	int num = strlen(s);

	for (int i = 0; i < num; i++) {
		Zinc_expect_false(ZINC_IS_EXTRA_BYTE(s[i]), "not extra byte");
		Zinc_expect_true(ZINC_IS_ONE_BYTE(s[i]), "is one byte");
		Zinc_expect_false(ZINC_IS_TWO_BYTES(s[i]), "is two byte");
		Zinc_expect_false(ZINC_IS_THREE_BYTES(s[i]), "is three byte");
		Zinc_expect_false(ZINC_IS_FOUR_BYTES(s[i]), "is four byte");
		Zinc_expect_int_equal(ZINC_NUM_BYTES(s[i]), 1, "num bytes");
	}
}

/* dynamic-output-none */
void test_utf8_two_byte()
{
	Zinc_test_name(__func__);

	char* s = "ÁÉÍÓÚÜÑáéíóúüñ¡¿";
	int num = strlen(s);
	char c[5];

	int i = 0;
	while (i < num) {
		int byte_count = ZINC_NUM_BYTES(s[i]);
		Zinc_assert_true(byte_count > 0 && byte_count <= 4, "byte_count");

		int j = 0;
		while (j < byte_count) {
			c[j++] = s[i++];
		}
		c[j] = '\0';

		Zinc_set_error("num bytes for (%s)", c);
		Zinc_assert_int_equal(byte_count, 2, Zinc_error_message);

		Zinc_set_error("not extra byte for (%s)", c);
		Zinc_expect_false(ZINC_IS_EXTRA_BYTE(c[0]), Zinc_error_message);

		j = 1;
		while (j < byte_count) {
			Zinc_set_error("extra byte for (%s)", c);
			Zinc_expect_true(ZINC_IS_EXTRA_BYTE(c[j]), Zinc_error_message);
			j++;
		}

		Zinc_set_error("one byte for (%s)", c);
		Zinc_expect_false(ZINC_IS_ONE_BYTE(c[0]), Zinc_error_message);

		Zinc_set_error("two bytes for (%s)", c);
		Zinc_expect_true(ZINC_IS_TWO_BYTES(c[0]), Zinc_error_message);

		Zinc_set_error("three bytes for (%s)", c);
		Zinc_expect_false(ZINC_IS_THREE_BYTES(c[0]), Zinc_error_message);

		Zinc_set_error("four bytes for (%s)", c);
		Zinc_expect_false(ZINC_IS_FOUR_BYTES(c[0]), Zinc_error_message);
	}
}

/* dynamic-output-none */
void test_utf8_three_byte()
{
	Zinc_test_name(__func__);

	char* s = "€";

	int byte_count = ZINC_NUM_BYTES(s[0]);
	Zinc_assert_true(byte_count > 0 && byte_count <= 4, "byte_count");

	Zinc_set_error("num bytes for (%s)", s);
	Zinc_assert_int_equal(byte_count, 3, Zinc_error_message);

	Zinc_set_error("not extra byte for (%s)", s);
	Zinc_expect_false(ZINC_IS_EXTRA_BYTE(s[0]), Zinc_error_message);

	int j = 1;
	while (j < byte_count) {
		Zinc_set_error("extra byte for (%s)", s);
		Zinc_expect_true(ZINC_IS_EXTRA_BYTE(s[j]), Zinc_error_message);
		j++;
	}

	Zinc_set_error("one byte for (%s)", s);
	Zinc_expect_false(ZINC_IS_ONE_BYTE(s[0]), Zinc_error_message);

	Zinc_set_error("two bytes for (%s)", s);
	Zinc_expect_false(ZINC_IS_TWO_BYTES(s[0]), Zinc_error_message);

	Zinc_set_error("three bytes for (%s)", s);
	Zinc_expect_true(ZINC_IS_THREE_BYTES(s[0]), Zinc_error_message);

	Zinc_set_error("four bytes for (%s)", s);
	Zinc_expect_false(ZINC_IS_FOUR_BYTES(s[0]), Zinc_error_message);
}

/* dynamic-output-none */
void test_utf8_four_byte()
{
	Zinc_test_name(__func__);

	char* s = "𐍈";

	int byte_count = ZINC_NUM_BYTES(s[0]);
	Zinc_assert_true(byte_count > 0 && byte_count <= 4, "byte_count");

	Zinc_set_error("num bytes for (%s)", s);
	Zinc_assert_int_equal(byte_count, 4, Zinc_error_message);

	Zinc_set_error("not extra byte for (%s)", s);
	Zinc_expect_false(ZINC_IS_EXTRA_BYTE(s[0]), Zinc_error_message);

	int j = 1;
	while (j < byte_count) {
		Zinc_set_error("extra byte for (%s)", s);
		Zinc_expect_true(ZINC_IS_EXTRA_BYTE(s[j]), Zinc_error_message);
		j++;
	}

	Zinc_set_error("one byte for (%s)", s);
	Zinc_expect_false(ZINC_IS_ONE_BYTE(s[0]), Zinc_error_message);

	Zinc_set_error("two bytes for (%s)", s);
	Zinc_expect_false(ZINC_IS_TWO_BYTES(s[0]), Zinc_error_message);

	Zinc_set_error("three bytes for (%s)", s);
	Zinc_expect_false(ZINC_IS_THREE_BYTES(s[0]), Zinc_error_message);

	Zinc_set_error("four bytes for (%s)", s);
	Zinc_expect_true(ZINC_IS_FOUR_BYTES(s[0]), Zinc_error_message);
}

/* dynamic-output-none */
void test_utf8()
{
	test_utf8_one_byte();
	test_utf8_two_byte();
	test_utf8_three_byte();
	test_utf8_four_byte();
}
