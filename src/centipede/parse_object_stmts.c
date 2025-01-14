#include "ast.h"
#include "parse_data.h"
#include "parse_tools.h"
#include "parse_expr.h"
#include <assert.h>
#include "parse_let.h"

Cent_ast* Cent_parse_object_stmt(Cent_parse_data* pd);
Cent_ast* Cent_parse_property(Cent_parse_data* pd);
void Cent_parse_method_call_seq(Cent_parse_data* pd, Cent_ast* n);

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_object_stmts(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_object_stmts;

    Cent_environment* env = NULL;
    Cent_environment_create(&env);
    env->prev = pd->top;
    pd->top = env;
    n->env = env;

    if (Cent_has_separator(pd, n)) {
        while (Cent_has_separator(pd, n));
    }

    while (true) {
        Cent_lookahead(pd);
        if (pd->lookahead->type == Cent_token_right_curly_brace) {
            break;
        }

        Cent_ast* a = Cent_parse_object_stmt(pd);
        if (a) {
            Cent_ast_add(n, a);
        }

        if (Cent_has_separator(pd, n)) {
            while (Cent_has_separator(pd, n));
        } else {
            break;
        }
    }

    pd->top = env->prev;

    return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_object_stmt(Cent_parse_data* pd)
{
    Cent_lookahead(pd);

    if (pd->lookahead->type == Cent_token_dot) {
        return Cent_parse_property(pd);
    }

    if (pd->lookahead->type == Cent_token_let) {
        return Cent_parse_let(pd);
    }

    return Cent_parse_expr(pd);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_property(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_prop_set;

    Cent_token* dot = NULL;
    if (!Cent_match(pd, Cent_token_dot, "expected dot", &dot, n)) {
        assert(false && "not possible");
    }
    Cent_token_destroy(dot);
    free(dot);

    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, n);
    /* test case: test_parse_value_error_object_property_expected_id */

    /* property set */
    if (id && id->builtin_type == Cent_builtin_type_none) {
        Cent_ast* a = NULL;
        Cent_ast_create(&a);
        a->type = Cent_ast_type_id;
        if (id) {
            Zinc_string_copy(&id->value, &a->text);
            Cent_token_destroy(id);
            free(id);
        }
        Cent_ast_add(n, a);

        Cent_token* eq = NULL;
        Cent_match(pd, Cent_token_equal, "expected equal", &eq, n);
        Cent_token_destroy(eq);
        free(eq);

        Cent_ast* b = Cent_parse_expr(pd);
        if (b) {
            Cent_ast_add(n, b);
        }

        return n;
    }

    /* builtin method call */
    if (id) {
        if (id->builtin_type == Cent_builtin_type_child_of) {
            n->type = Cent_ast_type_method_child_of;
        } else if (id->builtin_type == Cent_builtin_type_property_of) {
            n->type = Cent_ast_type_method_property_of;
        } else {
            error_list_set(pd->errors, &id->loc, "invalid method: %b", id->value);
            n->has_error = true;
        }

        Cent_token_destroy(id);
        free(id);

        Cent_token* lp = NULL;
        Cent_match(pd, Cent_token_left_paren, "expected left parenthesis", &lp, n);
        Cent_token_destroy(lp);
        free(lp);

        Cent_parse_method_call_seq(pd, n);

        Cent_token* rp = NULL;
        Cent_match(pd, Cent_token_right_paren, "expected right parenthesis", &rp, n);
        Cent_token_destroy(rp);
        free(rp);

        return n;
    }

    error_list_set(pd->errors, &pd->lookahead->loc, "expected a property set or method call");
    n->has_error = true;

    return n;
}

/* call_seq -> expr call_seq' | e */
/* call_seq' -> , expr | e */
void Cent_parse_method_call_seq(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_ast* a = Cent_parse_expr(pd);
    Cent_ast_add(n, a);

    Cent_lookahead(pd);

    while (pd->lookahead->type == Cent_token_comma) {
        Cent_token* comma = NULL;
        if (!Cent_match(pd, Cent_token_comma, "expected comma", &comma, n)) {
            assert(false && "not possible");
        }
        Cent_token_destroy(comma);
        free(comma);

        Cent_ast* b = Cent_parse_expr(pd);
        Cent_ast_add(n, b);

        Cent_lookahead(pd);
    }

    if (!n->has_error) {
        if (n->type == Cent_ast_type_method_child_of || n->type == Cent_ast_type_method_property_of) {
            Cent_ast* p = Cent_ast_get(n, 0);
            if (!p) {
                error_list_set(pd->errors, &p->loc, "expected argument to method");
                n->has_error = true;
            } else if (p->type != Cent_ast_type_expr_variable) {
                error_list_set(pd->errors, &p->loc, "expected a variable");
                n->has_error = true;
            } else {
                p = Cent_ast_get(n, 1);
                if (n->type == Cent_ast_type_method_child_of) {
                    if (p) {
                        error_list_set(pd->errors, &p->loc, "too many arguments for @child_of");
                        n->has_error = true;
                    }
                }
                if (n->type == Cent_ast_type_method_property_of) {
                    if (!p) {
                        error_list_set(
                            pd->errors,
                            &pd->lookahead->loc,
                            "not enough arguments for @property_of()");
                        n->has_error = true;
                    } else if (p->type != Cent_ast_type_expr_string) {
                        error_list_set(pd->errors, &p->loc, "expecting a string expression for @property_of");
                        n->has_error = true;
                    } else if (p->data.string.size == 0) {
                        error_list_set(pd->errors, &p->loc, "string should not be empty");
                        n->has_error = true;
                    }
                }
            }
        }
    }
}