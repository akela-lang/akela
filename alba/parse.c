#include <stdlib.h>
#include "result.h"
#include "allocator.h"
#include "token.h"
#include "dag.h"
#include "parse_stmts.h"
#include "scan.h"
#include "parse.h"
#include "input.h"
#include "parse_tools.h"

enum result parse(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	*root = NULL;
	enum result r;

	r = stmts(al, ts, root);

	if (r == result_error) {
		return r;
	}

	if (!ts->is->done) {
		int num;
		r = get_lookahead(al, ts, 1, &num);
		if (r == result_error) {
			return r;
		}
	}

	if (token_list_count(&ts->lookahead) > 0) {
		struct token* t = ts->lookahead.head->t;
		char* names[token_count];
		r = token_name_init(names);
		return set_error("%d, %d: Couldn't process token: %s", t->line, t->col, names[ts->lookahead.head->t->type]);
	}

	return result_ok;
}
