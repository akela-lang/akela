#include "ast.h"
#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>
#include "parse_object_stmts.h"

void Cent_parse_number(Cent_parse_data* pd, Cent_ast* n);
void Cent_parse_string(Cent_parse_data* pd, Cent_ast* n);
void Cent_parse_boolean(Cent_parse_data* pd, Cent_ast* n);
void Cent_parse_object(Cent_parse_data* pd, Cent_ast* n);

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_expr(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_value;

    Cent_lookahead(pd);

    /* number */
    if (pd->lookahead->type == Cent_token_number) {
        Cent_parse_number(pd, n);
        return n;
    }

    /* string */
    if (pd->lookahead->type == Cent_token_string) {
        Cent_parse_string(pd, n);
        return n;
    }

    /* boolean */
    if (pd->lookahead->type == Cent_token_true || pd->lookahead->type == Cent_token_false) {
        Cent_parse_boolean(pd, n);
        return n;
    }

    Cent_token* id = NULL;
    if (!Cent_match(pd, Cent_token_id, "expected id", &id, n)) {
        Cent_token_destroy(id);
        free(id);
        return n;
    }
    /* test case: test_parse_value_error_expected_id */

    /* enum value */
    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_double_colon) {
        Cent_value* v = NULL;
        Cent_value_create(&v);
        Cent_value_set_type(v, Cent_value_type_enum);
        if (id) {
            buffer_copy(&id->value, &v->display);
            Cent_token_destroy(id);
            free(id);
        } else {
            v->has_error = true;
        }

        Cent_token* dc = NULL;
        if (!Cent_match(pd, Cent_token_double_colon, "expected double-colon", &dc, n)) {
            assert(false && "not possible");
        }
        buffer_copy_str(&v->display, "::");
        Cent_token_destroy(dc);
        free(dc);

        Cent_token* id2 = NULL;
        Cent_match(pd, Cent_token_id, "expected id", &id2, n);
        /* test case: test_parse_value_error_enum_expected_id */

        if (id2) {
            buffer_copy(&id2->value, &v->display);
            Cent_token_destroy(id2);
            free(id2);
        } else {
            v->has_error = true;
        }

        n->value = v;
        return n;
    }

    /* assignment */
    if (pd->lookahead->type == Cent_token_equal) {
        n->type = Cent_ast_type_assign;

        Cent_ast* a = NULL;
        Cent_ast_create(&a);
        a->type = Cent_ast_type_id;
        buffer_copy(&id->value, &a->text);
        Cent_ast_add(n, a);

        Cent_token* eq = NULL;
        if (!Cent_match(pd, Cent_token_equal, "expected equal", &eq, n)) {
            assert(false && "not possible");
        }
        Cent_token_destroy(eq);
        free(eq);

        Cent_ast* b = Cent_parse_expr(pd);
        if (b->type == Cent_ast_type_assign) {
            error_list_set(pd->errors, &b->loc, "nested assignments are not allowed");
            b->has_error = true;
        }
        Cent_ast_add(n, b);
        /* test case: test_parse_value_error_nested_assignments */

        Cent_token_destroy(id);
        free(id);

        if (!n->has_error) {
            Cent_symbol* sym = NULL;
            Cent_symbol_create(&sym);
            sym->type = Cent_symbol_type_value;
            sym->data.value = b->value;
            Cent_environment_add_symbol(pd->top, &a->text, sym);
        }
        return n;
    }

    /* object */
    if (pd->lookahead->type == Cent_token_left_curly_brace) {
        buffer_copy(&id->value, &n->text);
        Cent_parse_object(pd, n);

        Cent_token_destroy(id);
        free(id);
        return n;
    }

    /* builtin function */
    if (pd->lookahead->type == Cent_token_left_paren) {
        Cent_token* lp = NULL;
        if (!Cent_match(pd, Cent_token_left_paren, "expected left paren", &lp, n)) {
            assert(false && "not possible");
        }
        Cent_token_destroy(lp);
        free(lp);

        if (id->builtin_type == Cent_builtin_type_none) {
            error_list_set(
                pd->errors,
                &id->loc,
                "id is not a builtin function which are denoted by an id starting with '@'");
            n->has_error = true;
        }

        if (id->builtin_type == Cent_builtin_type_top) {
            n->type = Cent_ast_type_function_top;
        } else if (id->builtin_type == Cent_builtin_type_file_name) {
            n->type = Cent_ast_type_function_file_name;
        } else {
            n->type = Cent_ast_type_none;
            if (id->builtin_type == Cent_builtin_type_none) {
                error_list_set(pd->errors, &id->loc, "invalid builtin function");
                n->has_error = true;
            }
        }
        Cent_token_destroy(id);
        free(id);

        Cent_token* rp = NULL;
        Cent_match(pd, Cent_token_right_paren, "expected right paren", &rp, n);
        Cent_token_destroy(rp);
        free(rp);

        return n;
    }

    /* variable */
    Cent_ast* a = NULL;
    Cent_ast_create(&a);
    a->type = Cent_ast_type_id;
    buffer_copy(&id->value, &a->text);
    a->loc = id->loc;
    Cent_ast_add(n, a);
    Cent_token_destroy(id);
    free(id);

    if (!n->has_error) {
        Cent_symbol* sym = Cent_environment_get(pd->top, &a->text);
        if (!sym) {
            error_list_set(pd->errors, &a->loc, "invalid id: %b", &a->text);
            n->has_error = true;
        } else if (sym->type != Cent_symbol_type_value) {
            error_list_set(pd->errors, &a->loc, "id is not a value");
            n->has_error = true;
        }
    }

    return n;
}

