#include "input.h"
#include "parse_tools.h"
#include "token.h"
#include "result.h"
#include "scan.h"
#include <string.h>
#include "source.h"

void parse_state_init(struct parse_state* ps, struct scan_state* sns)
{
	ps->sns = sns;
	token_list_init(&ps->lookahead);
}

/* get lookahead token */
enum parse_result get_lookahead(struct allocator* al, struct parse_state* ps, int count, int* num)
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

		r = scan_get_token(al, ps->sns, &gt, &t);
		if (r == result_error) {
			return r;
		}
		if (!gt) {
			break;
		}

		r = token_list_add(al, &ps->lookahead, t);
		if (r == result_error) {
			return r;
		}
	}

	return r;
}

/* expecting specific token */
enum result match(struct allocator* al, struct parse_state* ps, enum token_type type, char* reason, struct token** t)
{
	enum result r = result_ok;

	int num = token_list_count(&ps->lookahead);
	int got_token;
	if (num <= 0) {
		enum result r = scan_get_token(al, ps->sns, &got_token, t);
		if (r == result_error) {
			return r;
		}
		if (!got_token) {
			return set_source_error(NULL, ps->sns->lc, "%s", reason);
		}
		r = token_list_add(al, &ps->lookahead, *t);
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

	return set_source_error(*t, ps->sns->lc, "%s", reason);
}
