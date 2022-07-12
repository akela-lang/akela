#include <stdio.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdbool.h>
#include "lookahead_char.h"
#include "uconv.h"
#include "input.h"
#include "result.h"

void lookahead_char_init(struct lookahead_char* lc, input_getchar f, input_data d, UConverter* conv)
{
	lc->lb_8 = &lc->lookahead[0];
	lc->la0_8 = &lc->lookahead[4];
	lc->la1_8 = &lc->lookahead[8];
	lc->la2_8 = &lc->lookahead[12];
	lc->lb_size = 0;
	lc->la_size = 0;
	lc->tr_in_size = 0;
	lc->tr_out_size = 0;
	lc->tr_out_pos = 0;
	lc->f = f;
	lc->d = d;
	lc->has_partial = false;
	lc->done = false;
	lc->conv = conv;
	lc->line = 1;
	lc->col = 1;
	lc->last_col_count = 0;
	lc->byte_pos = 0;
}

enum result lookahead_char_get_input(struct lookahead_char* lc)
{
	int c;
	int count;

	if (lc->tr_in_pos < lc->tr_in_size) return set_error("More input not needed: in pos is less than in size");

	lc->tr_in_size = 0;
	lc->tr_in_pos = 0;

	if (lc->has_partial) {
		c = lc->partial;
	} else {
		c = lc->f(lc->d);
	}
	if (c == EOF) {
		lc->done = true;
	}
	count = NUM_BYTES(c);
	if (count == 0) return set_error("Incorrect utf-8 encoding: byte count not encoded");
	do {
		if (lc->tr_in_size + count < TR_IN_SIZE) {
			lc->tr_in[lc->tr_in_size++] = c;
			lc->has_partial = false;
			for (int i = 1; i < count; i++) {
				c = lc->f(lc->d);
				if (c == EOF) {
					lc->done = true;
					return set_error("Got EOF before end of utf-8 char");
				}
				if (!IS_EXTRA_BYTE(c)) return set_error("Incorrect utf-8 encoding: not extra byte");
				lc->tr_in[lc->tr_in_size++] = c;
			}
		} else {
			lc->partial = c;
			lc->has_partial = true;
			break;
		}
		c = lc->f(lc->d);
		if (c == EOF) {
			lc->done = true;
			break;
		}
		count = NUM_BYTES(c);
		if (count == 0) return set_error("Incorrect utf-8 encoding: byte count not encoded");
	} while (lc->tr_in_size + count < TR_IN_SIZE);

	return result_ok;
}

enum result lookahead_char_translate(struct lookahead_char* lc)
{
	int32_t len;
	UErrorCode err;

	if (lc->tr_out_pos < lc->tr_out_size) return set_error("Translation not needed: out pos is less than out size");

	lc->tr_out_size = 0;
	lc->tr_out_pos = 0;

	len = ucnv_toUChars(lc->conv, lc->tr_out, TR_OUT_SIZE, lc->tr_in, lc->tr_in_size, &err);
	if (U_FAILURE(err)) {
		return set_error("utf error: %d", err);
	}

	lc->tr_out_size = len;
	return result_ok;
}

bool lookahead_char_has_utf8(struct lookahead_char* lc)
{
	return lc->tr_in_pos < lc->tr_in_size;
}

bool lookahead_char_has_utf16(struct lookahead_char* lc)
{
	return lc->tr_out_pos < lc->tr_out_size;
}

bool lookahead_char_utf8_pop(struct lookahead_char* lc, char* buf)
{
	char c;
	int count;

	if (lc->tr_in_pos < lc->tr_in_size) {
		c = lc->tr_in[lc->tr_in_pos++];
		int i = 0;
		buf[i++] = c;
		count = NUM_BYTES(c);
		while (i < count) {
			c = lc->tr_in[lc->tr_in_pos++];
			buf[i++] = c;
		}
		return true;
	}

	return false;
}

