#include "ast.h"
#include "zinc/memory.h"

void Worm_ast_init(Worm_ast* n)
{
    n->type = Worm_ast_type_none;
    Zinc_string_init(&n->value);
    n->next = NULL;
    n->prev = NULL;
    n->head = NULL;
    n->tail = NULL;
}

void Worm_ast_create(Worm_ast** n)
{
    Zinc_malloc_safe((void**)n, sizeof(Worm_ast)); 
    Worm_ast_init(*n);
}

void Worm_ast_add(Worm_ast* p, Worm_ast* c)
{
    if (p->head && p->tail) {
        p->tail->next = c;
        c->prev = p->tail;
        p->tail = c;
    } else {
        p->head = c;
        p->tail = c;
    }
}

void Worm_ast_destroy(Worm_ast* n)
{
    Zinc_string_destroy(&n->value);
    
    Worm_ast* p = n->head;
    while (p) {
        Worm_ast* temp = p;
        p = p->next;
        Worm_ast_destroy(temp);
        free(temp);
    }
}
