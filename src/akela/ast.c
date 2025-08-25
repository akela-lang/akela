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
	n->kind = AKE_AST_NONE;
	Zinc_string_init(&n->struct_value);
	Zinc_string_init(&n->boolean_value);
	n->type = NULL;
    Zinc_location_init(&n->loc);
	n->env = NULL;
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
	n->parent = NULL;
	n->has_error = false;
	n->is_set = false;
}

void Ake_AstSet(Ake_Ast* n, Ake_AstKind kind)
{
	assert(!n->is_set);
	n->kind = kind;
	switch (kind) {
		case AKE_AST_ID:
			Zinc_string_init(&n->data.id.value);
			n->is_set = true;
			break;
		case AKE_AST_SIGN:
			n->data.sign.op = NULL;
			n->data.sign.right = NULL;
			n->is_set = true;
			break;
		case AKE_AST_NUMBER:
			Zinc_string_init(&n->data.number.value);
			n->is_set = true;
			break;
		case AKE_AST_STRING:
			Zinc_string_init(&n->data.string.value);
			n->is_set = true;
			break;
		default:
			break;
	}
}

void Ake_AstValidate(Ake_Ast* n)
{
	switch(n->kind) {
		case AKE_AST_ID:
		case AKE_AST_SIGN:
		case AKE_AST_NUMBER:
		case AKE_AST_STRING:
			assert(n->is_set);
			break;
		default:
			break;
	}
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_AstDestroy(Ake_Ast* n)
{
    if (n) {
    	Ake_AstValidate(n);

        Ake_Ast* p = n->head;
        while (p) {
            Ake_Ast* temp = p;
            p = p->next;
            Ake_AstDestroy(temp);
        }

    	switch(n->kind) {
    		case AKE_AST_ID:
    			Zinc_string_destroy(&n->data.id.value);
    			break;
    		case AKE_AST_SIGN:
    			Ake_AstDestroy(n->data.sign.op);
    			Ake_AstDestroy(n->data.sign.right);
    			break;
    		case AKE_AST_NUMBER:
    			Zinc_string_destroy(&n->data.number.value);
    			break;
    		case AKE_AST_STRING:
    			Zinc_string_destroy(&n->data.string.value);
    			break;
        	default:
    			break;
    	}

    	Zinc_string_destroy(&n->struct_value);
    	Zinc_string_destroy(&n->boolean_value);
        Ake_TypeDestroy(n->type);
    	free(n->type);

    	Ake_EnvironmentDestroy(n->env);
    	free(n->env);

        free(n);
    }
}

void Ake_AstAdd(Ake_Ast* p, Ake_Ast* c)
{
	Ake_AstValidate(p);
	Ake_AstValidate(c);

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
	if (c->has_error) {
		p->has_error = true;
	}
}

Ake_Ast* Ake_AstGet(Ake_Ast* p, size_t pos)
{
	Ake_AstValidate(p);

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
	Ake_AstValidate(src);
	Ake_AstValidate(dest);

    Ake_AstSet(dest, src->kind);
    dest->type = Ake_TypeClone(src->type);
    dest->loc = src->loc;
	switch (src->kind) {
		case AKE_AST_ID:
			Zinc_string_add_string(&src->data.id.value, &dest->data.id.value);
			break;
		case AKE_AST_SIGN:
			dest->data.sign.right = Ake_AstClone(src->data.sign.right);
			break;
		case AKE_AST_NUMBER:
			Zinc_string_add_string(&src->data.number.value, &dest->data.number.value);
			break;
		case AKE_AST_STRING:
			Zinc_string_add_string(&src->data.string.value, &dest->data.string.value);
		default:
			break;
	}
	Zinc_string_copy(&src->struct_value, &dest->struct_value);
	Zinc_string_copy(&src->boolean_value, &dest->boolean_value);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_AstClone(Ake_Ast* n)
{
	Ake_AstValidate(n);

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

		Ake_AstValidate(a);
		Ake_AstValidate(b);

		switch (a->kind) {
			case AKE_AST_ID:
				if (!Zinc_string_compare(&a->data.id.value, &b->data.id.value)) {
					return false;
				}
				break;
			case AKE_AST_SIGN:
				if (!Ake_AstMatch(a->data.sign.right, b->data.sign.right)) {
					return false;
				}
				break;
			case AKE_AST_NUMBER:
				if (!Zinc_string_compare(&a->data.number.value, &b->data.number.value)) {
					return false;
				}
			case AKE_AST_STRING:
				if (!Zinc_string_compare(&a->data.string.value, &b->data.string.value)) {
					return false;
				}
			default:
				break;
		}

		if (!Zinc_string_compare(&a->struct_value, &b->struct_value)) {
			return false;
		}

		if (!Zinc_string_compare(&a->boolean_value, &b->boolean_value)) {
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
    	Ake_AstValidate(n);
        Ake_Ast* p = n->head;
        while (p) {
            count++;
            p = p->next;
        }
    }

    return count;
}

void Ake_AstListInit(Ake_AstList* list)
{
	list->head = NULL;
	list->tail = NULL;
}

void Ake_AstListCreate(Ake_AstList** list)
{
	Zinc_malloc_safe((void**)list, sizeof (Ake_AstList));
	Ake_AstListInit(*list);
}

void Ake_AstListDestroy(Ake_AstList* list)
{
	Ake_Ast* p = list->head;
	while (p) {
		Ake_Ast* temp = p;
		p = p->next;
		Ake_AstDestroy(temp);
	}
}

void Ake_AstListAdd(Ake_AstList* list, Ake_Ast* n)
{
	Ake_AstValidate(n);

	if (list->head && list->tail) {
		list->tail->next = n;
		n->prev = list->tail;
		list->tail = n;
	} else {
		list->head = n;
		list->tail = n;
	}
}