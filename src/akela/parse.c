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

Ake_ast* Ast_parse(struct Ake_parse_state* ps)
{
    Ake_ast* n = NULL;

    n = Ake_parse_stmts(ps, true);

    struct location next_loc = Ake_get_location(ps);
    struct Ake_token* t0 = Ake_get_lookahead(ps);
    assert(t0);
	if (t0->type != Ake_token_eof) {
		char* names[Ake_token_count];
		enum result r = Ake_token_name_init(names);
		if (r == result_error) {
			error_list_set(ps->el, &next_loc, "token name init: %s", error_message);
            n->type = Ake_ast_type_error;
            Ake_token_destroy(t0);
            free(t0);
			return n;
		}

		error_list_set(ps->el, &next_loc, "Couldn't process token: %s", names[t0->type]);
        n->type = Ake_ast_type_error;
 	}

    Ake_token_destroy(t0);
    free(t0);

	return n;
}
