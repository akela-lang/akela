#include <stdlib.h>
#include <stdbool.h>
#include "type_use.h"
#include "zinc/result.h"
#include "zinc/memory.h"
#include "zinc/buffer.h"

/* dynamic-output n */
void type_use_create(struct type_use** n)
{
	/* allocate n */
	malloc_safe((void**)n, sizeof(struct type_use));
	type_use_init(*n);
}

void type_use_init(struct type_use* n)
{
	n->td = NULL;
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
}

/* dynamic-destroy n n{} */
void type_use_destroy(struct type_use* n)
{
	if (n) {
		/* destroy n{} */
		struct type_use* p = n->head;
		while (p) {
			struct type_use* temp = p;
			p = p->next;
			type_use_destroy(temp);
		}

		/* destroy n */
		free(n);
	}
}

/* dynamic-output-none */
void type_use_add(struct type_use* p, struct type_use* c)
{
	// set sibling to left
	struct type_use* prev = p->tail;
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
void type_use_push(struct type_use* p, struct type_use* c)
{
	struct type_use* old_head = p->head;
	if (old_head) {
		old_head->prev = c;
	}
	c->next = old_head;
	p->head = c;
}

/* dynamic-output-none */
struct type_use* type_use_get(struct type_use* p, size_t pos)
{
	int i = 0;
	for (struct type_use* n = p->head; n; n = n->next) {
		if (i == pos) {
			return n;
		}
		i++;
	}
	return NULL;
}

/* copy dag excluding etype */
struct type_use* type_use_copy(struct type_use* n)
{
	struct type_use* copy = NULL;

	if (n) {
		type_use_create(&copy);
		copy->td = n->td;

		struct type_use* p = n->head;
		while (p) {
			struct type_use* p_copy = NULL;
			p_copy = type_use_copy(p);
			type_use_add(copy, p_copy);
			p = p->next;
		}
	}

	return copy;
}

bool type_use_match(struct type_use* a, struct type_use* b)
{
	struct type_use* copy = NULL;

	if (a && b) {
		if(a->td != b->td) {
			return false;
		}

		struct type_use* c = a->head;
		struct type_use* d = b->head;
		do {
			if (!type_use_match(c, d)) {
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

int type_use_count_children(struct type_use* tu)
{
	int count = 0;

	struct type_use* p = tu->head;
	while (p) {
		count++;
		p = p->next;
	}

	return count;
}
