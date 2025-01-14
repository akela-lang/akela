#include <stdlib.h>
#include <stdbool.h>
#include "type_def.h"
#include "zinc/memory.h"
#include "ast.h"
#include "struct_element.h"

void Ake_type_def_create(struct Ake_type_def** n)
{
	malloc_safe((void**)n, sizeof(struct Ake_type_def));
	Ake_type_def_init(*n);
}

void Ake_type_def_init(struct Ake_type_def* n)
{
	n->type = Ake_type_none;
	buffer_init(&n->name);
	n->is_signed = false;
	n->bit_count = 0;
	n->composite = NULL;
    n->composite_type = NULL;
    hash_table_init(&n->struct_impl, IMPL_HASH_SIZE);
    hash_table_init(&n->type_impl, IMPL_HASH_SIZE);
}

void Ake_type_def_destroy(struct Ake_type_def* n)
{
	if (n) {
		buffer_destroy(&n->name);
        Ake_ast_destroy(n->composite);
        hash_table_map(&n->struct_impl, (hash_table_func)Ake_struct_element_destroy);
        hash_table_destroy(&n->struct_impl);
        hash_table_map(&n->type_impl, (hash_table_func)Ake_ast_destroy);
        hash_table_destroy(&n->type_impl);
        free(n);
	}
}

/* copy dag excluding etype */
struct Ake_type_def* Ake_type_def_copy(struct Ake_type_def* n)
{
	struct Ake_type_def* copy = NULL;

	if (n) {
		Ake_type_def_create(&copy);
		copy->type = n->type;
		buffer_copy(&n->name, &copy->name);
		copy->is_signed = n->is_signed;
		copy->bit_count = n->bit_count;
	}

	return copy;
}