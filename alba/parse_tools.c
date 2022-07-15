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
enum parse_result get_lookahead(struct parse_state* ps, int count, int* num)
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
		r = token_list_add(&ps->lookahead, t);
		if (r == result_error) {
			return r;
		}
	}

	return r;
}

/* expecting specific token */
/* dynamic-output ps{} t t{} */
enum result match(struct parse_state* ps, enum token_type type, char* reason, struct token** t)
{
	enum result r = result_ok;

	int num = token_list_count(&ps->lookahead);
	int got_token;
	if (num <= 0) {
		/* allocate ps{sns{wt{} el{}}} t t{}*/
		enum result r = scan_get_token(ps->sns, &got_token, t);
		if (r == result_error) {
			return r;
		}
		if (!got_token) {
			/* dynamic-output ps{el{}} */
			return set_source_error(ps->el, NULL, ps->sns->lc, "%s", reason);
		}
		/* transfer t t{} -> ps{lookahead{}} */
		r = token_list_add(&ps->lookahead, *t);
		if (r == result_error) {
			return r;
		}
	} else {
		*t = ps->lookahead.head;
	}

	if ((*t)->type == type) {
		token_list_pop(&ps->lookahead);
		return r;
	}

	/* alocate ps{el{}} */
	return set_source_error(ps->el, *t, ps->sns->lc, "%s", reason);
}
