#include <stdlib.h>
#include <stdbool.h>
#include "ast.h"
#include "zinc/result.h"
#include "zinc/memory.h"
#include "type_def.h"
#include "windows.h"
#include <DbgHelp.h>

/* dynamic-output-none */
enum result ast_set_names(char** names)
{
	for (int i = 0; i < ast_type_count; i++) {
		names[i] = NULL;
	}

	names[ast_type_none] = "none";
	names[ast_type_id] = "id";
	names[ast_type_sign] = "sign";
	names[ast_type_number] = "number";
	names[ast_type_string] = "string";
	names[ast_type_assign] = "assign";
	names[ast_type_plus] = "plus";
	names[ast_type_minus] = "minus";
	names[ast_type_mult] = "mult";
	names[ast_type_divide] = "divide";
	names[ast_type_stmts] = "stmts";
	names[ast_type_function] = "function";
	names[ast_type_dseq] = "dseq";
	names[ast_type_dret] = "dret";
	names[ast_type_call] = "call";
	names[ast_type_cseq] = "cseq";
	names[ast_type_if] = "if";
	names[ast_type_conditional_branch] = "conditional-branch";
	names[ast_type_default_branch] = "default-branch";
	names[ast_type_equality] = "equality";
	names[ast_type_not_equal] = "not-equal";
	names[ast_type_less_than] = "less-than";
	names[ast_type_less_than_or_equal] = "less-than-or-equal";
	names[ast_type_greater_than] = "greater-than";
	names[ast_type_greater_than_or_equal] = "greater-than-or-equal";
	names[ast_type_not] = "not";
	names[ast_type_and] = "and";
	names[ast_type_or] = "or";
	names[ast_type_while] = "while";
	names[ast_type_for_range] = "for-range";
	names[ast_type_for_iteration] = "for-iteration";
	names[ast_type_declaration] = "declaration";
	names[ast_type_array_literal] = "array-literal";
	names[ast_type_array_subscript] = "array-subscript";
	names[ast_type_anonymous_function] = "anonymous-function";
	names[ast_type_var] = "var";
	names[ast_type_boolean] = "boolean";
	names[ast_type_array] = "array";
	names[ast_type_parenthesis] = "parenthesis";
	names[ast_type_type] = "type";
	names[ast_type_power] = "power";
	names[ast_type_type_pool] = "type-pool";
	names[ast_type_dot] = "dot";
	names[ast_type_module] = "module";
	names[ast_type_struct] = "struct";

	for (int i = 0; i < ast_type_count; i++) {
		if (names[i] == NULL) {
			return set_error("missing dag name: %d", i);
		}
	}

	return result_ok;
}

/* dynamic-output n */
void ast_node_create(struct ast_node** n)
{
	/* allocate n */
	malloc_safe((void**)n, sizeof(struct ast_node));
	ast_node_init(*n);
}

/* dynamic-destroy n n{} */
void ast_node_destroy(struct ast_node* n)
{
	if (n) {
		/* destroy n{} */
		struct ast_node* p = n->head;
		while (p) {
			struct ast_node* temp = p;
			p = p->next;
			ast_node_destroy(temp);
		}

		/* destroy n{} */
		buffer_destroy(&n->value);
		ast_node_destroy(n->tu);

		/* destroy n */
		free(n);
	}
}

/* dynamic-output-none */
void ast_node_init(struct ast_node* n)
{
	n->type = ast_type_none;
	buffer_init(&n->value);
	n->tu = NULL;
	n->td = NULL;
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
}

/* dynamic-output-none */
void ast_node_add(struct ast_node* p, struct ast_node* c)
{
	// set sibling to left
	struct ast_node* prev = p->tail;
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
void ast_node_push(struct ast_node* parent, struct ast_node* child)
{
	struct ast_node* old_head = parent->head;
	if (old_head) {
		old_head->prev = child;
	}
	child->next = old_head;
	parent->head = child;
}

/* dynamic-output-none */
struct ast_node* ast_node_get(struct ast_node* p, size_t pos)
{
	int i = 0;
	for (struct ast_node* n = p->head; n; n = n->next) {
		if (i == pos) {
			return n;
		}
		i++;
	}
	return NULL;
}

/* dynamic-output-none */
/* dynamic-temp a */
void ast_node_print(struct ast_node* root, char** names)
{
	if (root == NULL) return;

	if (root->td) {
		buffer_finish(&root->td->name);
		printf("[%p]%s<%s>", root, names[root->type], root->td->name.buf);
	} else {
		printf("[%p]%s", root, names[root->type]);
	}
	if (root->value.size > 0) {
		buffer_finish(&root->value);
		printf("-%s", root->value.buf);
	}
	printf(":");
	for (struct ast_node* p = root->head; p; p = p->next) {
		if (p->td) {
			buffer_finish(&p->td->name);
			printf(" %s<%s>", names[p->type], p->td->name.buf);
		} else {
			printf(" %s", names[p->type]);
		}
		if (p->value.size > 0) {
			buffer_finish(&p->value);
			printf("-%s", p->value.buf);
		}
	}

	printf("\n");

	ast_node_print(root->tu, names);

	for (struct ast_node* p = root->head; p; p = p->next) {
		ast_node_print(p, names);
	}
}

/* copy dag excluding etype */
struct ast_node* ast_node_copy(struct ast_node* n)
{
	struct ast_node* copy = NULL;

	if (n) {
		ast_node_create(&copy);
		copy->type = n->type;
		copy->td = n->td;
		buffer_copy(&n->value, &copy->value);
		
		struct ast_node* p = n->head;
		while (p) {
			struct ast_node* p_copy = NULL;
			p_copy = ast_node_copy(p);
			ast_node_add(copy, p_copy);
			p = p->next;
		}
	}

	return copy;
}

bool ast_node_match(struct ast_node* a, struct ast_node* b)
{
	struct ast_node* copy = NULL;

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

		struct ast_node* c = a->head;
		struct ast_node* d = b->head;
		do {
			if (!ast_node_match(c, d)) {
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

int ast_node_count_children(struct ast_node* n)
{
	int count = 0;
	struct ast_node* p = n->head;
	while (p) {
		count++;
		p = p->next;
	}
	return count;
}
