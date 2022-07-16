#define _DAG_C

#include <stdlib.h>
#include "dag.h"
#include "zinc/result.h"
#include "zinc/memory.h"

/* dynamic-output n */
enum result dag_create_node(struct dag_node** n)
{
	/* allocate n */
	malloc_safe(n, sizeof(struct dag_node));
	dag_init_node(*n);
	return result_ok;
}

/* static-output */
void dag_init_node(struct dag_node* n)
{
	n->type = dag_type_none;
	buffer_init(&n->value);
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
}

/* static-output */
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

/* assume parent and child are not NULL */
/* static-output */
void dag_push(struct dag_node* parent, struct dag_node* child)
{
	struct dag_node* old_head = parent->head;
	if (old_head) {
		old_head->prev = child;
	}
	child->next = old_head;
	parent->head = child;
}

/* static-output */
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

/* static-output */
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

/* static-output */
/* dynamic-temp a */
void dag_print(struct dag_node* root, char** names)
{
	if (root == NULL) return;

	printf("%s:", names[root->type]);

	for (struct dag_node* p = root->head; p; p = p->next) {
		printf(" %s", names[p->type]);
		if (p->value.size > 0) {
			char* a;
			enum result r;

			/* allocate a */
			r = buffer2array(&p->value, &a);
			printf(":%s", a);

			/* destroy a */
			free(a);
		}
	}

	printf("\n");

	for (struct dag_node* p = root->head; p; p = p->next) {
		if (p->head != NULL) {
			dag_print(p, names);
		}
	}
}
