#ifndef AKELA_SYMBOL_TABLE_H
#define AKELA_SYMBOL_TABLE_H

#include "api.h"
#include <stdbool.h>
#include "zinc/hash.h"
#include "token.h"
#include <stdlib.h>
#include "ast.h"
#include "environment.h"
#include "type_use.h"

typedef struct Type_use Type_use;
typedef struct Ake_ast Ake_ast;

struct symbol_table {
	struct environment* initial;
	struct environment* global;
	struct environment* top;
	struct Type_use* numeric_pool;
    struct environment* deactivated;
    size_t id_count;
    struct type_def* function_type_def;
};

AKELA_API void environment_begin(struct symbol_table* st);
AKELA_API void environment_end(struct symbol_table* st);
AKELA_API void symbol_table_init(struct symbol_table* st);
AKELA_API void symbol_table_create(struct symbol_table** st);
AKELA_API void symbol_table_destroy(struct symbol_table* st);
AKELA_API bool symbol_table_is_global(struct symbol_table* st);
AKELA_API bool is_numeric(struct type_def* td);
AKELA_API bool type_find_whole(struct symbol_table* st, Type_use* a, Type_use* b);
AKELA_API bool type_def_can_cast(struct type_def* a, struct type_def* b);
AKELA_API bool type_use_can_cast(Type_use* a, Type_use* b);
AKELA_API void transfer_global_symbols(struct symbol_table* src, struct symbol_table* dest);
AKELA_API void transfer_module_symbols(struct environment* src, struct environment* dest, struct buffer* module_name);
AKELA_API void set_current_function(struct environment* env, Ake_ast* fd);
AKELA_API Ake_ast* get_current_function(struct environment* env);
AKELA_API size_t symbol_table_generate_id(struct symbol_table* st);
AKELA_API void symbol_table_print(struct symbol_table* st);

#endif
