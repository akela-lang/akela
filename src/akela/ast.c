#define AKELA_AST_C

#include <stdlib.h>
#include <stdbool.h>
#include "ast.h"
#include "zinc/memory.h"
#include "zinc/list.h"
#include "type.h"

void Ake_ast_create(Ake_Ast** n)
{
	Zinc_malloc_safe((void**)n, sizeof(Ake_Ast));
	Ake_ast_init(*n);
}

void Ake_ast_init(Ake_Ast* n)
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
void Ake_ast_destroy(Ake_Ast* n)
{
    if (n) {
        Ake_Ast* p = n->head;
        while (p) {
            Ake_Ast* temp = p;
            p = p->next;
            Ake_ast_destroy(temp);
        }

        Zinc_string_destroy(&n->value);
        Ake_TypeDestroy(n->type);

    	Ake_EnvironmentDestroy(n->env);
    	free(n->env);

        free(n);
    }
}

void Ake_ast_add(Ake_Ast* p, Ake_Ast* c)
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

/* assume parent and child are not NULL */
void Ast_node_push(Ake_Ast* parent, Ake_Ast* child)
{
	Ake_Ast* old_head = parent->head;
	if (old_head) {
		old_head->prev = child;
	}
	child->next = old_head;
	parent->head = child;
}

Ake_Ast* Ast_node_get(Ake_Ast* p, size_t pos)
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
void Ake_ast_copy(Ake_Ast* src, Ake_Ast* dest)
{
    dest->kind = src->kind;
    dest->type = Ake_TypeClone(src->type);
    dest->loc = src->loc;
    Zinc_string_copy(&src->value, &dest->value);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_ast_clone(Ake_Ast* n)
{
	Ake_Ast* copy = NULL;

	if (n) {
        Ake_ast_create(&copy);
        Ake_ast_copy(n, copy);

		Ake_Ast* p = n->head;
		while (p) {
			Ake_Ast* p_copy = NULL;
			p_copy = Ake_ast_clone(p);
            Ake_ast_add(copy, p_copy);
			p = p->next;
		}
	}

	return copy;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Ake_ast_match(Ake_Ast* a, Ake_Ast* b)
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
			if (!Ake_ast_match(c, d)) {
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

size_t Ake_ast_count_children(Ake_Ast* n)
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
