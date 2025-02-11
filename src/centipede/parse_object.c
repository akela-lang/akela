#include "ast.h"
#include "parse_data.h"
#include "parse_tools.h"
#include "parse_expr.h"
#include <assert.h>
#include "parse_const.h"

Cent_ast* Cent_parse_object_stmts(Cent_parse_data* pd);
Cent_ast* Cent_parse_object_stmt(Cent_parse_data* pd);
Cent_ast* Cent_parse_property(Cent_parse_data* pd);
void Cent_parse_method_call_seq(Cent_parse_data* pd, Cent_ast* n);
Cent_ast* Cent_parse_object_level(Cent_parse_data* pd, size_t level);
void Cent_parse_object_finish(Cent_parse_data* pd, Cent_token* id, Cent_ast* n);

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_parse_object_finish(Cent_parse_data* pd, Cent_token* id, Cent_ast* n)
{
    n->type = Cent_ast_type_expr_object;

    Zinc_string_copy(&id->value, &n->text);

    Cent_token_destroy(id);
    free(id);

    Cent_token* lcb = NULL;
    if (!Cent_match(pd, Cent_token_left_curly_brace, "expected left-curly-brace", &lcb, n)) {
        assert(false && "not possible");
    }
    Cent_token_destroy(lcb);
    free(lcb);

    Cent_environment* env = NULL;
    Cent_environment_create(&env);
    env->prev = pd->top;
    pd->top = env;
    n->env = env;

    Cent_ast* a = Cent_parse_object_stmt(pd);
    if (a) {
        Cent_ast_add(n, a);
    }

    while (Cent_parse_sep(pd, n)) {
        Cent_ast* b = Cent_parse_object_stmt(pd);
        if (b) {
            Cent_ast_add(n, b);
        }
    }

    pd->top = env->prev;

    Cent_token* rcb = NULL;
    Cent_match(pd, Cent_token_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Cent_token_destroy(rcb);
    free(rcb);
}


/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_object(Cent_parse_data* pd, Cent_ast* ns)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_expr_object;
    n->loc = ns->loc;

    if (ns->type == Cent_ast_type_namespace) {
        Cent_ast* id = Cent_ast_get(ns, 0);
        assert(id);
        Cent_ast* variant = Cent_ast_get(ns, 1);
        assert(variant);

        Zinc_string_copy(&id->text, &n->text);

        Cent_ast* vset = NULL;
        Cent_ast_create(&vset);
        vset->type = Cent_ast_type_variant_set;
        Zinc_string_add_string(&vset->text, &variant->text);
        Cent_ast_add(n, vset);

        Zinc_priority_task* task = NULL;
        Zinc_priority_task_create(&task);
        task->priority = Cent_task_type_transform_variant_set;
        task->data = vset;
        Zinc_priority_queue_add(&pd->pq, task);
    } else if (ns->type == Cent_ast_type_id) {
        Zinc_string_copy(&ns->text, &n->text);
    }

    Cent_ast_destroy(ns);
    free(ns);

    Cent_token* lcb = NULL;
    if (!Cent_match(pd, Cent_token_left_curly_brace, "expected left-curly-brace", &lcb, n)) {
        assert(false && "not possible");
    }
    Cent_token_destroy(lcb);
    free(lcb);

    Cent_environment* env = NULL;
    Cent_environment_create(&env);
    env->prev = pd->top;
    pd->top = env;
    n->env = env;

    Cent_ast* a = Cent_parse_object_stmt(pd);
    if (a) {
        Cent_ast_add(n, a);
    }

    while (Cent_parse_sep(pd, n)) {
        Cent_ast* b = Cent_parse_object_stmt(pd);
        if (b) {
            Cent_ast_add(n, b);
        }
    }

    pd->top = env->prev;

    Cent_token* rcb = NULL;
    Cent_match(pd, Cent_token_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Cent_token_destroy(rcb);
    free(rcb);

    return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_object_stmt(Cent_parse_data* pd)
{
    Cent_lookahead(pd);

    if (pd->lookahead->type == Cent_token_dot) {
        return Cent_parse_property(pd);
    }

    if (pd->lookahead->type == Cent_token_const) {
        return Cent_parse_const(pd);
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
            Zinc_error_list_set(pd->errors, &id->loc, "invalid method: %b", id->value);
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

    Zinc_error_list_set(pd->errors, &pd->lookahead->loc, "expected a property set or method call");
    n->has_error = true;

    return n;
}

/* call_seq -> expr call_seq' | e */
/* call_seq' -> , expr | e */
void Cent_parse_method_call_seq(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_ast* a = Cent_parse_expr(pd);
    if (!a) {
        return;
    }
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
                Zinc_error_list_set(pd->errors, &p->loc, "expected argument to method");
                n->has_error = true;
            } else if (p->type != Cent_ast_type_expr_variable) {
                Zinc_error_list_set(pd->errors, &p->loc, "expected a variable");
                n->has_error = true;
            } else {
                p = Cent_ast_get(n, 1);
                if (n->type == Cent_ast_type_method_child_of) {
                    if (p) {
                        Zinc_error_list_set(pd->errors, &p->loc, "too many arguments for @child_of");
                        n->has_error = true;
                    }
                }
                if (n->type == Cent_ast_type_method_property_of) {
                    if (!p) {
                        Zinc_error_list_set(
                            pd->errors,
                            &pd->lookahead->loc,
                            "not enough arguments for @property_of()");
                        n->has_error = true;
                    } else if (p->type != Cent_ast_type_expr_string) {
                        Zinc_error_list_set(pd->errors, &p->loc, "expecting a string expression for @property_of");
                        n->has_error = true;
                    } else if (p->data.string.size == 0) {
                        Zinc_error_list_set(pd->errors, &p->loc, "string should not be empty");
                        n->has_error = true;
                    }
                }
            }
        }
    }
}

Cent_ast* Cent_parse_follow_on(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_follow_on;

    Cent_ast* a = NULL;
    Cent_ast_create(&a);
    a->type = Cent_ast_type_expr_natural;

    size_t level = 0;
    Cent_lookahead(pd);
    while (pd->lookahead->type == Cent_token_dot) {
        Cent_token* dot = NULL;
        if (!Cent_match(pd, Cent_token_dot, "expected dot", &dot, a)) {
            assert(false && "not possible");
        }
        Cent_token_destroy(dot);
        free(dot);
        level++;
        Cent_lookahead(pd);
    }

    a->data.integer = level;
    Cent_ast_add(n, a);

    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, n);

    Cent_ast* b = NULL;
    Cent_ast_create(&b);
    b->type = Cent_ast_type_expr_object;
    Cent_parse_object_finish(pd, id, b);
    Cent_ast_add(n, b);

    if (!n->has_error) {
        Zinc_priority_task* task = NULL;
        Zinc_priority_task_create(&task);
        task->priority = Cent_task_type_check_follow_on;
        task->data = n;
        Zinc_priority_queue_add(&pd->pq, task);
    }

    return n;
}
