#include "parse_tools.h"
#include <cobble/compile_tools.h>
#include "zinc/memory.h"
#include "lex.h"
#include "dom.h"
#include "general.h"

void Json_parse_data_init(Json_parse_data* pd, struct error_list* el, Json_lex_data* ld)
{
    pd->el = el;
    pd->ld = ld;
    pd->lookahead = NULL;
}

void Json_parse_data_create(Json_parse_data** pd, struct error_list* el, Json_lex_data* ld)
{
    malloc_safe((void**)pd, sizeof(Json_parse_data));
    Json_parse_data_init(*pd, el, ld);
}

void Json_get_lookahead(Json_parse_data* pd)
{
    if (pd->lookahead) return;
    Json_token* t = NULL;
    while (!t) {
        t = Json_lex(pd->ld);
    }
    pd->lookahead = t;
}

bool Json_match(Json_parse_data* pd, Json_token_type type, Json_token** t, Json_dom* dom)
{
    Json_get_lookahead(pd);
    if (pd->lookahead->type == type) {
        *t = pd->lookahead;
        pd->lookahead = NULL;
        Json_location_combine(&dom->loc, &(*t)->loc);
        return true;
    }

    return false;
}