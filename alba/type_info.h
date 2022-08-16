#ifndef _TYPE_INFO_H
#define _TYPE_INFO_H

#include "alba_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "source.h"
#include "symbol_table.h"

struct type_info {
	struct buffer name;
	bool is_integer;
	bool is_float;
	bool is_signed;
	int bit_count;
	struct type_info* next;
};

struct type_node {
	struct type_info* ti;
	struct type_node* next;
	struct type_node* prev;
	struct type_node* head;
	struct type_node* tail;
};

/* dynamic-output n */
ALBA_API void type_node_create(struct type_node** n);

/* dynamic-destroy n n{} */
ALBA_API void type_node_destroy(struct type_node* n);

/* dynamic-output-none */
ALBA_API void type_node_init(struct type_node* n);

/* dynamic-output-none */
ALBA_API void type_node_add(struct type_node* p, struct type_node* c);

/* dynamic-output-none */
ALBA_API void type_node_push(struct type_node* parent, struct type_node* child);

/* dynamic-output-none */
ALBA_API struct type_node* type_node_get(struct type_node* p, size_t pos);

ALBA_API struct type_node* type_node_copy(struct type_node* n);

ALBA_API bool type_node_match(struct type_node* a, struct type_node* b);

#endif
