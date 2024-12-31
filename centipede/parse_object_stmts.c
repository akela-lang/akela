#include "ast.h"
#include "parse_data.h"
#include "parse_tools.h"
#include "parse_expr.h"
#include <assert.h>

Cent_ast* Cent_parse_object_stmt(Cent_parse_data* pd);
Cent_ast* Cent_parse_property(Cent_parse_data* pd);

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

    Cent_ignore_newlines(pd, n);
    while (true) {
        Cent_lookahead(pd);
        if (pd->lookahead->type == Cent_token_right_curly_brace) {
            break;
        }

        Cent_ast* a = Cent_parse_object_stmt(pd);
        if (a) {
            Cent_ast_add(n, a);
        }
        if (!Cent_has_separator(pd, n)) {
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
            buffer_copy(&id->value, &a->text);
            Cent_token_destroy(id);
            free(id);
        }
        Cent_ast_add(n, a);

        Cent_lookahead(pd);
        if (id->builtin_type != Cent_builtin_type_none) {

        }

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

        Cent_ast* a = Cent_parse_expr(pd);
        Cent_ast_add(n, a);

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