#define AKELA_AST_C

#include <stdlib.h>
#include <stdbool.h>
#include "ast.h"
#include "zinc/memory.h"
#include "zinc/list.h"
#include "type.h"

void Ake_AstCreate(Ake_Ast** n)
{
	Zinc_malloc_safe((void**)n, sizeof(Ake_Ast));
	Ake_AstInit(*n);
}

void Ake_AstInit(Ake_Ast* n)
{
	n->kind = Ake_ast_type_none;
	Zinc_string_init(&n->value);
	n->type = NULL;
    Zinc_location_init(&n->loc);
	n->env = NULL;
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
	n->parent = NULL;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_AstDestroy(Ake_Ast* n)
{
    if (n) {
        Ake_Ast* p = n->head;
        while (p) {
            Ake_Ast* temp = p;
            p = p->next;
            Ake_AstDestroy(temp);
        }

        Zinc_string_destroy(&n->value);
        Ake_TypeDestroy(n->type);
    	free(n->type);

    	Ake_EnvironmentDestroy(n->env);
    	free(n->env);

        free(n);
    }
}

void Ake_AstAdd(Ake_Ast* p, Ake_Ast* c)
{
    if (p->head && p->tail) {
        p->tail->next = c;
        c->prev = p->tail;
        p->tail = c;
    } else {
        p->head = c;
        p->tail = c;
    }
    Zinc_location_combine(&p->loc, &c->loc);
	c->parent = p;
}

Ake_Ast* Ake_AstGet(Ake_Ast* p, size_t pos)
{
	int i = 0;
	for (Ake_Ast* n = p->head; n; n = n->next) {
		if (i == pos) {
			return n;
		}
		i++;
	}
	return NULL;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_AstCopy(Ake_Ast* src, Ake_Ast* dest)
{
    dest->kind = src->kind;
    dest->type = Ake_TypeClone(src->type);
    dest->loc = src->loc;
    Zinc_string_copy(&src->value, &dest->value);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_AstClone(Ake_Ast* n)
{
	Ake_Ast* copy = NULL;

	if (n) {
        Ake_AstCreate(&copy);
        Ake_AstCopy(n, copy);

		Ake_Ast* p = n->head;
		while (p) {
			Ake_Ast* p_copy = NULL;
			p_copy = Ake_AstClone(p);
            Ake_AstAdd(copy, p_copy);
			p = p->next;
		}
	}

	return copy;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Ake_AstMatch(Ake_Ast* a, Ake_Ast* b)
{
	if (a && b) {
		if (a->kind != b->kind) {
			return false;
		}

		if (!Zinc_string_compare(&a->value, &b->value)) {
			return false;
		}

		if (!Ake_TypeMatch(a->type, b->type, NULL)) {
			return false;
		}

		Ake_Ast* c = a->head;
		Ake_Ast* d = b->head;
		do {
			if (!Ake_AstMatch(c, d)) {
				return false;
			}
			if (c) c = c->next;
			if (d) d = d->next;
		} while (c || d);
	} else if (!a && !b) {
		return true;
	} else {
		return false;
	}

	return true;
}

size_t Ake_AstCountChildren(Ake_Ast* n)
{
    size_t count = 0;
    if (n) {
        Ake_Ast* p = n->head;
        while (p) {
            count++;
            p = p->next;
        }
    }

    return count;
}
