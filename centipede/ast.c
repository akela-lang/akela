#include "ast.h"
#include <assert.h>

#include "environment.h"
#include "zinc/memory.h"

void Cent_ast_init(Cent_ast *ast)
{
    ast->type = Cent_ast_type_none;
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

void Cent_ast_set_type(Cent_ast *ast, Cent_ast_type type)
{
    ast->type = type;
    if (ast->type == Cent_ast_type_element) {
        ast->data.element = NULL;
    } else if (ast->type == Cent_ast_type_enumerate) {
        ast->data.enumerate = NULL;
    } else if (ast->type == Cent_ast_type_value) {
        ast->data.value = NULL;
    } else {
        assert(false && "should not happen");
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_ast_destroy(Cent_ast *ast)
{
    if (ast->env) {
        Cent_environment_destroy(ast->env);
        free(ast->env);
    }

    Cent_ast* p = ast->head;
    while (p) {
        Cent_ast* temp = p;
        p = p->next;
        Cent_ast_destroy(temp);
        free(temp);
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
}