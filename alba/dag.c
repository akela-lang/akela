#include <stdlib.h>
#include "dag.h"
#include "result.h"
#include "memory.h"

enum result dag_create_node(struct allocator* al, struct dag_node** n)
{
	enum result r = malloc_safe(n, sizeof(struct dag_node));
	if (r == result_error) {
		return r;
	}
	dag_init_node(*n);
	return result_ok;
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

/*
* assume parent and child are not NULL
*/
void dag_push(struct dag_node* parent, struct dag_node* child)
{
	struct dag_node* old_head = parent->head;
	if (old_head) {
		old_head->prev = child;
	}
	child->next = old_head;
	parent->head = child;
}

struct dag_node* dag_get_child(struct dag_node* p, size_t pos)
{
	int i = 0;
	for (struct dag_node* n = p->head; n; n = n->next) {
		if (i == pos) {
			return n;
		}
		i++;
	}
	return NULL;
}

int is_binary_operator(struct dag_node* n)
{
	if (n->type == dag_type_plus) {
		return 1;
	} else if (n->type == dag_type_minus) {
		return 1;
	} else if (n->type == dag_type_mult) {
		return 1;
	} else if (n->type == dag_type_divide) {
		return 1;
	}

	return 0;
}
