#define AKELA_AST_C

#include <stdlib.h>
#include <stdbool.h>
#include "ast.h"
#include "zinc/memory.h"

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
    n->is_mut = false;
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
        Type_use_destroy(n->tu);
        free(n);
    }
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
	if (root->tu && root->tu->td) {
		buffer_finish(&root->tu->td->name);
		if (debug) printf("[%p]", root);
		printf("%s,%s", names[root->type], root->tu->td->name.buf);
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
		if (p->tu && p->tu->td) {
			buffer_finish(&p->tu->td->name);
			printf("%s,%s", names[p->type], p->tu->td->name.buf);
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

	for (Ast_node* p = root->head; p; p = p->next) {
        Ast_node_print(p, debug);
	}
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ast_node_copy(Ast_node* src, Ast_node* dest)
{
    dest->type = src->type;
    dest->tu = Type_use_clone(src->tu);
    dest->loc = src->loc;
    buffer_copy(&src->value, &dest->value);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ast_node* Ast_node_clone(Ast_node* n)
{
	Ast_node* copy = NULL;

	if (n) {
        Ast_node_create(&copy);
        Ast_node_copy(n, copy);

		Ast_node* p = n->head;
		while (p) {
			Ast_node* p_copy = NULL;
			p_copy = Ast_node_clone(p);
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

        if (a->tu && b->tu) {
            if (a->tu->td != b->tu->td) {
                return false;
            }
        } else if (a->tu) {
            return false;
        } else if (b->tu) {
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
