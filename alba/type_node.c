#include <stdlib.h>
#include <stdbool.h>
#include "type_node.h"
#include "zinc/result.h"
#include "zinc/memory.h"
#include "zinc/buffer.h"

/* dynamic-output n */
void type_node_create(struct type_node** n)
{
	/* allocate n */
	malloc_safe((void**)n, sizeof(struct type_node));
	type_node_init(*n);
}

void type_node_init(struct type_node* n)
{
	n->ti = NULL;
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
}

/* dynamic-destroy n n{} */
void type_node_destroy(struct type_node* n)
{
	if (n) {
		/* destroy n{} */
		struct type_node* p = n->head;
		while (p) {
			struct type_node* temp = p;
			p = p->next;
			type_node_destroy(temp);
		}

		/* destroy n */
		free(n);
	}
}

/* dynamic-output-none */
void type_node_add(struct type_node* p, struct type_node* c)
{
	// set sibling to left
	struct type_node* prev = p->tail;
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
void type_node_push(struct type_node* p, struct type_node* c)
{
	struct type_node* old_head = p->head;
	if (old_head) {
		old_head->prev = c;
	}
	c->next = old_head;
	p->head = c;
}

/* dynamic-output-none */
struct type_node* type_node_get(struct type_node* p, size_t pos)
{
	int i = 0;
	for (struct type_node* n = p->head; n; n = n->next) {
		if (i == pos) {
			return n;
		}
		i++;
	}
	return NULL;
}

/* copy dag excluding etype */
struct type_node* type_node_copy(struct type_node* n)
{
	struct type_node* copy = NULL;

	if (n) {
		type_node_create(&copy);
		copy->ti = n->ti;

		struct type_node* p = n->head;
		while (p) {
			struct type_node* p_copy = NULL;
			p_copy = type_node_copy(p);
			type_node_add(copy, p_copy);
			p = p->next;
		}
	}

	return copy;
}

bool type_node_match(struct type_node* a, struct type_node* b)
{
	struct type_node* copy = NULL;

	if (a && b) {
		if(a->ti != b->ti) {
			return false;
		}

		struct type_node* c = a->head;
		struct type_node* d = b->head;
		do {
			if (!type_node_match(c, d)) {
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
