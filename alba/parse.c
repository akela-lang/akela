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
#include "source.h"
#include "lookahead_char.h"

enum result parse(struct allocator* al, struct parse_state* ps, struct dag_node** root)
{
	*root = NULL;
	enum result r;

	r = stmts(al, ps, root);

	if (r == result_error) {
		return r;
	}

	if (!lookahead_char_done(ps->sns->lc)) {
		int num;
		r = get_lookahead(al, ps, 1, &num);
		if (r == result_error) {
			return r;
		}
	}

	if (token_list_count(&ps->lookahead) > 0) {
		struct token* t = ps->lookahead.head;
		char* names[token_count];
		r = token_name_init(names);
		return set_source_error(t, ps->sns->lc, "Couldn't process token: %s", names[t->type]);
	}

	return result_ok;
}
