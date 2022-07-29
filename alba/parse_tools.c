#include <string.h>
#include <stdbool.h>
#include "input.h"
#include "parse_tools.h"
#include "token.h"
#include "zinc/result.h"
#include "scan.h"
#include "source.h"

/* dynamic-output-none */
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
bool get_lookahead(struct parse_state* ps, int count, int* num)
{
	bool valid = true;
	int gt;
	struct token* t;

	*num = token_list_count(&ps->lookahead);
	if (*num >= count) {
		return valid;
	}

	for (; *num < count; (*num)++) {
		if (lookahead_char_done(ps->sns->lc)) {
			break;
		}

		/* allocate sns{wt{} el{}} t t{}*/
		valid = scan_get_token(ps->sns, &gt, &t);
		if (!valid) {
			return valid;
		}
		if (!gt) {
			break;
		}

		/* transfer t t{} -> ps{lookahead{}} */
		token_list_add(&ps->lookahead, t);
	}

	return valid;
}

/* expecting specific token */
/* dynamic-output ps{} t t{} */
bool match(struct parse_state* ps, enum token_type type, char* reason, struct token** t)
{
	bool valid = true;
	struct location loc;

	int num = token_list_count(&ps->lookahead);
	int got_token;
	if (num <= 0) {
		/* allocate ps{sns{wt{} el{}}} t t{}*/
		valid = valid && scan_get_token(ps->sns, &got_token, t);
		if (!valid) {
			return valid;
		}
		if (!got_token) {
			get_scan_location(ps->sns, &loc);
			/* dynamic-output ps{el{}} */
			set_source_error(ps->el, &loc, "%s", reason);
			valid = false;
			return valid;
		}
		/* transfer t t{} -> ps{lookahead{}} */
		token_list_add(&ps->lookahead, *t);
	} else {
		*t = ps->lookahead.head;
	}

	if ((*t)->type == type) {
		token_list_pop(&ps->lookahead);
		return valid;
	}

	valid = false;
	get_token_location(*t, &loc);
	/* alocate ps{el{}} */
	set_source_error(ps->el, &loc, "%s", reason);
	*t = NULL;
	return valid;
}

/* dynamic-output ps{} */
bool get_parse_location(struct parse_state* ps, struct location* loc)
{
	bool valid = true;
	if (!ps->lookahead.head) {
		/* allocate ps{} */
		int num;
		valid = valid && get_lookahead(ps, 1, &num);
	}

	struct token* t = get_token(&ps->lookahead, 0);

	if (!valid || !t) {
		loc->line = ps->sns->lc->line;
		loc->col = ps->sns->lc->col;
		loc->byte_pos = ps->sns->lc->byte_pos;
		return valid;
	}

	loc->line = t->line;
	loc->col = t->col;
	loc->byte_pos = t->byte_pos;

	return valid;
}
