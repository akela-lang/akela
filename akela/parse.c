#include <stdlib.h>
#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "ast.h"
#include "parse_stmts.h"
#include "lex.h"
#include "parse.h"
#include "parse_tools.h"
#include <assert.h>

Ast_node* parse(struct parse_state* ps)
{
    Ast_node* n = NULL;

	struct location loc_stmts;
    n = parse_stmts(ps, true, &loc_stmts);

    struct location next_loc;
    get_location(ps, &next_loc);
    struct token* t0 = get_lookahead(ps);
    assert(t0);
	if (t0->type != token_eof) {
		char* names[token_count];
		enum result r = token_name_init(names);
		if (r == result_error) {
			error_list_set(ps->el, &next_loc, "token name init: %s", error_message);
            n->type = Ast_type_error;
            token_destroy(t0);
            free(t0);
			return n;
		}

		error_list_set(ps->el, &next_loc, "Couldn't process token: %s", names[t0->type]);
        n->type = Ast_type_error;
 	}

    token_destroy(t0);
    free(t0);

	return n;
}
