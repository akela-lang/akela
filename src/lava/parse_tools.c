#include <stdbool.h>
#include "lex.h"
#include "parse_data.h"

void Lava_lookahead(Lava_parse_data* pd)
{
    if (!pd->lookahead) {
        pd->lookahead = Lava_lex(pd->ld);
    }
}

bool Lava_match(Lava_parse_data* pd, Lava_token_kind kind, const char* message, Lava_token** t)
{
    Lava_lookahead(pd);
    if (pd->lookahead->kind == kind) {
        *t = pd->lookahead;
        pd->lookahead = NULL;
        return true;
    }

    *t = NULL;
    Zinc_error_list_set(pd->errors, &pd->lookahead->loc, message);
    return false;
}