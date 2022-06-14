#include <stdlib.h>
#include "result.h"
#include "allocator.h"
#include "token.h"
#include "dag.h"
#include "stmts.h"
#include "scan.h"
#include "parse.h"
#include "input.h"

enum result parse(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	*root = NULL;
	enum result r = stmts(al, ts, root);
	if (r == result_error) {
		return r;
	}
	return result_ok;
}

void token_state_init(struct input_state* is, struct token_state* ts)
{
	ts->is = is;
	token_list_init(&ts->lookahead);
}

/* get lookahead token */
enum result get_lookahead(struct allocator* al, struct token_state* ts, int count, int* num)
{
	enum result r = result_ok;
	int gt;
	struct token* t;

	*num = token_list_count(&ts->lookahead);
	if (*num >= count) {
		return r;
	}

	for (; *num < count; (*num)++) {
		if (ts->is->done) {
			break;
		}
	
		enum result r = scan_get_token(al, ts->is, &gt, &t);
		if (r == result_error) {
			return r;
		}
		if (!gt) {
			break;
		}
		r = token_list_add(al, &ts->lookahead, t);
		if (r == result_error) {
			return r;
		}
	}

	return r;
}

/* expecting specific token */
enum result match(struct allocator* al, struct token_state* ts, enum token_type type, char* reason)
{
	enum result r = result_ok;

	int num = token_list_count(&ts->lookahead);
	int got_token;
	struct token* t;
	if (num <= 0) {
		enum result r = scan_get_token(al, ts->is, &got_token, &t);
		if (r == result_error) {
			return r;
		}
		if (!got_token) {
			return set_error(reason);
		}
		r = token_list_add(al, &ts->lookahead, t);
		if (r == result_error) {
			return r;
		}
	}

	if (ts->lookahead.head->t->type == type) {
		token_list_pop(&ts->lookahead);
		return r;
	}

	return set_error(reason);
}
