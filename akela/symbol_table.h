#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/hash.h"
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
	struct symbol* constructor;
	struct ast_node* root;
	struct ast_node* root_ptr;
};

struct symbol_table {
	struct environment* initial;
	struct environment* global;
	struct environment* top;
	struct ast_node* numeric_pool;
};

/* dynamic-output env{} */
/* transfer p -> env */
AKELA_API void environment_init(struct environment* env, struct environment* p);

/* dynamic-output env{} */
/* transfer sym -> env{} */
AKELA_API void environment_put(struct environment* env, struct buffer* value, struct symbol* sym);

/* dynamic-output-none */
AKELA_API struct symbol* environment_get(struct environment* env, struct buffer* value);

/* dynamic-output-none */
AKELA_API struct symbol* environment_get_local(struct environment* env, struct buffer* value);

/* dynamic-output_none */
AKELA_API void symbol_init(struct symbol* sym);

/* dynamic-destroy env env{} */
AKELA_API void environment_destroy(struct environment* env);

AKELA_API void symbol_table_init(struct symbol_table* st);

AKELA_API void symbol_table_create(struct symbol_table** st);

AKELA_API void symbol_table_destroy(struct symbol_table* st);

AKELA_API bool symbol_table_is_global(struct symbol_table* st);

AKELA_API bool is_numeric(struct type_def* td);

AKELA_API bool type_find_whole(struct symbol_table* st, struct ast_node* a, struct ast_node* b);

AKELA_API bool type_def_can_cast(struct type_def* a, struct type_def* b);

AKELA_API bool type_use_can_cast(struct ast_node* a, struct ast_node* b);

AKELA_API void transfer_global_symbols(struct symbol_table* src, struct symbol_table* dest);

AKELA_API void transfer_module_symbols(struct environment* src, struct environment* dest, struct buffer* module_name);

AKELA_API void set_current_function(struct environment* env, struct ast_node* fd);
AKELA_API struct ast_node* get_current_function(struct environment* env);

#endif
