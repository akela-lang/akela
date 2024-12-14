#include <cobble/compile_tools.h>
#include "dom.h"
#include "parse_tools.h"
#include <assert.h>

Json_dom* Json_parse_value(Json_parse_data* pd);
Json_dom* Json_parse_string(Json_parse_data* pd);
Json_dom* Json_parse_number(Json_parse_data* pd);

Json_dom* Json_parse(Json_parse_data* pd)
{
    return Json_parse_value(pd);
}

bool Json_parse_is_valid(Json_parse_data* pd, Json_dom* dom)
{
    return !dom->has_error && !pd->el->head;
}


Json_dom* Json_parse_value(Json_parse_data* pd)
{
    Json_get_lookahead(pd);

    if (pd->lookahead->type == Json_token_type_string) {
        return Json_parse_string(pd);
    }

    if (pd->lookahead->type == Json_token_type_number) {
        return Json_parse_number(pd);
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
    Json_dom_set_type(dom, Json_dom_type_string);

    Json_token* s = NULL;
    if (!Json_match(pd, Json_token_type_string, &s, dom)) {
        assert(false && "not possible");
    }
    buffer_copy(&s->value, &dom->value.string);

    return dom;
}

Json_dom* Json_parse_number(Json_parse_data* pd)
{
    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_number);

    Json_token* n = NULL;
    if (!Json_match(pd, Json_token_type_number, &n, dom)) {
        assert(false && "not possible");
    }

    if (n->number_type == Json_dom_number_type_integer) {
        dom->number_type = Json_dom_number_type_integer;
        dom->value.integer = n->number.integer;
    } else if (n->number_type == Json_dom_number_type_fp) {
        dom->number_type = Json_dom_number_type_fp;
        dom->value.fp = n->number.fp;
    } else {
        assert(false && "invalid number type");
    }

    return dom;
}