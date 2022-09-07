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
	struct type_def* td;
	struct ast_node* tu;
};

struct symbol_table {
	struct environment* initial;
	struct environment* global;
	struct environment* top;
	struct ast_node* numeric_pool;
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

ALBA_API bool type_find_whole(struct symbol_table* st, struct ast_node* a, struct ast_node* b);

ALBA_API bool type_def_can_cast(struct type_def* a, struct type_def* b);

ALBA_API bool type_use_can_cast(struct ast_node* a, struct ast_node* b);

ALBA_API void transfer_global_symbols(struct symbol_table* src, struct symbol_table* dest);

ALBA_API void transfer_module_symbols(struct environment* src, struct environment* dest, struct buffer* module_name);

#endif
