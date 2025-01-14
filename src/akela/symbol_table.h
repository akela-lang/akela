#ifndef AKELA_SYMBOL_TABLE_H
#define AKELA_SYMBOL_TABLE_H

#include "api.h"
#include <stdbool.h>
#include "zinc/hash_map_string.h"
#include "token.h"
#include <stdlib.h>
#include "ast.h"
#include "environment.h"
#include "type_use.h"

typedef struct Ake_type_use Ake_type_use;
typedef struct Ake_ast Ake_ast;

typedef struct Ake_symbol_table {
	struct Ake_environment* initial;
	struct Ake_environment* global;
	struct Ake_environment* top;
	struct Ake_type_use* numeric_pool;
    struct Ake_environment* deactivated;
    size_t id_count;
    struct Ake_type_def* function_type_def;
} Ake_symbol_table;

AKELA_API void Ake_environment_begin(struct Ake_symbol_table* st);
AKELA_API void Ake_environment_end(struct Ake_symbol_table* st);
AKELA_API void Ake_symbol_table_init(struct Ake_symbol_table* st);
AKELA_API void Ake_symbol_table_create(struct Ake_symbol_table** st);
AKELA_API void Ake_symbol_table_destroy(struct Ake_symbol_table* st);
AKELA_API bool Ake_symbol_table_is_global(struct Ake_symbol_table* st);
AKELA_API bool Ake_is_numeric(struct Ake_type_def* td);
AKELA_API bool Ake_type_find_whole(struct Ake_symbol_table* st, Ake_type_use* a, Ake_type_use* b);
AKELA_API bool Ake_type_def_can_cast(struct Ake_type_def* a, struct Ake_type_def* b);
AKELA_API bool Ake_type_use_can_cast(Ake_type_use* a, Ake_type_use* b);
AKELA_API void Ake_transfer_global_symbols(struct Ake_symbol_table* src, struct Ake_symbol_table* dest);
AKELA_API void Ake_transfer_module_symbols(struct Ake_environment* src, struct Ake_environment* dest, struct Zinc_string* module_name);
AKELA_API void Ake_set_current_function(struct Ake_environment* env, Ake_ast* fd);
AKELA_API Ake_ast* Ake_get_current_function(struct Ake_environment* env);
AKELA_API size_t Ake_symbol_table_generate_id(struct Ake_symbol_table* st);
AKELA_API void Ake_symbol_table_print(struct Ake_symbol_table* st);

#endif
