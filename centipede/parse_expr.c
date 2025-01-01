#include "ast.h"
#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>
#include "parse_object_stmts.h"

Cent_ast* Cent_parse_expr_number(Cent_parse_data* pd);
Cent_ast* Cent_parse_expr_string(Cent_parse_data* pd);
Cent_ast* Cent_parse_expr_boolean(Cent_parse_data* pd);
Cent_ast* Cent_parse_expr_id(Cent_parse_data* pd);
void Cent_parse_expr_enum(Cent_parse_data* pd, Cent_token* id, Cent_ast* n);
void Cent_parse_expr_assign(Cent_parse_data* pd, Cent_token* id, Cent_ast* n);
void Cent_parse_expr_object(Cent_parse_data* pd, Cent_token* id, Cent_ast* n);
void Cent_parse_expr_builtin_function(Cent_parse_data* pd, Cent_token* id, Cent_ast* n);
void Cent_parse_expr_variable(Cent_parse_data* pd, Cent_token* id, Cent_ast* n);

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_expr(Cent_parse_data* pd)
{
    Cent_lookahead(pd);

    if (pd->lookahead->type == Cent_token_number) {
        return Cent_parse_expr_number(pd);
    }

    if (pd->lookahead->type == Cent_token_string) {
        return Cent_parse_expr_string(pd);
    }

    if (pd->lookahead->type == Cent_token_true || pd->lookahead->type == Cent_token_false) {
        return Cent_parse_expr_boolean(pd);
    }

    return Cent_parse_expr_id(pd);
}

Cent_ast* Cent_parse_expr_number(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_expr_number;
    Cent_ast_value_set_type(n, Cent_value_type_number);

    Cent_token* num = NULL;
    if (!Cent_match(pd, Cent_token_number, "expected number", &num, n)) {
        assert(false && "not possible");
    }
    if (num) {
        if (num->number_type == Cent_number_type_integer) {
            n->number_type = Cent_number_type_integer;
            n->data.integer = num->number_value.integer;
        } else if (num->number_type == Cent_number_type_fp) {
            n->number_type = Cent_number_type_fp;
            n->data.fp = num->number_value.fp;
        } else {
            assert(false && "not possible");
        }

        Cent_token_destroy(num);
        free(num);
    }

    return n;
}

Cent_ast* Cent_parse_expr_string(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_expr_string;

    Cent_token* str = NULL;
    if (!Cent_match(pd, Cent_token_string, "expected string", &str, n)) {
        assert(false && "not possible");
    }

    Cent_ast_value_set_type(n, Cent_value_type_string);
    buffer_copy(&str->value, &n->data.string);
    Cent_token_destroy(str);
    free(str);

    return n;
}

Cent_ast* Cent_parse_expr_boolean(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_expr_boolean;

    Cent_ast_value_set_type(n, Cent_value_type_boolean);
    Cent_token* bln = NULL;
    if (!Cent_match(pd, pd->lookahead->type, "expected boolean", &bln, n)) {
        assert(false && "not possible");
    }

    if (bln->type == Cent_token_true) {
        n->data.boolean = true;
    } else if (bln->type == Cent_token_false) {
        n->data.boolean = false;
    } else {
        assert(false && "not possible");
    }
    Cent_token_destroy(bln);
    free(bln);

    return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_expr_id(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);

    Cent_token* id = NULL;
    if (!Cent_match(pd, Cent_token_id, "expected id", &id, n)) {
        Cent_token_destroy(id);
        free(id);
        n->has_error = true;
        return n;
    }
    /* test case: test_parse_value_error_expected_id */

    Cent_lookahead(pd);

    if (pd->lookahead->type == Cent_token_double_colon) {
        Cent_parse_expr_enum(pd, id, n);
        return n;
    }

    if (pd->lookahead->type == Cent_token_equal) {
        Cent_parse_expr_assign(pd, id, n);
        return n;
    }

    if (pd->lookahead->type == Cent_token_left_curly_brace) {
        Cent_parse_expr_object(pd, id, n);
        return n;
    }

    if (pd->lookahead->type == Cent_token_left_paren) {
        Cent_parse_expr_builtin_function(pd, id, n);
        return n;
    }

    Cent_parse_expr_variable(pd, id, n);
    return n;
}

void Cent_parse_expr_enum(Cent_parse_data* pd, Cent_token* id, Cent_ast* n)
{
    n->type = Cent_ast_type_expr_enum;
    Cent_ast_value_set_type(n, Cent_value_type_enum);
    if (id) {
        buffer_copy(&id->value, &n->data.enumeration.id1);
        buffer_copy(&id->value, &n->data.enumeration.display);
        n->data.enumeration.loc1 = id->loc;
    } else {
        n->has_error = true;
    }

    Cent_token* dc = NULL;
    if (!Cent_match(pd, Cent_token_double_colon, "expected double-colon", &dc, n)) {
        assert(false && "not possible");
    }
    buffer_copy_str(&n->data.enumeration.display, "::");
    Cent_token_destroy(dc);
    free(dc);

    Cent_token* id2 = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id2, n);
    /* test case: test_parse_value_error_enum_expected_id */

    if (id2) {
        buffer_copy(&id2->value, &n->data.enumeration.id2);
        buffer_copy(&id2->value, &n->data.enumeration.display);
        n->data.enumeration.loc2 = id2->loc;
    } else {
        n->has_error = true;
    }

    Cent_token_destroy(id);
    free(id);
    Cent_token_destroy(id2);
    free(id2);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_parse_expr_assign(Cent_parse_data* pd, Cent_token* id, Cent_ast* n)
{
    n->type = Cent_ast_type_expr_assign;

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
    if (b->type == Cent_ast_type_expr_assign) {
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
        Cent_symbol_set_type(sym, Cent_symbol_type_variable);
        sym->data.variable.n = b;
        Cent_environment_add_symbol(pd->top, &a->text, sym);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_parse_expr_object(Cent_parse_data* pd, Cent_token* id, Cent_ast* n)
{
    n->type = Cent_ast_type_expr_object;

    buffer_copy(&id->value, &n->text);
    Cent_token_destroy(id);
    free(id);

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

void Cent_parse_expr_builtin_function(Cent_parse_data* pd, Cent_token* id, Cent_ast* n)
{
    if (id->builtin_type == Cent_builtin_type_top) {
        n->type = Cent_ast_type_expr_function_top;
    } else if (id->builtin_type == Cent_builtin_type_file_name) {
        n->type = Cent_ast_type_expr_function_file_name;
    } else {
        n->type = Cent_ast_type_none;
        if (id->builtin_type == Cent_builtin_type_none) {
            error_list_set(pd->errors, &id->loc, "invalid builtin function");
            n->has_error = true;
        }
    }
    Cent_token_destroy(id);
    free(id);

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

    Cent_token* rp = NULL;
    Cent_match(pd, Cent_token_right_paren, "expected right paren", &rp, n);
    Cent_token_destroy(rp);
    free(rp);
}

void Cent_parse_expr_variable(Cent_parse_data* pd, Cent_token* id, Cent_ast* n)
{
    n->type = Cent_ast_type_expr_variable;
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
        } else if (sym->type != Cent_symbol_type_variable) {
            error_list_set(pd->errors, &a->loc, "id is not a value");
            n->has_error = true;
        }
    }
}