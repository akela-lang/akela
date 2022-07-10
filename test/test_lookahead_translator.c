#include "assert.h"
#include "alba/lookahead_translator.h"

void test_lookahead_translator_cbyte()
{
	test_name(__func__);

	enum result r;
	struct circular_byte_buffer cbyte;
	int size = 16;

	r = circular_byte_buffer_init(&cbyte, size);
	assert_ok(r, "init");

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
}

void test_lookahead_translator()
{
	test_lookahead_translator_cbyte();
}
