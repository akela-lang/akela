#include <stdlib.h>
#include "dag.h"
#include "result.h"
#include "memory.h"

enum result_enum dag_create_node(struct dag_node** n)
{
	enum result_enum r = malloc_safe(n, sizeof(struct dag_node));
	if (r == error_result) {
		return r;
	}
	dag_init_node(*n);
	return ok_result;
}

void dag_init_node(struct dag_node* n)
{
	n->type = dag_type_none;
	string_init(&n->value);
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
}

void dag_add_child(struct dag_node* p, struct dag_node* c)
{
	// set sibling to left
	struct dag_node* prev = p->tail;
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

void dag_destroy(struct dag_node* r)
{
	struct dag_node* c = r->head;
	while (c) {
		if (c->head) {
			dag_destroy(c->head);
		}
		struct dag_node* temp = c;
		c = c->next;
		string_reset(&temp->value);
		free(temp);
	}
	string_reset(&r->value);
	free(r);
}
