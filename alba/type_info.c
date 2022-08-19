#include <stdlib.h>
#include <stdbool.h>
#include "type_info.h"
#include "zinc/result.h"
#include "zinc/memory.h"
#include "zinc/buffer.h"

/* dynamic-output n */
void type_info_create(struct type_info** n)
{
	/* allocate n */
	malloc_safe((void**)n, sizeof(struct type_info));
	type_info_init(*n);
}

void type_info_init(struct type_info* n)
{
	n->type = type_none;
	buffer_init(&n->name);
	n->is_signed = false;
	n->bit_count = 0;
	n->is_generic = false;
	n->generic_count = 0;
	n->pool = NULL;
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
}

/* dynamic-destroy n n{} */
void type_info_destroy(struct type_info* n)
{
	if (n) {
		/* destroy n{} */
		struct type_info* p = n->head;
		while (p) {
			struct type_info* temp = p;
			p = p->next;
			type_info_destroy(temp);
		}

		/* destroy n n{} */
		buffer_destroy(&n->name);
		free(n);
	}
}

/* dynamic-output-none */
void type_info_add(struct type_info* p, struct type_info* c)
{
	// set sibling to left
	struct type_info* prev = p->tail;
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
void type_info_push(struct type_info* p, struct type_info* c)
{
	struct type_info* old_head = p->head;
	if (old_head) {
		old_head->prev = c;
	}
	c->next = old_head;
	p->head = c;
}

/* dynamic-output-none */
struct type_info* type_info_get(struct type_info* p, size_t pos)
{
	int i = 0;
	for (struct type_info* n = p->head; n; n = n->next) {
		if (i == pos) {
			return n;
		}
		i++;
	}
	return NULL;
}

/* copy dag excluding etype */
struct type_info* type_info_copy(struct type_info* n)
{
	struct type_info* copy = NULL;

	if (n) {
		type_info_create(&copy);
		copy->type = n->type;
		buffer_copy(&n->name, &copy->name);
		copy->is_signed = n->is_signed;
		copy->bit_count = n->bit_count;

		struct type_info* p = n->head;
		while (p) {
			struct type_info* p_copy = NULL;
			p_copy = type_info_copy(p);
			type_info_add(copy, p_copy);
			p = p->next;
		}
	}

	return copy;
}

bool type_info_match(struct type_info* a, struct type_info* b)
{
	struct type_info* copy = NULL;

	if (a && b) {
		if (a->type != b->type) {
			return false;
		}
		if (!buffer_compare(&a->name, &b->name)) {
			return false;
		}

		struct type_info* c = a->head;
		struct type_info* d = b->head;
		do {
			if (!type_info_match(c, d)) {
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
