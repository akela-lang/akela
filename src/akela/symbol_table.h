#ifndef AKELA_SYMBOL_TABLE_H
#define AKELA_SYMBOL_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "api.h"
#include <stdbool.h>
#include "zinc/hash_map_string.h"
#include "token.h"
#include <stdlib.h>
#include "ast.h"
#include "environment.h"
#include "type_use.h"
#include "zinc/list.h"

typedef struct Ake_type_use Ake_type_use;
typedef struct Ake_ast Ake_ast;

typedef struct Ake_symbol_table {
	Ake_Environment* top;
	Zinc_list numeric_pool;
    size_t id_count;
} Ake_symbol_table;

AKELA_API void Ake_begin_environment(struct Ake_symbol_table* st, Ake_ast* n);
AKELA_API void Ake_end_environment(struct Ake_symbol_table* st);

AKELA_API void Ake_symbol_table_init(struct Ake_symbol_table* st);
AKELA_API void Ake_symbol_table_create(struct Ake_symbol_table** st);
AKELA_API void Ake_symbol_table_destroy(struct Ake_symbol_table* st);
AKELA_API bool Ake_is_numeric(Ake_TypeDef* td);
AKELA_API bool Ake_type_find_whole(struct Ake_symbol_table* st, Ake_type_use* a, Ake_type_use* b);
AKELA_API bool Ake_type_def_can_cast(Ake_TypeDef* a, Ake_TypeDef* b);
AKELA_API bool Ake_type_use_can_cast(Ake_type_use* a, Ake_type_use* b);
AKELA_API void Ake_set_current_function(Ake_symbol_table* st, Ake_ast* fd);
AKELA_API Ake_ast* Ake_get_current_function(Ake_symbol_table* st);
AKELA_API size_t Ake_symbol_table_generate_id(struct Ake_symbol_table* st);
AKELA_API Ake_Environment* Ake_get_current_env(Ake_ast* n);

#ifdef __cplusplus
}
#endif

#endif
