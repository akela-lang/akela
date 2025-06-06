#include <stdbool.h>
#include "parse_data.h"
#include "ast.h"
#include "lex.h"
#include <assert.h>
#include "parse_tools.h"
#include "parse_transform.h"
#include "parse_validation.h"

void Cent_lookahead(Cent_parse_data* pd)
{
    if (!pd->lookahead) {
        pd->lookahead = lex(pd->ld);
    }
}

bool Cent_match(Cent_parse_data* pd, Cent_token_type type, char* message, Cent_token** t, Cent_ast* n)
{
    Cent_lookahead(pd);

    if (pd->lookahead->type == type) {
        Zinc_location_combine(&n->loc, &pd->lookahead->loc);
        *t = pd->lookahead;
        pd->lookahead = NULL;
        return true;
    }

    Zinc_error_list_set(pd->errors, &pd->lookahead->loc, message);
    n->has_error = true;
    return false;
}

void Cent_ignore(Cent_parse_data* pd)
{
    Cent_token* t = pd->lookahead;
    pd->lookahead = NULL;
    Cent_token_destroy(t);
    free(t);
}

void Cent_ignore_newlines(Cent_parse_data* pd)
{
    Cent_lookahead(pd);
    while (pd->lookahead->type == Cent_token_newline) {
        Cent_ignore(pd);
        Cent_lookahead(pd);
    }
}

// Sep -> '\n' | ';' | e
bool Cent_parse_sep(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_token_type type;

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_newline || pd->lookahead->type == Cent_token_semicolon) {
        type = pd->lookahead->type;
    } else {
        return false;
    }

    Cent_token* sep = NULL;
    if (!Cent_match(pd, type, "expected separator", &sep, n)) {
        assert(false && "not possible");
    }
    Cent_token_destroy(sep);
    free(sep);

    return true;
}

Cent_environment* Cent_get_environment(Cent_ast* n)
{
    Cent_ast* p = n;
    while (p) {
        if (p->env) {
            return p->env;
        }
        p = p->parent;
    }

    return NULL;
}

Cent_namespace_result Cent_namespace_lookup(Cent_ast* n)
{
    Cent_namespace_result nr = {NULL, NULL};

    Cent_ast* p = n->head;
    assert(p);
    assert(p != n->tail);

    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get(top, &p->text);
    assert(sym);

    if (sym->type != Cent_symbol_type_module) {
        nr.sym = sym;
        nr.node = p;
        return nr;
    }

    Cent_module* mod = sym->data.module;
    assert(mod);

    p = p->next;
    while (p && p != n->tail) {
        Cent_module* temp = Cent_module_get(mod, &p->text);
        if (!temp) break;
        mod = temp;
        p = p->next;
    }

    sym = Cent_environment_get(mod->env, &p->text);
    if (sym) {
        nr.sym = sym;
        nr.node = p;
        return nr;
    }

    return nr;
}

void Cent_parse_process_tasks(Cent_parse_data* pd, Cent_parse_result* pr)
{
    while (pd->pq.head) {
        Zinc_priority_task* task = Zinc_priority_queue_pop_lowest(&pd->pq);
        if (task->priority == Cent_task_type_update_element_type) {
            Cent_update_element_type(pr, task->data);
        } else if (task->priority == Cent_task_type_update_enum_type) {
            Cent_update_enum(pr, task->data);
        } else if (task->priority == Cent_task_type_transform_variant_set) {
            Cent_parse_transform_variant_set(pd, pr, task->data);
        } else if (task->priority == Cent_task_type_transform_variant_type) {
            Cent_parse_transform_variant_type(pd, pr, task->data);
        } else if (task->priority == Cent_task_type_check_namespace) {
            Cent_check_namespace(pr, task->data);
        } else if (task->priority == Cent_task_type_check_variable) {
            Cent_check_variables(pr, task->data);
        } else if (task->priority == Cent_task_type_update_namespace) {
            Cent_update_namespace(pr, task->data);
        } else {
            assert(false && "unhandled task");
        }
        free(task);
    }
}
