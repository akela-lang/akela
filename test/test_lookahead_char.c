#include "zinc/unit_test.h"
#include "assert_compiler.h"
#include "alba/lookahead_char.h"
#include "alba/input.h"
#include "zinc/buffer.h"
#include "alba/uconv.h"

/* dynamic-output-none */
/* dynamic-temp bf{} conv */
void test_lookahead_char_short()
{
	test_name(__func__);

	enum result r;

	struct buffer bf;
	buffer_init(&bf);

	/* allocate bf{} */
	array2buffer("hello", &bf);

	struct string_data sd;
	string_data_init(&bf, &sd);

	UConverter* conv;
	/* open resource */
	r = conv_open(&conv);
	assert_ok(r, "conv_open");

	struct lookahead_char lc;
	lookahead_char_init(&lc, (input_getchar)string_getchar, &sd, conv);

	expect_true(lookahead_char_need_preping(&lc), "need loading prep");

	/* prep */
	r = lookahead_char_prep(&lc);
	assert_ok(r, "load prep");
	assert_true(lc.done, "done");
	assert_true(lookahead_char_has_utf8(&lc), "has utf8");
	assert_int_equal(lc.tr_in_size, 5, "tr in size");
	assert_true(lookahead_char_has_utf16(&lc), "has utf16");

	/* load */
	expect_true(lookahead_char_need_loading(&lc), "need loading");
	lookahead_char_load(&lc);

	expect_int_equal(lc.la_size, 2, "la size");
	expect_int_equal(lc.lb_size, 0, "lb size");
	expect_utf8_char(lc.la0_8, "h", "h");
	expect_utf8_char(lc.la1_8, "e", "e");
	expect_utf32_char(lc.la0_32, 'h', "h");
	expect_utf32_char(lc.la1_32, 'e', "e");

	expect_size_t_equal(lc.line, 1, "line");
	expect_size_t_equal(lc.col, 1, "col");
	expect_size_t_equal(lc.last_col_count, 0, "last col count");
	expect_size_t_equal(lc.byte_pos, 0, "byte pos");

	expect_false(lookahead_char_need_preping(&lc), "need loading prep");
	expect_false(lookahead_char_need_loading(&lc), "need loading");

	/* pop */
	lookahead_char_pop(&lc);

	expect_int_equal(lc.la_size, 1, "la size");
	expect_int_equal(lc.lb_size, 1, "lb size");
	expect_utf8_char(lc.lb_8, "h", "h");
	expect_utf8_char(lc.la0_8, "e", "e");
	expect_utf32_char(lc.lb_32, 'h', "h");
	expect_utf32_char(lc.la0_32, 'e', "e");

	expect_size_t_equal(lc.line, 1, "line");
	expect_size_t_equal(lc.col, 2, "col");
	expect_size_t_equal(lc.last_col_count, 0, "last col count");
	expect_size_t_equal(lc.byte_pos, 1, "byte pos");

	expect_false(lookahead_char_need_preping(&lc), "need loading prep");
	expect_true(lookahead_char_need_loading(&lc), "need loading");

	/* reload */
	lookahead_char_load(&lc);

	expect_false(lookahead_char_need_preping(&lc), "need loading prep");
	expect_false(lookahead_char_need_loading(&lc), "need loading");

	expect_int_equal(lc.la_size, 2, "la size");
	expect_int_equal(lc.lb_size, 1, "lb size");
	expect_utf8_char(lc.lb_8, "h", "h");
	expect_utf8_char(lc.la0_8, "e", "e");
	expect_utf8_char(lc.la1_8, "l", "l");
	expect_utf32_char(lc.lb_32, 'h', "h");
	expect_utf32_char(lc.la0_32, 'e', "e");
	expect_utf32_char(lc.la1_32, 'l', "l");

	expect_size_t_equal(lc.line, 1, "line");
	expect_size_t_equal(lc.col, 2, "col");
	expect_size_t_equal(lc.last_col_count, 0, "last col count");
	expect_size_t_equal(lc.byte_pos, 1, "byte pos");

	/* push */
	lookahead_char_push(&lc);

	expect_false(lookahead_char_need_preping(&lc), "need loading prep");
	expect_false(lookahead_char_need_loading(&lc), "need loading");

	expect_int_equal(lc.la_size, 3, "la size");
	expect_int_equal(lc.lb_size, 0, "lb size");
	expect_utf8_char(lc.la0_8, "h", "h");
	expect_utf8_char(lc.la1_8, "e", "e");
	expect_utf8_char(lc.la2_8, "l", "l");
	expect_utf32_char(lc.la0_32, 'h', "h");
	expect_utf32_char(lc.la1_32, 'e', "e");
	expect_utf32_char(lc.la2_32, 'l', "l");

	expect_size_t_equal(lc.line, 1, "line");
	expect_size_t_equal(lc.col, 1, "col");
	expect_size_t_equal(lc.last_col_count, 0, "last col count");
	expect_size_t_equal(lc.byte_pos, 0, "byte pos");

	/* pop */
	lookahead_char_pop(&lc);

	expect_false(lookahead_char_need_preping(&lc), "need loading prep");
	expect_false(lookahead_char_need_loading(&lc), "need loading");

	expect_int_equal(lc.la_size, 2, "la size");
	expect_int_equal(lc.lb_size, 1, "lb size");
	expect_utf8_char(lc.lb_8, "h", "h");
	expect_utf8_char(lc.la0_8, "e", "e");
	expect_utf8_char(lc.la1_8, "l", "l");
	expect_utf32_char(lc.lb_32, 'h', "h");
	expect_utf32_char(lc.la0_32, 'e', "e");
	expect_utf32_char(lc.la1_32, 'l', "l");

	expect_size_t_equal(lc.line, 1, "line");
	expect_size_t_equal(lc.col, 2, "col");
	expect_size_t_equal(lc.last_col_count, 0, "last col count");
	expect_size_t_equal(lc.byte_pos, 1, "byte pos");

	/* destroy bf{} */
	buffer_destroy(&bf);

	/* close resource */
	conv_close(conv);
}

