#include <stdlib.h>
#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "ast.h"
#include "parse_stmts.h"
#include "scan.h"
#include "parse.h"
#include "input.h"
#include "parse_tools.h"
#include "source.h"
#include "lookahead_char.h"
#include <assert.h>

struct ast_node* parse(struct parse_state* ps)
{
    struct ast_node* n = NULL;

	struct location loc_stmts;
	n = parse_stmts(ps, true, &loc_stmts);

    struct location next_loc;
    get_location(ps, &next_loc);
    struct token* t0 = get_token(&ps->lookahead, 0);
    assert(t0);
	if (t0->type != token_eof) {
		char* names[token_count];
		enum result r = token_name_init(names);
		if (r == result_error) {
			set_source_error(ps->el, &next_loc, "token name init: %s", error_message);
            n->type = ast_type_error;
			return n;
		}

		set_source_error(ps->el, &next_loc, "Couldn't process token: %s", names[t0->type]);
        n->type = ast_type_error;
	}

	return n;
}
