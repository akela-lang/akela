#define _DAG_C

#include <stdlib.h>
#include <stdbool.h>
#include "dag.h"
#include "zinc/result.h"
#include "zinc/memory.h"

/* dynamic-output-none */
enum result dag_set_names(char** names)
{
	for (int i = 0; i < dag_type_count; i++) {
		names[i] = NULL;
	}

	names[dag_type_none] = "none";
	names[dag_type_id] = "word";
	names[dag_type_sign] = "sign";
	names[dag_type_number] = "number";
	names[dag_type_assign] = "assign";
	names[dag_type_plus] = "plus";
	names[dag_type_minus] = "minus";
	names[dag_type_mult] = "mult";
	names[dag_type_divide] = "divide";
	names[dag_type_stmts] = "stmts";
	names[dag_type_function] = "function";
	names[dag_type_dseq] = "dseq";
	names[dag_type_dret] = "dret";
	names[dag_type_call] = "call";
	names[dag_type_cseq] = "cseq";
	names[dag_type_if] = "if";
	names[dag_type_conditional_branch] = "conditional-branch";
	names[dag_type_default_branch] = "default-branch";
	names[dag_type_equality] = "equality";
	names[dag_type_not_equal] = "not equal";
	names[dag_type_less_than_or_equal] = "less than or equal";
	names[dag_type_greater_than_or_equal] = "greater than or equal";
	names[dag_type_not] = "not";
	names[dag_type_and] = "and";
	names[dag_type_or] = "or";
	names[dag_type_while] = "while";
	names[dag_type_for_range] = "for-range";
	names[dag_type_for_iteration] = "for-iteration";
	names[dag_type_declaration] = "declaration";
	names[dag_type_array_literal] = "array-literal";
	names[dag_type_array_subscript] = "array-subscript";
	names[dag_type_anonymous_function] = "anonymous-function";
	names[dag_type_var] = "var";
	names[dag_type_boolean] = "boolean";
	names[dag_type_array] = "array";
	names[dag_type_array_type_name] = "array-type-name";
	names[dag_type_type_name] = "type-name";

	for (int i = 0; i < dag_type_count; i++) {
		if (names[i] == NULL) {
			return set_error("missing dag name: %d", i);
		}
	}

	return result_ok;
}

/* dynamic-output n */
void dag_create_node(struct dag_node** n)
{
	/* allocate n */
	malloc_safe((void**)n, sizeof(struct dag_node));
	dag_init_node(*n);
}

/* dynamic-destroy n n{} */
void dag_destroy(struct dag_node* n)
{
	if (n) {
		/* destroy n{} */
		struct dag_node* p = n->head;
		while (p) {
			struct dag_node* temp = p;
			p = p->next;
			dag_destroy(temp);
		}

		/* destroy n{} */
		buffer_destroy(&n->value);
		token_list_destroy(&n->tl);
		dag_destroy(n->etype);

		/* destroy n */
		free(n);
	}
}

/* dynamic-output-none */
void dag_init_node(struct dag_node* n)
{
	n->type = dag_type_none;
	buffer_init(&n->value);
	token_list_init(&n->tl);
	n->etype = NULL;
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
}

/* dynamic-output-none */
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
/* dynamic-output-none */
void dag_push(struct dag_node* parent, struct dag_node* child)
{
	struct dag_node* old_head = parent->head;
	if (old_head) {
		old_head->prev = child;
	}
	child->next = old_head;
	parent->head = child;
}

/* dynamic-output-none */
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

/* dynamic-output-none */
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

/* dynamic-output-none */
/* dynamic-temp a */
void dag_print(struct dag_node* root, char** names)
{
	if (root == NULL) return;

	printf("%s:", names[root->type]);

	for (struct dag_node* p = root->head; p; p = p->next) {
		printf(" %s", names[p->type]);
		if (p->value.size > 0) {
			char* a;

			/* allocate a */
			buffer2array(&p->value, &a);
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

/* copy dag excluding etype */
struct dag_node* dag_copy(struct dag_node* n)
{
	struct dag_node* copy = NULL;

	if (n) {
		dag_create_node(&copy);
		copy->type = n->type;
		buffer_copy(&n->value, &copy->value);
		
		struct dag_node* p = n->head;
		while (p) {
			struct dag_node* p_copy = NULL;
			p_copy = dag_copy(p);
			dag_add_child(copy, p_copy);
			p = p->next;
		}
	}

	return copy;
}

bool dag_match(struct dag_node* a, struct dag_node* b)
{
	struct dag_node* copy = NULL;

	if (a && b) {
		if (a->type != b->type) {
			return false;
		}

		if (!buffer_compare(&a->value, &b->value)) {
			return false;
		}

		struct dag_node* c = a->head;
		struct dag_node* d = b->head;
		do {
			if (!dag_match(c, d)) {
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
