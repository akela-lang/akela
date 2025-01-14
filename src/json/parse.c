#include <cobble/compile_tools.h>
#include "dom.h"
#include "parse_tools.h"
#include <assert.h>
#include "zinc/hash.h"

Json_dom* Json_parse_value(Json_parse_data* pd);
Json_dom* Json_parse_string(Json_parse_data* pd);
Json_dom* Json_parse_number(Json_parse_data* pd);
Json_dom* Json_parse_array(Json_parse_data* pd);
void Json_parse_array_seq(Json_parse_data* pd, Json_dom* parent);
Json_dom* Json_parse_bool(Json_parse_data* pd);
Json_dom* Json_parse_null(Json_parse_data* pd);
Json_dom* Json_parse_object(Json_parse_data* pd);
void Json_parse_object_seq(Json_parse_data* pd, Json_dom* parent);

Json_dom* Json_parse(Json_parse_data* pd)
{
    Json_dom* root = Json_parse_value(pd);
    Json_get_lookahead(pd);
    if (pd->lookahead->type != Json_token_type_eof) {
        error_list_set(
            pd->el,
            &pd->lookahead->loc,
            "Could not process token: %s",
            Json_token_type_name(pd->lookahead->type));
    }

    Json_token_destroy(pd->lookahead);
    free(pd->lookahead);
    pd->lookahead = NULL;

    return root;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Json_dom* Json_parse_value(Json_parse_data* pd)
{
    Json_get_lookahead(pd);

    if (pd->lookahead->type == Json_token_type_eof) {
        return NULL;
    }

    if (pd->lookahead->type == Json_token_type_string) {
        return Json_parse_string(pd);
    }

    if (pd->lookahead->type == Json_token_type_number) {
        return Json_parse_number(pd);
    }

    if (pd->lookahead->type == Json_token_type_true || pd->lookahead->type == Json_token_type_false) {
        return Json_parse_bool(pd);
    }

    if (pd->lookahead->type == Json_token_type_null) {
        return Json_parse_null(pd);
    }

    if (pd->lookahead->type == Json_token_type_left_square_bracket) {
        return Json_parse_array(pd);
    }

    if (pd->lookahead->type == Json_token_type_left_curly_brace) {
        return Json_parse_object(pd);
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

Json_dom* Json_parse_null(Json_parse_data* pd)
{
    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_null);

    Json_token* n = NULL;
    if (!Json_match(pd, Json_token_type_null, &n, dom)) {
        assert(false && "not possible");
    }
    Json_token_destroy(n);
    free(n);

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
    Zinc_string_copy(&s->value, &dom->value.string);
    Json_token_destroy(s);
    free(s);

    return dom;
}

Json_token* Json_parse_string_token(Json_parse_data* pd, Json_dom* parent)
{
    Json_token* s = NULL;
    if (!Json_match(pd, Json_token_type_string, &s, parent)) {
        assert(false && "not possible");
    }

    return s;
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

    Json_token_destroy(n);
    free(n);

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
        Json_dom_add_element(parent, a);
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
            Json_dom_add_element(parent, b);
        } else {
            error_list_set(pd->el, &pd->lookahead->loc, "expected value");
            parent->has_error = true;
        }

        Json_get_lookahead(pd);
    }
}

/* object -> { object_seq } */
/* NOLINTNEXTLINE(misc-no-recursion) */
Json_dom* Json_parse_object(Json_parse_data* pd)
{
    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_object);

    Json_token* lcb = NULL;
    if (!Json_match(pd, Json_token_type_left_curly_brace, &lcb, dom)) {
        assert(false && "not possible");
    }
    Json_token_destroy(lcb);
    free(lcb);

    Json_parse_object_seq(pd, dom);

    Json_token* rcb = NULL;
    if (!Json_match(pd, Json_token_type_right_curly_brace, &rcb, dom)) {
        error_list_set(pd->el, &pd->lookahead->loc, "expected right curly brace");
        dom->has_error = true;
    }
    Json_token_destroy(rcb);
    free(rcb);

    return dom;
}

/* object_seq -> string : value object_seq' | e */
/* object_seq' -> , string : value object_seq' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
void Json_parse_object_seq(Json_parse_data* pd, Json_dom* parent)
{
    Json_get_lookahead(pd);
    if (pd->lookahead->type != Json_token_type_string) {
        return;
    }

    Json_token* s = Json_parse_string_token(pd, parent);

    Json_token* colon = NULL;
    if (!Json_match(pd, Json_token_type_colon, &colon, parent)) {
        error_list_set(pd->el, &pd->lookahead->loc, "expected colon");
        parent->has_error = true;
    }
    Json_token_destroy(colon);
    free(colon);

    Json_dom* value = Json_parse_value(pd);
    if (value) {
        hash_table_add(&parent->value.object, &s->value, value);
    } else {
        error_list_set(pd->el, &pd->lookahead->loc, "expected value");
        parent->has_error = true;
    }

    Json_token_destroy(s);
    free(s);

    Json_get_lookahead(pd);
    while (pd->lookahead->type == Json_token_type_comma) {
        Json_token* comma = NULL;
        if (!Json_match(pd, Json_token_type_comma, &comma, parent)) {
            assert(false && "not possible");
        }
        Json_token_destroy(comma);
        free(comma);

        Json_get_lookahead(pd);
        if (pd->lookahead->type != Json_token_type_string) {
            error_list_set(pd->el, &pd->lookahead->loc, "expected string");
            parent->has_error = true;
            continue;
        }

        s = Json_parse_string_token(pd, parent);

        colon = NULL;
        if (!Json_match(pd, Json_token_type_colon, &colon, parent)) {
            error_list_set(pd->el, &pd->lookahead->loc, "expected colon");
            parent->has_error = true;
        }
        Json_token_destroy(colon);
        free(colon);

        value = Json_parse_value(pd);
        if (value) {
            hash_table_add(&parent->value.object, &s->value, value);
        } else {
            error_list_set(pd->el, &pd->lookahead->loc, "expected value");
            parent->has_error = true;
        }

        Json_token_destroy(s);
        free(s);
        Json_get_lookahead(pd);
    }
}