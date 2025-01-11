#include "ast.h"
#include <assert.h>

#include "environment.h"
#include "zinc/memory.h"

void Cent_ast_init(Cent_ast *ast)
{
    ast->type = Cent_ast_type_none;
    ast->value_type = Cent_value_type_none;
    ast->number_type = Cent_number_type_none;
    ast->variable_type = Cent_variable_type_none,
    buffer_init(&ast->text);
    ast->env = NULL;
    location_init(&ast->loc);
    ast->has_error = false;
    ast->next = NULL;
    ast->prev = NULL;
    ast->head = NULL;
    ast->tail = NULL;
    ast->parent = NULL;
}

void Cent_ast_create(Cent_ast **ast)
{
    malloc_safe((void**)ast, sizeof(Cent_ast));
    Cent_ast_init(*ast);
}

void Cent_ast_value_set_type(Cent_ast *ast, Cent_value_type type)
{
    ast->value_type = type;
    Cent_data_init(&ast->data, type);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_ast_destroy(Cent_ast *ast)
{
    if (ast) {
        buffer_destroy(&ast->text);

        if (ast->env) {
            /* do not destroy shared base */
            if (ast->env->prev) {
                Cent_environment_destroy(ast->env);
                free(ast->env);
            }
        }

        Cent_ast* p = ast->head;
        while (p) {
            Cent_ast* temp = p;
            p = p->next;
            Cent_ast_destroy(temp);
            free(temp);
        }

        Cent_data_destroy(&ast->data, ast->value_type);
    }
}

void Cent_ast_add(Cent_ast *p, Cent_ast *c)
{
    if (p->head && p->tail) {
        p->tail->next = c;
        c->prev = p->tail;
        p->tail = c;
    } else {
        p->head = c;
        p->tail = c;
    }

    if (c->has_error) {
        p->has_error = true;
    }

    location_combine(&p->loc, &c->loc);

    c->parent = p;
}

Cent_ast* Cent_ast_get(Cent_ast *n, size_t index)
{
    size_t i = 0;
    Cent_ast* p = n->head;
    while (p) {
        if (i == index) {
            return p;
        }
        p = p->next;
        i++;
    }

    return NULL;
}

size_t Cent_ast_count(Cent_ast* n)
{
    size_t count = 0;
    Cent_ast* p = n->head;
    while (p) {
        count++;
        p = p->next;
    }
    return count;
}