#include <stdlib.h>
#include <stdbool.h>
#include "type_def.h"
#include "zinc/result.h"
#include "zinc/memory.h"
#include "zinc/buffer.h"

/* dynamic-output n */
void type_def_create(struct type_def** n)
{
	/* allocate n */
	malloc_safe((void**)n, sizeof(struct type_def));
	type_def_init(*n);
}

void type_def_init(struct type_def* n)
{
	n->type = type_none;
	buffer_init(&n->name);
	n->is_signed = false;
	n->bit_count = 0;
	n->is_generic = false;
	n->generic_count = 0;
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
}

/* dynamic-destroy n n{} */
void type_def_destroy(struct type_def* n)
{
	if (n) {
		/* destroy n{} */
		struct type_def* p = n->head;
		while (p) {
			struct type_def* temp = p;
			p = p->next;
			type_def_destroy(temp);
		}

		/* destroy n n{} */
		buffer_destroy(&n->name);
		free(n);
	}
}

/* dynamic-output-none */
void type_def_add(struct type_def* p, struct type_def* c)
{
	// set sibling to left
	struct type_def* prev = p->tail;
	if (prev) {
		prev->next = c;
	}

	// set child
	c->prev = prev;
	c->next = NULL;

	// set parent
	if (p->head == NULL) {
		p->head = c;
	}
	p->tail = c;
}

/* assume parent and child are not NULL */
/* dynamic-output-none */
void type_def_push(struct type_def* p, struct type_def* c)
{
	struct type_def* old_head = p->head;
	if (old_head) {
		old_head->prev = c;
	}
	c->next = old_head;
	p->head = c;
}

/* dynamic-output-none */
struct type_def* type_def_get(struct type_def* p, size_t pos)
{
	int i = 0;
	for (struct type_def* n = p->head; n; n = n->next) {
		if (i == pos) {
			return n;
		}
		i++;
	}
	return NULL;
}

/* copy dag excluding etype */
struct type_def* type_def_copy(struct type_def* n)
{
	struct type_def* copy = NULL;

	if (n) {
		type_def_create(&copy);
		copy->type = n->type;
		buffer_copy(&n->name, &copy->name);
		copy->is_signed = n->is_signed;
		copy->bit_count = n->bit_count;

		struct type_def* p = n->head;
		while (p) {
			struct type_def* p_copy = NULL;
			p_copy = type_def_copy(p);
			type_def_add(copy, p_copy);
			p = p->next;
		}
	}

	return copy;
}

bool type_def_match(struct type_def* a, struct type_def* b)
{
	struct type_def* copy = NULL;

	if (a && b) {
		if (a->type != b->type) {
			return false;
		}
		if (!buffer_compare(&a->name, &b->name)) {
			return false;
		}

		struct type_def* c = a->head;
		struct type_def* d = b->head;
		do {
			if (!type_def_match(c, d)) {
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
