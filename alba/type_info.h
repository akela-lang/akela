#ifndef _TYPE_INFO_H
#define _TYPE_INFO_H

#include "alba_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "source.h"
#include "symbol_table.h"

enum type {
	type_none,
	type_integer,
	type_float,
	type_array,
	type_struct,
	type_boolean,
	type_string,
	type_function,
	type_function_dseq,
};

struct type_info {
	enum type type;
	struct buffer name;
	bool is_signed;
	int bit_count;
	bool is_generic;
	int generic_count;
	struct type_info* pool;
	struct type_info* next;
	struct type_info* prev;
	struct type_info* head;
	struct type_info* tail;
};

/* dynamic-output n */
ALBA_API void type_info_create(struct type_info** n);

/* dynamic-destroy n n{} */
ALBA_API void type_info_destroy(struct type_info* n);

/* dynamic-output-none */
ALBA_API void type_info_init(struct type_info* n);

/* dynamic-output-none */
ALBA_API void type_info_add(struct type_info* p, struct type_info* c);

/* dynamic-output-none */
ALBA_API void type_info_push(struct type_info* parent, struct type_info* child);

/* dynamic-output-none */
ALBA_API struct type_info* type_info_get(struct type_info* p, size_t pos);

ALBA_API struct type_info* type_info_copy(struct type_info* n);

ALBA_API bool type_info_match(struct type_info* a, struct type_info* b);

#endif
