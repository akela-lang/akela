#include "assert.h"
#include "alba/lookahead_char.h"
#include "alba/input.h"
#include "alba/buffer.h"
#include "alba/allocator.h"
#include "alba/uconv.h"

void test_lookahead_char_short()
{
	test_name(__func__);

	enum result r;

	struct allocator al;
	allocator_init(&al);

	struct buffer bf;
	buffer_init(&bf);

	r = array2buffer(&al, "hello", &bf);
	assert_ok(r, "array2buffer");

	struct string_data sd;
	string_data_init(&bf, &sd);

	UConverter* conv;
	r = conv_open(&conv);
	assert_ok(r, "conv_open");

	struct lookahead_char lc;
	lookahead_char_init(&lc, string_getchar, &sd, conv);

	expect_true(lookahead_char_need_loading_prep(&lc), "need loading prep");
	expect_true(lookahead_char_need_loading(&lc), "need loading");

	/* prep */
	r = lookahead_char_load_prep(&lc);
	assert_ok(r, "load prep");
	assert_true(lc.done, "done");
	assert_true(lookahead_char_has_utf8(&lc), "has utf8");
	assert_int_equal(lc.tr_in_size, 5, "tr in size");
	assert_true(lookahead_char_has_utf16(&lc), "has utf16");

	/* load */
	lookahead_char_load(&lc);

	expect_int_equal(lc.la_size, 2, "la size");
	expect_int_equal(lc.lb_size, 0, "lb size");
	expect_utf8_char(lc.la0_8, "h", "h");
	expect_utf8_char(lc.la1_8, "e", "e");
	expect_utf32_char(lc.la0_32, 'h', "h");
	expect_utf32_char(lc.la1_32, 'e', "e");

	expect_false(lookahead_char_need_loading_prep(&lc), "need loading prep");
	expect_false(lookahead_char_need_loading(&lc), "need loading");

	/* pop */
	lookahead_char_pop(&lc);

	expect_int_equal(lc.la_size, 1, "la size");
	expect_int_equal(lc.lb_size, 1, "lb size");
	expect_utf8_char(lc.lb_8, "h", "h");
	expect_utf8_char(lc.la0_8, "e", "e");
	expect_utf32_char(lc.lb_32, 'h', "h");
	expect_utf32_char(lc.la0_32, 'e', "e");

	expect_false(lookahead_char_need_loading_prep(&lc), "need loading prep");
	expect_true(lookahead_char_need_loading(&lc), "need loading");

	/* reload */
	lookahead_char_load(&lc);

	expect_false(lookahead_char_need_loading_prep(&lc), "need loading prep");
	expect_false(lookahead_char_need_loading(&lc), "need loading");

	expect_int_equal(lc.la_size, 2, "la size");
	expect_int_equal(lc.lb_size, 1, "lb size");
	expect_utf8_char(lc.lb_8, "h", "h");
	expect_utf8_char(lc.la0_8, "e", "e");
	expect_utf8_char(lc.la1_8, "l", "l");
	expect_utf32_char(lc.lb_32, 'h', "h");
	expect_utf32_char(lc.la0_32, 'e', "e");
	expect_utf32_char(lc.la1_32, 'l', "l");

	/* push */
	lookahead_char_push(&lc);

	expect_false(lookahead_char_need_loading_prep(&lc), "need loading prep");
	expect_false(lookahead_char_need_loading(&lc), "need loading");

	expect_int_equal(lc.la_size, 3, "la size");
	expect_int_equal(lc.lb_size, 0, "lb size");
	expect_utf8_char(lc.la0_8, "h", "h");
	expect_utf8_char(lc.la1_8, "e", "e");
	expect_utf8_char(lc.la2_8, "l", "l");
	expect_utf32_char(lc.la0_32, 'h', "h");
	expect_utf32_char(lc.la1_32, 'e', "e");
	expect_utf32_char(lc.la2_32, 'l', "l");

	/* pop */
	lookahead_char_pop(&lc);

	expect_false(lookahead_char_need_loading_prep(&lc), "need loading prep");
	expect_false(lookahead_char_need_loading(&lc), "need loading");

	expect_int_equal(lc.la_size, 2, "la size");
	expect_int_equal(lc.lb_size, 1, "lb size");
	expect_utf8_char(lc.lb_8, "h", "h");
	expect_utf8_char(lc.la0_8, "e", "e");
	expect_utf8_char(lc.la1_8, "l", "l");
	expect_utf32_char(lc.lb_32, 'h', "h");
	expect_utf32_char(lc.la0_32, 'e', "e");
	expect_utf32_char(lc.la1_32, 'l', "l");

	allocator_destroy(&al);
}

void test_lookahead_char()
{
	test_lookahead_char_short();
}
