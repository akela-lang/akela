#include "parse_data.h"
#include "lex.h"
#include <stdbool.h>
#include "zinc/error.h"
#include "ast.h"

void Worm_lookahead(Worm_parse_data* pd)
{
    if (!pd->lookahead) {
        pd->lookahead = Worm_lex(pd->ld);
    }
}

bool Worm_match(Worm_parse_data* pd,
    Worm_token_type type,
    char const* message,
    Worm_token** t,
    Worm_ast* n)
{
    if (!pd->lookahead) {
        pd->lookahead = Worm_lex(pd->ld);
    }

    if (pd->lookahead->type == type) {
        *t = pd->lookahead;
        Zinc_location_combine(&n->loc, &(*t)->loc);
        return true;
    }

    n->has_error = true;

    return false;
}
