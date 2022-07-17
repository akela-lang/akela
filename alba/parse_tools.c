#include "input.h"
#include "parse_tools.h"
#include "token.h"
#include "zinc/result.h"
#include "scan.h"
#include <string.h>
#include "source.h"

/* static-output */
/* initialize-output ps ps{}*/
void parse_state_init(struct parse_state* ps, struct scan_state* sns, struct compile_error_list* el)
{
	ps->sns = sns;
	token_list_init(&ps->lookahead);
	ps->el = el;
}

/* get lookahead token */
/* dynamic ps{} */
/* dynamic-temp t t{} */
enum result get_lookahead(struct parse_state* ps, int count, int* num)
{
	enum result r = result_ok;
	int gt;
	struct token* t;

	*num = token_list_count(&ps->lookahead);
	if (*num >= count) {
		return r;
	}

	for (; *num < count; (*num)++) {
		if (lookahead_char_done(ps->sns->lc)) {
			break;
		}

		/* allocate sns{wt{} el{}} t t{}*/
		r = scan_get_token(ps->sns, &gt, &t);
		if (r == result_error) {
			return r;
		}
		if (!gt) {
			break;
		}

		/* transfer t t{} -> ps{lookahead{}} */
		token_list_add(&ps->lookahead, t);
	}

	return r;
}

/* expecting specific token */
/* dynamic-output ps{} t t{} */
enum result match(struct parse_state* ps, enum token_type type, char* reason, struct token** t)
{
	enum result r = result_ok;
	struct location loc;

	int num = token_list_count(&ps->lookahead);
	int got_token;
	if (num <= 0) {
		/* allocate ps{sns{wt{} el{}}} t t{}*/
		enum result r = scan_get_token(ps->sns, &got_token, t);
		if (r == result_error) {
			return r;
		}
		if (!got_token) {
			get_scan_location(ps->sns, &loc);
			/* dynamic-output ps{el{}} */
			return set_source_error(ps->el, &loc, "%s", reason);
		}
		/* transfer t t{} -> ps{lookahead{}} */
		token_list_add(&ps->lookahead, *t);
	} else {
		*t = ps->lookahead.head;
	}

	if ((*t)->type == type) {
		token_list_pop(&ps->lookahead);
		return r;
	}

	get_token_location(*t, &loc);
	/* alocate ps{el{}} */
	return set_source_error(ps->el, &loc, "%s", reason);
}

/* dynamic-output ps{} */
enum result get_parse_location(struct parse_state* ps, struct location* loc)
{
	enum result r = result_ok;

	if (!ps->lookahead.head) {
		/* allocate ps{} */
		int num;
		r = get_lookahead(ps, 1, &num);
		if (r == result_error) {
			return r;
		}
	}

	struct token* t = get_token(&ps->lookahead, 0);

	if (!t) {
		loc->line = ps->sns->lc->line;
		loc->col = ps->sns->lc->col;
		loc->byte_pos = ps->sns->lc->byte_pos;
		return r;
	}

	loc->line = t->line;
	loc->col = t->col;
	loc->byte_pos = t->byte_pos;

	return r;
}
