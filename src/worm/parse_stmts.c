#include "parse_data.h"
#include "ast.h"
#include <assert.h>
#include "parse_tools.h"

bool Worm_parse_sep(Worm_parse_data* pd, Worm_ast* n);

/* stmts => opt-seps stmts' */
/* stmts' => stmt seps stmts' | stmt | e */
Worm_ast* Worm_parse_stmts(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_stmts;

    if (Worm_parse_sep(pd, n)) {
        Worm_parse_sep(pd, n);
    }

    return n;
}

bool Worm_parse_sep(Worm_parse_data* pd, Worm_ast* n)
{
    Worm_lookahead(pd);
    if (pd->lookahead->type == Worm_token_type_newline || pd->lookahead->type == Worm_token_type_semicolon) {
        Worm_token_type type = pd->lookahead->type;
        Worm_token* t = NULL;
        if (!Worm_match(pd, type, "expected separator", &t, n)) {
            assert(false && "not possible");
        }
        Worm_token_destroy(t);
        free(t);
        return true;
    }

    return false;
}
