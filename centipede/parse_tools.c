#include <stdbool.h>
#include "parse_data.h"
#include "ast.h"
#include "lex.h"

void Cent_lookahead(Cent_parse_data* pd)
{
    if (!pd->lookahead) {
        pd->lookahead = lex(pd->ld);
    }
}

bool Cent_match(Cent_parse_data* pd, Cent_token_type type, char* message, Cent_token** t, Cent_ast* n)
{
    Cent_lookahead(pd);

    if (pd->lookahead->type == type) {
        location_combine(&n->loc, &pd->lookahead->loc);
        *t = pd->lookahead;
        pd->lookahead = NULL;
        return true;
    }

    error_list_set(pd->errors, &pd->lookahead->loc, message);
    return false;
}