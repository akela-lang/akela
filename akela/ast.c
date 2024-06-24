#define AKELA_AST_C

#include <stdlib.h>
#include <stdbool.h>
#include "ast.h"
#include "zinc/memory.h"
#include "type_def.h"

void Type_options_init(Type_options* to)
{
    to->is_mut = false;
    to->is_array = false;
    to->is_slice = false;
    to->is_ref = false;
    to->original_is_mut = false;
    VectorInit(&to->dim, sizeof(Type_dimension));
}
void Ast_node_create(Ast_node** n)
{
	malloc_safe((void**)n, sizeof(Ast_node));
	Ast_node_init(*n);
}

void Ast_node_init(Ast_node* n)
{
	n->type = Ast_type_none;
	buffer_init(&n->value);
	n->tu = NULL;
	n->td = NULL;
    Type_options_init(&n->to);
    location_init(&n->loc);
    n->sym = NULL;
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ast_node_destroy(Ast_node* n)
{
    if (n) {
        Ast_node* p = n->head;
        while (p) {
            Ast_node* temp = p;
            p = p->next;
            Ast_node_destroy(temp);
        }

        buffer_destroy(&n->value);
        Ast_node_destroy(n->tu);
        Type_options_destroy(&n->to);
        free(n);
    }
}

void Type_options_destroy(Type_options* to)
{
    VectorDestroy(&to->dim);
}

void Ast_node_add(Ast_node* p, Ast_node* c)
{
    if (p->head && p->tail) {
        p->tail->next = c;
        c->prev = p->tail;
        p->tail = c;
    } else {
        p->head = c;
        p->tail = c;
    }
    location_combine(&p->loc, &c->loc);
}

/* assume parent and child are not NULL */
void Ast_node_push(Ast_node* parent, Ast_node* child)
{
	Ast_node* old_head = parent->head;
	if (old_head) {
		old_head->prev = child;
	}
	child->next = old_head;
	parent->head = child;
}

Ast_node* Ast_node_get(Ast_node* p, size_t pos)
{
	int i = 0;
	for (Ast_node* n = p->head; n; n = n->next) {
		if (i == pos) {
			return n;
		}
		i++;
	}
	return NULL;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ast_node_print(Ast_node* root, bool debug)
{
    char* names[Ast_type_count];
    Ast_set_names(names);

    if (root == NULL) return;
	if (!debug && !root->head) return;

	printf("<");
	if (root->td) {
		buffer_finish(&root->td->name);
		if (debug) printf("[%p]", root);
		printf("%s,%s", names[root->type], root->td->name.buf);
	} else {
		if (debug) printf("[%p]", root);
		printf("%s", names[root->type]);
	}
	if (root->value.size > 0) {
		buffer_finish(&root->value);
		printf(",%s", root->value.buf);
	}
	printf(">");
	printf(":");
	for (Ast_node* p = root->head; p; p = p->next) {
		printf(" <");
		if (p->td) {
			buffer_finish(&p->td->name);
			printf("%s,%s", names[p->type], p->td->name.buf);
		} else {
			printf("%s", names[p->type]);
		}
		if (p->value.size > 0) {
			buffer_finish(&p->value);
			printf(",%s", p->value.buf);
		}
		printf(">");
	}

	printf("\n");

	if (debug) {
        Ast_node_print(root->tu, debug);
	}

	for (Ast_node* p = root->head; p; p = p->next) {
        Ast_node_print(p, debug);
	}
}

void Type_options_copy(Type_options* src, Type_options* dest)
{
    dest->is_mut = src->is_mut;
    dest->is_array = src->is_array;
    dest->is_slice = src->is_slice;
    dest->is_ref = src->is_ref;
    dest->original_is_mut = src->original_is_mut;
    VectorCopy(&src->dim, &dest->dim);
}

void Type_options_reduce_dimension(Type_options* to)
{
    if (to->dim.count > 0) {
        Type_dimension current;
        Type_dimension *first = (Type_dimension*)VECTOR_PTR(&to->dim, 0);
        current = *first;

        if (!to->original_is_mut || current.option == Array_element_const) {
            to->is_mut = false;
        } else {
            to->is_mut = true;
        }

        for (size_t i = 0; i < to->dim.count - 1; i++) {
            Type_dimension* dest = (Type_dimension*)VECTOR_PTR(&to->dim, i);
            Type_dimension* src = (Type_dimension*)VECTOR_PTR(&to->dim, i + 1);
            *dest = *src;
        }
        if (to->dim.count > 0) {
            to->dim.count--;
        }
        if (to->dim.count == 0) {
            to->is_array = false;
            to->is_slice = false;
        }
    }
}

/* copy dag excluding tu */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ast_node* Ast_node_copy(Ast_node* n)
{
	Ast_node* copy = NULL;

	if (n) {
        Ast_node_create(&copy);
		copy->type = n->type;
		copy->td = n->td;
        Type_options_copy(&n->to, &copy->to);
        copy->loc = n->loc;
		buffer_copy(&n->value, &copy->value);

		Ast_node* p = n->head;
		while (p) {
			Ast_node* p_copy = NULL;
			p_copy = Ast_node_copy(p);
            Ast_node_add(copy, p_copy);
			p = p->next;
		}
	}

	return copy;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Ast_node_match(Ast_node* a, Ast_node* b)
{
	if (a && b) {
		if (a->type != b->type) {
			return false;
		}

		if (!buffer_compare(&a->value, &b->value)) {
			return false;
		}

		if (a->td != b->td) {
			return false;
		}

		Ast_node* c = a->head;
		Ast_node* d = b->head;
		do {
			if (!Ast_node_match(c, d)) {
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

size_t Ast_node_count_children(Ast_node* n)
{
    size_t count = 0;
    if (n) {
        Ast_node* p = n->head;
        while (p) {
            count++;
            p = p->next;
        }
    }

    return count;
}