void Cent_parse_number(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_number);

    Cent_token* num = NULL;
    if (!Cent_match(pd, Cent_token_number, "expected number", &num, n)) {
        assert(false && "not possible");
    }
    if (num) {
        if (num->number_type == Cent_number_type_integer) {
            value->number_type = Cent_number_type_integer;
            value->data.integer = num->number_value.integer;
        } else if (num->number_type == Cent_number_type_fp) {
            value->number_type = Cent_number_type_fp;
            value->data.fp = num->number_value.fp;
        } else {
            assert(false && "not possible");
        }

        Cent_token_destroy(num);
        free(num);
    }

    n->value = value;
}

void Cent_parse_string(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);

    Cent_token* str = NULL;
    if (!Cent_match(pd, Cent_token_string, "expected string", &str, n)) {
        assert(false && "not possible");
    }

    Cent_value_set_type(value, Cent_value_type_string);
    buffer_copy(&str->value, &value->data.string);
    Cent_token_destroy(str);
    free(str);

    n->value = value;
}

void Cent_parse_boolean(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_boolean);
    Cent_token* bln = NULL;
    if (!Cent_match(pd, pd->lookahead->type, "expected boolean", &bln, n)) {
        assert(false && "not possible");
    }

    if (bln->type == Cent_token_true) {
        value->data.boolean = true;
    } else if (bln->type == Cent_token_false) {
        value->data.boolean = false;
    } else {
        assert(false && "not possible");
    }
    Cent_token_destroy(bln);
    free(bln);

    n->value = value;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_parse_object(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_token* lcb = NULL;
    if (!Cent_match(
        pd,
        Cent_token_left_curly_brace,
        "expected left curly brace",
        &lcb,
        n)
    ) {
        assert(false && "not possible");
    }
    Cent_token_destroy(lcb);
    free(lcb);

    Cent_ast* a = Cent_parse_object_stmts(pd);
    Cent_ast_add(n, a);

    Cent_token* rcb = NULL;
    Cent_match(pd, Cent_token_right_curly_brace, "expected right curly brace", &rcb, n);
    Cent_token_destroy(rcb);
    free(rcb);
    /* test case: test_parse_value_error_object_expected_rcb */
}