/* dynamic-output-none */
/* dynamic bf */
void test_lookahead_char_line()
{
	test_name(__func__);

	enum result r;

	struct buffer bf;
	buffer_init(&bf);

	/* allocate bf */
	array2buffer("one\ntwo", &bf);

	struct string_data sd;
	string_data_init(&bf, &sd);

	UConverter* conv;
	r = conv_open(&conv);
	assert_ok(r, "conv_open");

	struct lookahead_char lc;
	lookahead_char_init(&lc, (input_getchar)string_getchar, &sd, conv);

	r = lookahead_char_prep(&lc);
	assert_ok(r, "load prep");
	lookahead_char_load(&lc);

	expect_utf8_char(lc.la0_8, "o", "o");
	expect_int_equal(lc.line, 1, "line");
	expect_int_equal(lc.col, 1, "col");
	expect_int_equal(lc.byte_pos, 0, "byte_pos");
	expect_size_t_equal(lc.last_col_count, 0, "last col count");

	lookahead_char_pop(&lc);
	expect_utf8_char(lc.la0_8, "n", "n");
	expect_int_equal(lc.line, 1, "line");
	expect_int_equal(lc.col, 2, "col");
	expect_int_equal(lc.byte_pos, 1, "byte_pos");
	expect_size_t_equal(lc.last_col_count, 0, "last col count");
	if (lookahead_char_need_preping(&lc)) {
		r = lookahead_char_prep(&lc);
		assert_ok(r, "prep");
	}
	if (lookahead_char_need_loading(&lc)) lookahead_char_load(&lc);

	lookahead_char_pop(&lc);
	expect_utf8_char(lc.la0_8, "e", "e");
	expect_int_equal(lc.line, 1, "line");
	expect_int_equal(lc.col, 3, "col");
	expect_int_equal(lc.byte_pos, 2, "byte_pos");
	expect_size_t_equal(lc.last_col_count, 0, "last col count");
	if (lookahead_char_need_preping(&lc)) {
		r = lookahead_char_prep(&lc);
		assert_ok(r, "prep");
	}
	if (lookahead_char_need_loading(&lc)) lookahead_char_load(&lc);

	lookahead_char_pop(&lc);
	expect_utf8_char(lc.la0_8, "\n", "\n");
	expect_int_equal(lc.line, 1, "line");
	expect_int_equal(lc.col, 4, "col");
	expect_int_equal(lc.byte_pos, 3, "byte_pos");
	expect_size_t_equal(lc.last_col_count, 0, "last col count");
	if (lookahead_char_need_preping(&lc)) {
		r = lookahead_char_prep(&lc);
		assert_ok(r, "prep");
	}
	if (lookahead_char_need_loading(&lc)) lookahead_char_load(&lc);

	lookahead_char_pop(&lc);
	expect_utf8_char(lc.la0_8, "t", "t");
	expect_int_equal(lc.line, 2, "line");
	expect_int_equal(lc.col, 1, "col");
	expect_int_equal(lc.byte_pos, 4, "byte_pos");
	expect_size_t_equal(lc.last_col_count, 4, "last col count");
	if (lookahead_char_need_preping(&lc)) {
		r = lookahead_char_prep(&lc);
		assert_ok(r, "prep");
	}
	if (lookahead_char_need_loading(&lc)) lookahead_char_load(&lc);

	lookahead_char_pop(&lc);
	expect_utf8_char(lc.la0_8, "w", "w");
	expect_int_equal(lc.line, 2, "line");
	expect_int_equal(lc.col, 2, "col");
	expect_int_equal(lc.byte_pos, 5, "byte_pos");
	expect_size_t_equal(lc.last_col_count, 4, "last col count");
	if (lookahead_char_need_preping(&lc)) {
		r = lookahead_char_prep(&lc);
		assert_ok(r, "prep");
	}
	if (lookahead_char_need_loading(&lc)) lookahead_char_load(&lc);

	lookahead_char_pop(&lc);
	expect_utf8_char(lc.la0_8, "o", "o");
	expect_int_equal(lc.line, 2, "line");
	expect_int_equal(lc.col, 3, "col");
	expect_int_equal(lc.byte_pos, 6, "byte_pos");
	expect_size_t_equal(lc.last_col_count, 4, "last col count");
	if (lookahead_char_need_preping(&lc)) {
		r = lookahead_char_prep(&lc);
		assert_ok(r, "prep");
	}
	if (lookahead_char_need_loading(&lc)) lookahead_char_load(&lc);

	lookahead_char_pop(&lc);
	expect_true(lookahead_char_done(&lc), "done");

	/* destroy bf */
	buffer_destroy(&bf);

	conv_close(conv);
}

void test_lookahead_char()
{
	test_lookahead_char_short();
	test_lookahead_char_line();
}
