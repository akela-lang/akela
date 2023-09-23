#include <stdio.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdbool.h>
#include <assert.h>
#include "lookahead_char.h"
#include "uconv.h"
#include "input.h"
#include "zinc/result.h"
#include "zinc/utf8.h"

/**
 * Initialize lookahead_char struct
 * @param lc lookahead_char struct pointer
 * @param f getchar function
 * @param d data pointer for f
 * @param conv the utf convertor
 */
void lookahead_char_init(struct lookahead_char* lc, input_getchar f, input_data d, UConverter* conv)
{
	lc->lb_8 = &lc->lookahead[0];
	lc->la0_8 = &lc->lookahead[4];
	lc->la1_8 = &lc->lookahead[8];
	lc->la2_8 = &lc->lookahead[12];
	lc->lb_size = 0;
	lc->la_size = 0;
	lc->tr_in_size = 0;
	lc->tr_in_pos = 0;
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

/**
 * Fill in tr_in (translation input), a buffer of utf8 characters
 * @param lc lookahead struct pointer
 * @return result_ok if successful, otherwise result_error if there is a utf encoding error
 */
enum result lookahead_char_get_input(struct lookahead_char* lc)
{
	int c;
	int count;

	/* should only call when more characters needed */
	assert(lc->tr_in_pos >= lc->tr_in_size);

	lc->tr_in_size = 0;
	lc->tr_in_pos = 0;

	do {
		if (lc->has_partial) {
			c = lc->partial;
		} else {
			c = lc->f(lc->d);
		}
		if (c == EOF) {
			lc->done = true;
			return result_ok;
		}
		count = NUM_BYTES(c);
		if (count == 0) return set_error("Incorrect utf-8 encoding: byte count not encoded");
		if (lc->tr_in_size + count <= TR_IN_SIZE) {
			lc->tr_in[lc->tr_in_size++] = (char)c;
			lc->has_partial = false;
			for (int i = 1; i < count; i++) {
				c = lc->f(lc->d);
				if (c == EOF) {
					lc->done = true;
					return set_error("Got EOF before end of utf-8 char");
				}
				if (!IS_EXTRA_BYTE(c)) return set_error("Incorrect utf-8 encoding: not extra byte");
				lc->tr_in[lc->tr_in_size++] = (char)c;
			}
		} else {
			lc->partial = c;
			lc->has_partial = true;
			break;
		}
	} while (lc->tr_in_size < TR_IN_SIZE);

	return result_ok;
}

/**
 * Translate input buffer and output to tr_out
 * @param lc the lookahead_char struct
 * @return result_ok if successful, otherwise result_error if there is a translation error
 */
enum result lookahead_char_translate(struct lookahead_char* lc)
{
	int32_t len;
	UErrorCode err = U_ZERO_ERROR;

	/* should be called only when more characters are needed */
	assert(lc->tr_out_pos >= lc->tr_out_size);

	lc->tr_out_size = 0;
	lc->tr_out_pos = 0;

	/* use lc{conv} */
	len = ucnv_toUChars(lc->conv, lc->tr_out, TR_OUT_SIZE, lc->tr_in, (int32_t)lc->tr_in_size, &err);
	if (U_FAILURE(err)) {
		return set_error("utf error: %d", err);
	}

	lc->tr_out_size = len;
	return result_ok;
}

/**
 * Check if there is some unconverted utf8 input
 * @param lc the lookahead_char struct
 * @return true if there is some input, otherwise false
 */
bool lookahead_char_has_utf8(struct lookahead_char* lc)
{
	return lc->tr_in_pos < lc->tr_in_size;
}

/**
 * Check if there is some unprocessed utf16
 * @param lc lookahead_char struct pointer
 * @return true if there is some utf16 unprocessed, otherwise false
 */
bool lookahead_char_has_utf16(struct lookahead_char* lc)
{
	return lc->tr_out_pos < lc->tr_out_size;
}

/**
 * Pop a utf8 code point into buffer
 * @param lc the lookahead_char struct pointer
 * @param buf the destination buffer
 * @return true if the pop was successful, otherwise false
 */
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

/**
 * pop a utf32 character
 * @param lc the lookahead_char struct pointer
 * @param c outputted UTF32 char
 * @return true if successful, otherwise false
 */
bool lookahead_char_utf32_pop(struct lookahead_char* lc, UChar32* c)
{
	if (lc->tr_out_pos < lc->tr_out_size) {
		U16_NEXT(lc->tr_out, lc->tr_out_pos, lc->tr_out_size, *c);
		return true;
	}

	return false;
}

/**
 * check if there are no more characters put in lookahead
 * @param lc the lookahead_char struct pointer
 * @return true if there are no more characters to process, otherwise false
 */
bool lookahead_char_done_loading(struct lookahead_char* lc)
{
	return lc->done && !lookahead_char_has_utf8(lc) && !lookahead_char_has_utf16(lc);
}

/**
 * check if there are no more characters to read
 * @param lc the lookahead_char struct pointer
 * @return true if there are no more characters, otherwise false
 */
bool lookahead_char_done(struct lookahead_char* lc)
{
	return lc->done && !lookahead_char_has_utf8(lc) && !lookahead_char_has_utf16(lc) && lc->la_size <= 0;
}

/**
 * check if we need to prep input or translation buffers
 * @param lc the lookahead_char struct pointer
 * @return true of we need to prep input or translation buffers, otherwise false
 */
bool lookahead_char_need_preping(struct lookahead_char* lc)
{
	return (!lc->done && !lookahead_char_has_utf8(lc)) || (lookahead_char_has_utf8(lc) && !lookahead_char_has_utf16(lc));
}

/**
 * check if we need to load lookahead
 * @param lc the lookahead_char struct pointer
 * @return true if need to load lookahead, otherwise false
 */
bool lookahead_char_need_loading(struct lookahead_char* lc)
{
	return lookahead_char_has_utf8(lc) && lookahead_char_has_utf16(lc) && lc->la_size < 2;
}

/**
 * prep the input and translation buffers
 * @param lc the lookahead_char struct pointer
 * @return result_ok if successful, otherwise result_error if utf encoding or translation error
 */
enum result lookahead_char_prep(struct lookahead_char* lc)
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

/**
 * load lookahead
 * @param lc the lookahead_char struct pointer
 * @return true if no more input characters to load into lookahead
 */
bool lookahead_char_load(struct lookahead_char* lc)
{
	bool a = false, b = false;
	
	if (lookahead_char_has_utf8(lc) && lookahead_char_has_utf16(lc)) {
		if (lc->la_size == 0) {
			a = lookahead_char_utf8_pop(lc, lc->la0_8);
			b = lookahead_char_utf32_pop(lc, &lc->la0_32);
			if (a && b) lc->la_size++;
		}
		if (lc->la_size == 1) {
			a = lookahead_char_utf8_pop(lc, lc->la1_8);
			b = lookahead_char_utf32_pop(lc, &lc->la1_32);
			if (a && b) lc->la_size++;
		}
	}

	return lookahead_char_done_loading(lc);
}

/**
 * pop character off of lookahead
 * @param lc the lookahead_char struct pointer
 */
void lookahead_char_pop(struct lookahead_char* lc)
{
	if (lc->la_size > 0) {
		char* temp8 = lc->lb_8;
		UChar32 temp32 = lc->lb_32;

		/* lookbehind = lookahead0 */
		lc->lb_8 = lc->la0_8;
		lc->lb_32 = lc->la0_32;

		/* lookahead0 = lookahead1 */
		lc->la0_8 = lc->la1_8;
		lc->la0_32 = lc->la1_32;

		/* lookahead1 = lookahead2 */
		lc->la1_8 = lc->la2_8;
		lc->la1_32 = lc->la2_32;

		/* lookahead2 = lookbehind */
		lc->la2_8 = temp8;
		lc->la2_32 = temp32;

		/* lookahead and lookbehind sizes */
		if (lc->la_size > 0) lc->lb_size = 1;
		else lc->lb_size = 0;
		if (lc->la_size > 0) lc->la_size--;

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

/**
 * put the previous character into the lookahead
 * @param lc the lookahead_char struct pointer
 */
void lookahead_char_push(struct lookahead_char* lc)
{
	if (lc->lb_size >= 1) {
		char* temp8 = lc->la2_8;
		UChar32 temp32 = lc->la2_32;

		/* lookahead2 = lookahead1 */
		lc->la2_8 = lc->la1_8;
		lc->la2_32 = lc->la1_32;

		/* lookahead1 = lookahead0 */
		lc->la1_8 = lc->la0_8;
		lc->la1_32 = lc->la0_32;

		/* lookahead0 = lookbehind */
		lc->la0_8 = lc->lb_8;
		lc->la0_32 = lc->lb_32;

		/* lookbehind = lookahead2 */
		lc->lb_8 = temp8;
		lc->lb_32 = temp32;

		/* lookahead and lookbehind sizes */
		lc->la_size++;
		lc->lb_size = 0;

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
