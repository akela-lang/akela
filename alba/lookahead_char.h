#ifndef _LOOKAHEAD_CHAR_H
#define _LOOKAHEAD_CHAR_H

#include "alba_api.h"
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdbool.h>
#include <stdlib.h>
#include "input.h"

/* allow 4 utf-8 cells */
#define LOOKAHEAD_SIZE (4*4)
#define TR_IN_SIZE 64
#define TR_OUT_SIZE TR_IN_SIZE
#define LINE_SIZE 100

/* one lookbehind which allows one push and 2 lookahead */
struct lookahead_char {
	char lookahead[LOOKAHEAD_SIZE];
	char* lb_8;	/* lookbehind - will be NULL after push */
	char* la0_8;	/* lookahead 0 */
	char* la1_8;	/* lookahead 1 */
	char* la2_8;	/* lookahead 2 - overflow only used after push */
	UChar32 lb_32;	/* lookbehind - will be unused after push */
	UChar32 la0_32;	/* lookahead 0 */
	UChar32 la1_32;	/* lookahead 1 */
	UChar32 la2_32;	/* lookahead 2 - overflow used after push */
	int lb_size;
	int la_size;
	char tr_in[TR_IN_SIZE];	/* translation input buffer */
	size_t tr_in_size;
	size_t tr_in_pos;
	UChar tr_out[TR_OUT_SIZE];	/* translation output buffer*/
	size_t tr_out_size;
	size_t tr_out_pos;
	input_getchar f;
	input_data d;
	int partial;
	bool has_partial;
	bool done;
	UConverter* conv;
	size_t line;
	size_t col;
	size_t last_col_count;
	size_t byte_pos;
};

/* dynamic-output-none */
/* resource-input conv */
/* resource-output lc{conv}*/
ALBA_API void lookahead_char_init(struct lookahead_char* lc, input_getchar f, input_data d, UConverter* conv);

/* dynamic-output-none */
ALBA_API enum result lookahead_char_get_input(struct lookahead_char* lc);

/* dynamic-output-none */
/* resource-use lc{conv} */
ALBA_API enum result lookahead_char_translate(struct lookahead_char* lc);

/* dynamic-output-none */
ALBA_API bool lookahead_char_has_utf8(struct lookahead_char* lc);

/* dynamic-output-none */
ALBA_API bool lookahead_char_has_utf16(struct lookahead_char* lc);

/* dynamic-output-none */
ALBA_API bool lookahead_char_utf8_pop(struct lookahead_char* lc, char* buf);

/* dynamic-output-none */
ALBA_API bool lookahead_char_utf32_pop(struct lookahead_char* lc, UChar32* c);

/* dynamic-output-none */
ALBA_API bool lookahead_char_done_loading(struct lookahead_char* lc);

/* dynamic-output-none */
ALBA_API bool lookahead_char_done(struct lookahead_char* lc);

/* dynamic-output-none */
ALBA_API bool lookahead_char_need_preping(struct lookahead_char* lc);

/* dynamic-output-none */
ALBA_API bool lookahead_char_need_loading(struct lookahead_char* lc);

/* dynamic-output-none */
/* resource-use lc{conv} */
ALBA_API enum result lookahead_char_prep(struct lookahead_char* lc);

/* dynamic-output-none */
ALBA_API bool lookahead_char_load(struct lookahead_char* lc);

/* dynamic-output-none */
ALBA_API void lookahead_char_pop(struct lookahead_char* lc);

/* dynamic-output-none */
ALBA_API void lookahead_char_push(struct lookahead_char* lc);

#endif
