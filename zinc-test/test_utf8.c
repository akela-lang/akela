#include <string.h>
#include <stdio.h>
#include "zinc/unit_test.h"
#include "zinc/utf8.h"
#include "zinc/result.h"

/* dynamic-output-none */
void test_utf8_one_byte()
{
	test_name(__func__);

	char* s = "abcdefghijklmnopqrstuvwxyz0123456789+-[]()*/#^&%@!.,?{}|\\'"":;-_=";
	int num = strlen(s);

	for (int i = 0; i < num; i++) {
		expect_false(IS_EXTRA_BYTE(s[i]), "not extra byte");
		expect_true(IS_ONE_BYTE(s[i]), "is one byte");
		expect_false(IS_TWO_BYTES(s[i]), "is two byte");
		expect_false(IS_THREE_BYTES(s[i]), "is three byte");
		expect_false(IS_FOUR_BYTES(s[i]), "is four byte");
		expect_int_equal(NUM_BYTES(s[i]), 1, "num bytes");
	}
}

/* dynamic-output-none */
void test_utf8_two_byte()
{
	test_name(__func__);

	char* s = "ÁÉÍÓÚÜÑáéíóúüñ¡¿";
	int num = strlen(s);
	char c[5];

	int i = 0;
	while (i < num) {
		int byte_count = NUM_BYTES(s[i]);
		assert_true(byte_count > 0 && byte_count <= 4, "byte_count");

		int j = 0;
		while (j < byte_count) {
			c[j++] = s[i++];
		}
		c[j] = '\0';

		set_error("num bytes for (%s)", c);
		assert_int_equal(byte_count, 2, error_message);

		set_error("not extra byte for (%s)", c);
		expect_false(IS_EXTRA_BYTE(c[0]), error_message);

		j = 1;
		while (j < byte_count) {
			set_error("extra byte for (%s)", c);
			expect_true(IS_EXTRA_BYTE(c[j]), error_message);
			j++;
		}

		set_error("one byte for (%s)", c);
		expect_false(IS_ONE_BYTE(c[0]), error_message);

		set_error("two bytes for (%s)", c);
		expect_true(IS_TWO_BYTES(c[0]), error_message);

		set_error("three bytes for (%s)", c);
		expect_false(IS_THREE_BYTES(c[0]), error_message);

		set_error("four bytes for (%s)", c);
		expect_false(IS_FOUR_BYTES(c[0]), error_message);
	}
}

/* dynamic-output-none */
void test_utf8_three_byte()
{
	test_name(__func__);

	char* s = "€";

	int byte_count = NUM_BYTES(s[0]);
	assert_true(byte_count > 0 && byte_count <= 4, "byte_count");

	set_error("num bytes for (%s)", s);
	assert_int_equal(byte_count, 3, error_message);

	set_error("not extra byte for (%s)", s);
	expect_false(IS_EXTRA_BYTE(s[0]), error_message);

	int j = 1;
	while (j < byte_count) {
		set_error("extra byte for (%s)", s);
		expect_true(IS_EXTRA_BYTE(s[j]), error_message);
		j++;
	}

	set_error("one byte for (%s)", s);
	expect_false(IS_ONE_BYTE(s[0]), error_message);

	set_error("two bytes for (%s)", s);
	expect_false(IS_TWO_BYTES(s[0]), error_message);

	set_error("three bytes for (%s)", s);
	expect_true(IS_THREE_BYTES(s[0]), error_message);

	set_error("four bytes for (%s)", s);
	expect_false(IS_FOUR_BYTES(s[0]), error_message);
}

/* dynamic-output-none */
void test_utf8_four_byte()
{
	test_name(__func__);

	char* s = "𐍈";

	int byte_count = NUM_BYTES(s[0]);
	assert_true(byte_count > 0 && byte_count <= 4, "byte_count");

	set_error("num bytes for (%s)", s);
	assert_int_equal(byte_count, 4, error_message);

	set_error("not extra byte for (%s)", s);
	expect_false(IS_EXTRA_BYTE(s[0]), error_message);

	int j = 1;
	while (j < byte_count) {
		set_error("extra byte for (%s)", s);
		expect_true(IS_EXTRA_BYTE(s[j]), error_message);
		j++;
	}

	set_error("one byte for (%s)", s);
	expect_false(IS_ONE_BYTE(s[0]), error_message);

	set_error("two bytes for (%s)", s);
	expect_false(IS_TWO_BYTES(s[0]), error_message);

	set_error("three bytes for (%s)", s);
	expect_false(IS_THREE_BYTES(s[0]), error_message);

	set_error("four bytes for (%s)", s);
	expect_true(IS_FOUR_BYTES(s[0]), error_message);
}

/* dynamic-output-none */
void test_utf8()
{
	test_utf8_one_byte();
	test_utf8_two_byte();
	test_utf8_three_byte();
	test_utf8_four_byte();
}