bool lookahead_char_utf32_pop(struct lookahead_char* lc, UChar32* c)
{
	if (lc->tr_out_pos < lc->tr_out_size) {
		U16_NEXT(lc->tr_out, lc->tr_out_pos, lc->tr_out_size, *c);
		return true;
	}

	return false;
}

bool lookahead_char_done_loading(struct lookahead_char* lc)
{
	return lc->done && !lookahead_char_has_utf8(lc) && !lookahead_char_has_utf16(lc);
}

bool lookahead_char_done(struct lookahead_char* lc)
{
	return lc->done && !lookahead_char_has_utf8 && !lookahead_char_has_utf16(lc) && lc->la_size <= 0;
}

bool lookahead_char_need_loading_prep(struct lookahead_char* lc)
{
	return !lookahead_char_has_utf8(lc) || !lookahead_char_has_utf16(lc);
}

bool lookahead_char_need_loading(struct lookahead_char* lc)
{
	return lc->la_size < 2;
}

enum result lookahead_char_load_prep(struct lookahead_char* lc)
{
	enum result r;
	if (!lookahead_char_has_utf8(lc)) {
		if (!lc->done) {
			r = lookahead_char_get_input(lc);
			if (r == result_error) return r;
		}
	}
	if (!lookahead_char_has_utf16(lc)) {
		r = lookahead_char_translate(lc);
		if (r == result_error) return r;
	}

	return result_ok;
}

bool lookahead_char_load(struct lookahead_char* lc)
{
	bool have_char = false;
	
	if (lookahead_char_has_utf8(lc) && lookahead_char_has_utf16(lc)) {
		if (lc->la_size == 0) {
			have_char = lookahead_char_utf8_pop(lc, lc->la0_8);
			have_char = lookahead_char_utf32_pop(lc, &lc->la0_32);
			lc->la_size++;
		}
		if (lc->la_size == 1) {
			have_char = lookahead_char_utf8_pop(lc, lc->la1_8);
			have_char = lookahead_char_utf32_pop(lc, &lc->la1_32);
			lc->la_size++;
		}
	}

	return lookahead_char_done_loading(lc);
}

void lookahead_char_pop(struct lookahead_char* lc)
{
	if (lc->la_size > 0) {
		/* lookbehind = lookahead0 */
		lc->lb_8 = lc->la0_8;
		lc->lb_32 = lc->la0_32;

		/* lookahead0 = lookahead1 */
		lc->la0_8 = lc->la1_8;
		lc->la0_32 = lc->la1_32;

		/* lookahead1 = lookahead2 */
		lc->la1_8 = lc->la2_8;
		lc->la1_32 = lc->la2_32;

		/* lookahead and lookbehind sizes */
		lc->la_size--;
		if (lc->lb_size == 0) lc->lb_size++;

		/* line and col */
		if (lc->lb_32 == '\n') {
			lc->line++;
			lc->last_col_count = lc->col;
			lc->col = 1;
		} else {
			lc->col++;
		}

		/* bytes */
		lc->byte_pos += NUM_BYTES(lc->lb_8[0]);
	}
}

void lookahead_char_push(struct lookahead_char* lc)
{
	if (lc->lb_size >= 1) {
		/* lookahead2 = lookahead1 */
		lc->la2_8 = lc->la1_8;
		lc->la2_32 = lc->la1_32;

		/* lookahead1 = lookahead0 */
		lc->la1_8 = lc->la0_8;
		lc->la1_32 = lc->la0_32;

		/* lookahead0 = lookbehind */
		lc->la0_8 = lc->lb_8;
		lc->la0_32 = lc->lb_32;

		/* lookahead and lookbehind sizes */
		lc->la_size++;
		lc->lb_size--;

		/* line and col */
		if (lc->la0_32 == '\n') {
			lc->line--;
			lc->col = lc->last_col_count;
		} else {
			lc->col--;
		}

		/* bytes */
		lc->byte_pos -= NUM_BYTES(lc->la0_8[0]);
	}
}
