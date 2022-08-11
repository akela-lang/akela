#include <stdlib.h>
#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "dag.h"
#include "parse_stmts.h"
#include "scan.h"
#include "parse.h"
#include "input.h"
#include "parse_tools.h"
#include "source.h"
#include "lookahead_char.h"

/* dynamic-output ps{} root root{} */
bool parse(struct parse_state* ps, struct dag_node** root)
{
	*root = NULL;
	bool valid = true;

	/* allocate ps{} root root{} */
	valid = stmts(ps, false, root) && valid;

	/* allocate ps{} */
	if (!lookahead_char_done(ps->sns->lc)) {
		int num;
		valid = get_lookahead(ps, 1, &num) && valid;
	}

	if (token_list_count(&ps->lookahead) > 0) {
		struct token* t = ps->lookahead.head;
		char* names[token_count];
		enum result r = token_name_init(names);
		struct location loc;
		if (r == result_error) {
			get_token_location(t, &loc);
			valid = set_source_error(ps->el, &loc, "token name init: %s", error_message);
			return valid;
		}

		/* allocate ps{} */
		get_parse_location(ps, &loc);
		valid = set_source_error(ps->el, &loc, "Couldn't process token: %s", names[t->type]);
	}

	return valid;
}
