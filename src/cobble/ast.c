#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include "zinc/memory.h"
#include "zinc/zstring.h"

void Cob_ast_init(Cob_ast* n)
{
    n->type = Cob_ast_type_none;
    n->num = 0;
    Zinc_location_init(&n->loc);
    n->is_root = false;
    n->is_group = false;
    n->group = 0;
    n->next = NULL;
    n->prev = NULL;
    n->head = NULL;
    n->tail = NULL;
    n->parent = NULL;
}

void Cob_ast_create(Cob_ast** n)
{
    Zinc_malloc_safe((void**)n, sizeof(Cob_ast));
    Cob_ast_init(*n);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cob_ast_destroy(Cob_ast* n)
{
    if (n) {
        Cob_ast* p = n->head;
        while (p) {
            Cob_ast* temp = p;
            p = p->next;
            Cob_ast_destroy(temp);
            free(temp);
        }
    }
}

void Cob_ast_add(Cob_ast* p, Cob_ast* c)
{
    if (p->head && p->tail) {
        p->tail->next = c;
        c->prev = p->tail;
        p->tail = c;
    } else {
        p->head = c;
        p->tail = c;
    }
    c->parent = p;
    Zinc_location_combine(&p->loc, &c->loc);

    if (c->type == Cob_ast_type_error) {
        p->type = Cob_ast_type_error;
    }
}

Cob_ast* Cob_ast_get(Cob_ast* n, int index)
{
    int count = 0;
    Cob_ast* p = n->head;
    while (p) {
        if (count == index) {
            return p;
        }
        count++;
        p = p->next;
    }

    return NULL;
}