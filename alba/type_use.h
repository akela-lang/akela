#ifndef _TYPE_USE_H
#define _TYPE_USE_H

#include "alba_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "source.h"
#include "symbol_table.h"
#include "type_def.h"

struct type_use {
	struct type_def* td;
	struct type_use* next;
	struct type_use* prev;
	struct type_use* head;
	struct type_use* tail;
};

/* dynamic-output n */
ALBA_API void type_use_create(struct type_use** n);

/* dynamic-destroy n n{} */
ALBA_API void type_use_destroy(struct type_use* n);

/* dynamic-output-none */
ALBA_API void type_use_init(struct type_use* n);

/* dynamic-output-none */
ALBA_API void type_use_add(struct type_use* p, struct type_use* c);

/* dynamic-output-none */
ALBA_API void type_use_push(struct type_use* parent, struct type_use* child);

/* dynamic-output-none */
ALBA_API struct type_use* type_use_get(struct type_use* p, size_t pos);

ALBA_API struct type_use* type_use_copy(struct type_use* n);

ALBA_API bool type_use_match(struct type_use* a, struct type_use* b);

ALBA_API int type_use_count_children(struct type_use* tu);

#endif
