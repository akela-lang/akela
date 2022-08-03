#include "zinc/unit_test.h"
#include "zinc/result.h"
#include "alba/lookahead_translator.h"

/* dynamic-output-none */
/* dynamic-temp cbyte{} */
void test_lookahead_translator_cbyte()
{
	test_name(__func__);

	enum result r;
	struct circular_byte_buffer cbyte;
	int size = 16;

	/* allocate cbyte{} */
	circular_byte_buffer_init(&cbyte, size);

	char* s = "0123456789abcdef";
	char* p = s;

	/* add until full */
	while (*p) {
		r = circular_byte_buffer_add(&cbyte, *p);
		assert_ok(r, "add");
		p++;
	}

	expect_int_equal(cbyte.size, 16, "size");

	/* failed add */
	r = circular_byte_buffer_add(&cbyte, 'x');
	assert_error(r, "no room");
	expect_error_message("No room in circular byte buffer: add x");

	/* pop */
	char c = circular_byte_buffer_pop(&cbyte);
	expect_char_equal(c, '0', "pop");
	expect_int_equal(cbyte.size, 15, "size");
	expect_int_equal(cbyte.start, 1, "start");

	/* add */
	r = circular_byte_buffer_add(&cbyte, 'g');
	assert_ok(r, "add");
	expect_int_equal(cbyte.size, 16, "size");
	expect_int_equal(cbyte.start, 1, "start");
	expect_char_equal(cbyte.buf[0], 'g', "g");

	/* destroy cbyte{} */
	circular_byte_buffer_destroy(&cbyte);
}

/* dynamic-output-none */
/* dynamic-temp cc32{} */
void test_lookahead_translator_circular_uchar32_buffer()
{
	test_name(__func__);

	enum result r;
	struct circular_uchar32_buffer cc32;
	int size = 16;

	/* allocate cc32{} */
	circular_uchar32_buffer_init(&cc32, size);

	int i;

	/* add until full */
	for (i = 0; i < 16; i++) {
		r = circular_uchar32_buffer_add(&cc32, i);
		assert_ok(r, "add");
	}

	expect_int_equal(cc32.size, 16, "size");

	/* failed add */
	r = circular_uchar32_buffer_add(&cc32, 100);
	assert_error(r, "no room");
	expect_error_message("No room in circular UChar32 buffer: add 100");

	/* pop */
	UChar32 c = circular_uchar32_buffer_pop(&cc32);
	expect_utf32_char(c, 0, "pop");
	expect_int_equal(cc32.size, 15, "size");
	expect_int_equal(cc32.start, 1, "start");

	/* add */
	r = circular_uchar32_buffer_add(&cc32, 'g');
	assert_ok(r, "add");
	expect_int_equal(cc32.size, 16, "size");
	expect_int_equal(cc32.start, 1, "start");
	expect_char_equal(cc32.buf[0], 'g', "g");

	/* destroy cc32{} */
	circular_uchar32_buffer_destroy(&cc32);
}

/* dynamic-output-none */
void test_lookahead_translator()
{
	test_lookahead_translator_cbyte();
	test_lookahead_translator_circular_uchar32_buffer();
}
