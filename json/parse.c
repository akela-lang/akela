#include <cobble/compile_tools.h>
#include "dom.h"
#include "parse_tools.h"
#include <assert.h>

Json_dom* Json_parse_value(Json_parse_data* pd);
Json_dom* Json_parse_string(Json_parse_data* pd);
Json_dom* Json_parse_number(Json_parse_data* pd);
Json_dom* Json_parse_array(Json_parse_data* pd);
void Json_parse_array_seq(Json_parse_data* pd, Json_dom* parent);
Json_dom* Json_parse_bool(Json_parse_data* pd);

Json_dom* Json_parse(Json_parse_data* pd)
{
    return Json_parse_value(pd);
}

bool Json_parse_is_valid(Json_parse_data* pd, Json_dom* dom)
{
    return !dom->has_error && !pd->el->head;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
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

    if (pd->lookahead->type == Json_token_type_left_square_bracket) {
        return Json_parse_array(pd);
    }

    if (pd->lookahead->type == Json_token_type_true || pd->lookahead->type == Json_token_type_false) {
        return Json_parse_bool(pd);
    }

    return NULL;
}

Json_dom* Json_parse_bool(Json_parse_data* pd)
{
    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_boolean);

    Json_get_lookahead(pd);
    Json_token_type type = pd->lookahead->type;

    if (type == Json_token_type_true) {
        Json_token* t = NULL;
        if (!Json_match(pd, type, &t, dom)) {
            assert(false && "not possible");
        }
        dom->value.boolean = true;
        Json_token_destroy(t);
        free(t);
    } else if (type == Json_token_type_false) {
        Json_token* f = NULL;
        if (!Json_match(pd, type, &f, dom)) {
            assert(false && "not possible");
        }
        dom->value.boolean = false;
        Json_token_destroy(f);
        free(f);
    } else {
        assert(false && "not possible");
    }

    return dom;
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

/* array -> [ array_seq ] */
/* NOLINTNEXTLINE(misc-no-recursion) */
Json_dom* Json_parse_array(Json_parse_data* pd)
{
    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_array);

    Json_token* lsb = NULL;
    if (!Json_match(pd, Json_token_type_left_square_bracket, &lsb, dom)) {
        assert(false && "not possible");
    }
    Json_token_destroy(lsb);
    free(lsb);

    Json_parse_array_seq(pd, dom);

    Json_token* rsb = NULL;
    if (!Json_match(pd, Json_token_type_right_square_bracket, &rsb, dom)) {
        error_list_set(pd->el, &pd->lookahead->loc, "expected right square bracket");
        dom->has_error = true;
    }
    Json_token_destroy(rsb);
    free(rsb);

    return dom;
}

/* array_seq -> value array_seq' | e */
/* array_seq' -> , value array_seq' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
void Json_parse_array_seq(Json_parse_data* pd, Json_dom* parent)
{
    Json_dom* a = Json_parse_value(pd);
    if (a) {
        Json_dom_add(parent, a);
    } else {
        return;
    }

    Json_get_lookahead(pd);
    while (pd->lookahead->type == Json_token_type_comma) {
        Json_token* comma = NULL;
        if (!Json_match(pd, Json_token_type_comma, &comma, a)) {
            assert(false && "not_possible");
        }
        Json_token_destroy(comma);
        free(comma);

        Json_dom* b = Json_parse_value(pd);
        if (b) {
            Json_dom_add(parent, b);
        } else {
            error_list_set(pd->el, &pd->lookahead->loc, "expected value");
            parent->has_error = true;
        }

        Json_get_lookahead(pd);
    }
}