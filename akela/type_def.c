#include <stdlib.h>
#include <stdbool.h>
#include "type_def.h"
#include "zinc/result.h"
#include "zinc/memory.h"
#include "zinc/buffer.h"
#include "ast.h"

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
	n->composite = NULL;
    n->composite_type = NULL;
}

/* dynamic-destroy n n{} */
void type_def_destroy(struct type_def* n)
{
	if (n) {
		/* destroy n n{} */
		buffer_destroy(&n->name);
        Ast_node_destroy(n->composite);
		free(n);
	}
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
	} else if (!a && !b) {
		return true;
	} else {
		return false;
	}

	return true;
}
