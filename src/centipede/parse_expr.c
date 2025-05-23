#include "ast.h"
#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>
#include "parse_object.h"

Cent_ast* Cent_parse_assign(Cent_parse_data* pd);
void Cent_parse_check_lvalue(Cent_parse_data* pd, Cent_ast* n);
void Cent_parse_check_rvalue(Cent_parse_data* pd, Cent_ast* n);
Cent_ast* Cent_parse_namespace(Cent_parse_data* pd);
Cent_ast* Cent_parse_factor(Cent_parse_data* pd);
Cent_ast* Cent_parse_expr_integer(Cent_parse_data* pd);
Cent_ast* Cent_parse_expr_natural(Cent_parse_data* pd);
Cent_ast* Cent_parse_expr_real(Cent_parse_data* pd);
Cent_ast* Cent_parse_expr_string(Cent_parse_data* pd);
Cent_ast* Cent_parse_expr_boolean(Cent_parse_data* pd);
Cent_ast* Cent_parse_expr_id(Cent_parse_data* pd);
void Cent_parse_expr_enum(Cent_parse_data* pd, Cent_token* id, Cent_ast* n);
void Cent_parse_expr_assign(Cent_parse_data* pd, Cent_token* id, Cent_ast* n);
void Cent_parse_expr_builtin_function(Cent_parse_data* pd, Cent_token* id, Cent_ast* n);
void Cent_parse_expr_variable(Cent_parse_data* pd, Cent_token* id, Cent_ast* n);

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_expr(Cent_parse_data* pd)
{
    return Cent_parse_namespace(pd);
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
    if (!a) {
        return a;
    }

    Cent_lookahead(pd);
    while (pd->lookahead->type == Cent_token_double_colon) {
        if (!n) {
            if (a->type != Cent_ast_type_id) {
                Zinc_error_list_set(pd->errors, &a->loc, "expected id");
                a->has_error = true;
                /* test case: test_parse_namespace_error_expected_id */
            }

            Cent_ast_create(&n);
            n->type = Cent_ast_type_namespace;
            Cent_ast_add(n, a);
        }

        Cent_token* dc = NULL;
        if (!Cent_match(pd, Cent_token_double_colon, "expected double colon", &dc, n)) {
            assert(false && "not possible");
        }
        Cent_token_destroy(dc);
        free(dc);

        b = Cent_parse_factor(pd);
        if (!b) {
            break;
        }
        if (b->type != Cent_ast_type_id) {
            Zinc_error_list_set(pd->errors, &b->loc, "expected id");
            b->has_error = true;
            /* test case: test_parse_namespace_error_expected_id2 */
        }
        Cent_ast_add(n, b);


        Cent_lookahead(pd);
    }

    if (!n) {
        if (a) {
            n = a;
        }
    }

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_left_curly_brace) {
        n = Cent_parse_object(pd, n); 
    }

    if (n->type == Cent_ast_type_id) {
        n->type = Cent_ast_type_expr_variable;
    }

    if (!n->has_error) {
        if (n->type == Cent_ast_type_namespace) {
            // check namespace
            Zinc_priority_task* task = NULL;
            Zinc_priority_task_create(&task);
            task->priority = Cent_task_type_check_namespace;
            task->data = n;
            Zinc_priority_queue_add(&pd->pq, task);

            // update namespace
            Zinc_priority_task* task2 = NULL;
            Zinc_priority_task_create(&task2);
            task2->priority = Cent_task_type_update_namespace;
            task2->data = n;
            Zinc_priority_queue_add(&pd->pq, task2);
        } else if (n->type == Cent_ast_type_expr_variable) {
            // check variable
            Zinc_priority_task* task = NULL;
            Zinc_priority_task_create(&task);
            task->priority = Cent_task_type_check_variable;
            task->data = n;
            Zinc_priority_queue_add(&pd->pq, task);
        }
    }

    return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_factor(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;

    Cent_lookahead(pd);

    if (pd->lookahead->type == Cent_token_integer) {
        n = Cent_parse_expr_integer(pd);
    } else if (pd->lookahead->type == Cent_token_natural) {
        n = Cent_parse_expr_natural(pd);
    } else if (pd->lookahead->type == Cent_token_real) {
        n = Cent_parse_expr_real(pd);
    } else if (pd->lookahead->type == Cent_token_string) {
        n = Cent_parse_expr_string(pd);
    } else if (pd->lookahead->type == Cent_token_true || pd->lookahead->type == Cent_token_false) {
        n = Cent_parse_expr_boolean(pd);
    } else if (pd->lookahead->type == Cent_token_id) {
        n = Cent_parse_expr_id(pd);
    }

    return n;
}

Cent_ast* Cent_parse_expr_integer(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_expr_integer;
    Cent_ast_value_set_type(n, Cent_value_type_integer);

    Cent_token* num = NULL;
    if (!Cent_match(pd, Cent_token_integer, "expected integer", &num, n)) {
        assert(false && "not possible");
    }
    if (num) {
        n->data.integer = num->data.integer;
        Cent_token_destroy(num);
        free(num);
    }

    return n;
}

Cent_ast* Cent_parse_expr_natural(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_expr_natural;
    Cent_ast_value_set_type(n, Cent_value_type_natural);

    Cent_token* num = NULL;
    if (!Cent_match(pd, Cent_token_natural, "expected natural", &num, n)) {
        assert(false && "not possible");
    }
    if (num) {
        n->data.natural = num->data.natural;
        Cent_token_destroy(num);
        free(num);
    }

    return n;
}

Cent_ast* Cent_parse_expr_real(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_expr_real;
    Cent_ast_value_set_type(n, Cent_value_type_real);

    Cent_token* num = NULL;
    if (!Cent_match(pd, Cent_token_real, "expected real", &num, n)) {
        assert(false && "not possible");
    }
    if (num) {
        n->data.real = num->data.real;
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
    Zinc_string_copy(&str->value, &n->data.string);
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

    Cent_parse_expr_variable(pd, id, n);
    return n;
}

void Cent_parse_expr_variable(Cent_parse_data* pd, Cent_token* id, Cent_ast* n)
{
    n->type = Cent_ast_type_id;
    Zinc_string_copy(&id->value, &n->text);
    Cent_token_destroy(id);
    free(id);
}
