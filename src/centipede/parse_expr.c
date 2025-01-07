#include "ast.h"
#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>
#include "parse_object_stmts.h"

Cent_ast* Cent_parse_assign(Cent_parse_data* pd);
void Cent_parse_check_lvalue(Cent_parse_data* pd, Cent_ast* n);
void Cent_parse_check_rvalue(Cent_parse_data* pd, Cent_ast* n);
Cent_ast* Cent_parse_namespace(Cent_parse_data* pd);
Cent_ast* Cent_parse_factor(Cent_parse_data* pd);
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
    return Cent_parse_assign(pd);
}

/* assign -> namespace = assign | namespace */
/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_assign(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast* a = NULL;
    Cent_ast* b = NULL;

    a = Cent_parse_namespace(pd);
    if (!a) {
        return NULL;
    }

    while (true) {
        Cent_lookahead(pd);
        if (pd->lookahead->type != Cent_token_equal) {
            break;
        }

        if (!n) {
            Cent_ast_create(&n);
            n->type = Cent_ast_type_expr_assign;
            Cent_ast_add(n, a);
        }

        Cent_token* eq = NULL;
        if (!Cent_match(pd, Cent_token_equal, "expected equal", &eq, n)) {
            assert(false && "not possible");
        }
        Cent_token_destroy(eq);
        free(eq);

        b = Cent_parse_namespace(pd);
        if (b) {
            Cent_ast_add(n, b);
        } else {
            error_list_set(pd->errors, &pd->lookahead->loc, "expected expression");
            n->has_error = true;
            break;
        }
    }

    if (!n) {
        n = a;
    }

    if (n && !n->has_error) {
        if (n->type == Cent_ast_type_expr_assign) {
            if (Cent_ast_count(n) > 2) {
                error_list_set(pd->errors, &n->loc, "nested assignments are not allowed");
                n->has_error = true;
            } else {
                Cent_ast* left_node = Cent_ast_get(n, 0);
                Cent_ast* right_node = Cent_ast_get(n, 1);
                Cent_parse_check_lvalue(pd, left_node);
                Cent_parse_check_rvalue(pd, right_node);

                if (!n->has_error) {
                    Cent_symbol* sym = NULL;
                    Cent_symbol_create(&sym);
                    Cent_symbol_set_type(sym, Cent_symbol_type_variable);
                    sym->data.variable.n = b;
                    Cent_environment_add_symbol(pd->top, &a->text, sym);
                }
            }
        } else {
            Cent_parse_check_rvalue(pd, n);
        }
    }

    return n;
}

void Cent_parse_check_lvalue(Cent_parse_data* pd, Cent_ast* n)
{
    if (n) {
        if (n->type == Cent_ast_type_id) {
            Cent_symbol* sym = Cent_environment_get(pd->top, &n->text);
            if (sym) {
                if (sym->type == Cent_symbol_type_variable) {
                    error_list_set(pd->errors, &n->loc, "shadowing of variable", &n->text);
                    n->has_error = true;
                } else {
                    error_list_set(pd->errors, &n->loc, "invalid variable name", &n->text);
                    n->has_error = true;
                }
            }
        }
    }
}

void Cent_parse_check_rvalue(Cent_parse_data* pd, Cent_ast* n)
{
    if (n) {
        if (n->type == Cent_ast_type_id) {
            n->type = Cent_ast_type_expr_variable;
        }
        if (n->type == Cent_ast_type_expr_variable) {
            Cent_symbol* sym = Cent_environment_get(pd->top, &n->text);
            if (!sym) {
                error_list_set(pd->errors, &n->loc, "variable does not exist: %b", &n->text);
                n->has_error = true;
            } else if (sym->type != Cent_symbol_type_variable) {
                error_list_set(pd->errors, &n->loc, "id is not a value");
                n->has_error = true;
            }
        }
    }
}

/* namespace -> namespace :: factor */
/* convert left recursion to right recursion */
/* namespace -> factor namespace' */
/* namespace' -> :: factor namespace' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_namespace(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast* a = NULL;
    Cent_ast* b = NULL;

    a = Cent_parse_factor(pd);

    Cent_lookahead(pd);
    while (pd->lookahead->type == Cent_token_double_colon) {
        if (!n) {
            if (a->type != Cent_ast_type_id) {
                error_list_set(pd->errors, &a->loc, "expected id");
                a->has_error = true;
            }

            Cent_ast_create(&n);
            n->type = Cent_ast_type_namespace;
            Cent_ast_add(n, a);
        }

        Cent_token* dc = NULL;
        if (!Cent_match(pd, Cent_token_double_colon, "expected double colon", &dc, n)) {
            assert(false && "not possible");
        }

        b = Cent_parse_factor(pd);
        if (b->type != Cent_ast_type_id) {
            error_list_set(pd->errors, &b->loc, "expected id");
            b->has_error = true;
        }

        Cent_ast_add(n, b);

        Cent_lookahead(pd);
    }

    if (!n) {
        n = a;
    }

    if (!n->has_error) {
    }

    return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_factor(Cent_parse_data* pd)
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

    if (pd->lookahead->type == Cent_token_id) {
        return Cent_parse_expr_id(pd);
    }

    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    error_list_set(pd->errors, &pd->lookahead->loc, "expected factor");
    n->has_error = true;

    return n;
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
    if (id->builtin_type == Cent_builtin_type_file_name) {
        n->type = Cent_ast_type_expr_function_file_name;
    } else {
        n->type = Cent_ast_type_none;
        if (id->builtin_type == Cent_builtin_type_none) {
            error_list_set(pd->errors, &id->loc, "invalid builtin function");
            n->has_error = true;
        }
    }

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

    Cent_token_destroy(id);
    free(id);

    Cent_token* rp = NULL;
    Cent_match(pd, Cent_token_right_paren, "expected right paren", &rp, n);
    Cent_token_destroy(rp);
    free(rp);
}

void Cent_parse_expr_variable(Cent_parse_data* pd, Cent_token* id, Cent_ast* n)
{
    n->type = Cent_ast_type_id;
    buffer_copy(&id->value, &n->text);
    Cent_token_destroy(id);
    free(id);
}