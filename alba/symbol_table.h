#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include "alba_api.h"
#include <stdbool.h>
#include "hash.h"
#include "token.h"

#define ENVIRONMENT_HASH_TABLE_SIZE 32

struct environment {
	struct hash_table ht;
	struct environment* prev;
};

struct symbol {
	enum token_enum tk_type;
	struct ast_node* dec;
	struct type_def* td;
	struct type_use* tu;
};

struct symbol_table {
	struct environment* initial;
	struct environment* top;
	struct type_def* numeric_pool;
};

/* dynamic-output env{} */
/* transfer p -> env */
ALBA_API void environment_init(struct environment* env, struct environment* p);

/* dynamic-output env{} */
/* transfer sym -> env{} */
ALBA_API void environment_put(struct environment* env, struct buffer* value, struct symbol* sym);

/* dynamic-output-none */
ALBA_API struct symbol* environment_get(struct environment* env, struct buffer* value);

/* dynamic-output-none */
ALBA_API struct symbol* environment_get_local(struct environment* env, struct buffer* value);

/* dynamic-output_none */
ALBA_API void symbol_init(struct symbol* sym);

/* dynamic-destroy env env{} */
ALBA_API void environment_destroy(struct environment* env);

ALBA_API void symbol_table_init(struct symbol_table* st);

ALBA_API void symbol_table_destroy(struct symbol_table* st);

ALBA_API bool symbol_table_is_global(struct symbol_table* st);

ALBA_API bool is_numeric(struct type_def* td);

ALBA_API bool type_find_whole(struct symbol_table* st, struct type_use* a, struct type_use* b);

ALBA_API bool type_use_can_cast(struct symbol_table* st, struct type_use* a, struct type_use* b);

#endif
