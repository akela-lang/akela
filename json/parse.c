#include <cobble/compile_tools.h>
#include "dom.h"
#include "parse_tools.h"
#include <assert.h>

Json_dom* Json_parse_value(Json_parse_data* pd);
Json_dom* Json_parse_string(Json_parse_data* pd);

Json_dom* Json_parse(Json_parse_data* pd)
{
    return Json_parse_value(pd);
}

Json_dom* Json_parse_value(Json_parse_data* pd)
{
    Json_get_lookahead(pd);

    if (pd->lookahead->type == Json_token_type_string) {
        return Json_parse_string(pd);
    }

    if (pd->lookahead->type == Json_token_type_eof) {
        return NULL;
    }

    assert(false && "invalid token type");
}

Json_dom* Json_parse_string(Json_parse_data* pd)
{
    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    dom->type = Json_token_type_string;

    Json_token* s = NULL;
    if (!Json_match(pd, Json_token_type_string, &s, dom)) {
        assert(false && "not possible");
    }
    buffer_copy(&s->value, &dom->value.string);

    return dom;
}