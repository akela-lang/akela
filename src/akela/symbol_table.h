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
#include "type.h"
#include "zinc/list.h"

typedef struct Ake_type_use Ake_type_use;
typedef struct Ake_Ast Ake_Ast;

typedef struct Ake_symbol_table {
	Ake_Environment* top;
	Ake_Environment* deactivated;
	Zinc_list numeric_pool;
    size_t id_count;
} Ake_symbol_table;

AKELA_API void Ake_begin_environment(struct Ake_symbol_table* st, Ake_Ast* n);
AKELA_API void Ake_end_environment(struct Ake_symbol_table* st);

AKELA_API void Ake_symbol_table_init(struct Ake_symbol_table* st);
AKELA_API void Ake_symbol_table_create(struct Ake_symbol_table** st);
AKELA_API void Ake_symbol_table_destroy(struct Ake_symbol_table* st);
AKELA_API bool Ake_is_numeric(Ake_Type* td);
AKELA_API void Ake_set_current_function(Ake_symbol_table* st, Ake_Ast* fd);
AKELA_API Ake_Ast* Ake_get_current_function(Ake_symbol_table* st);
AKELA_API size_t Ake_symbol_table_generate_id(struct Ake_symbol_table* st);

#ifdef __cplusplus
}
#endif

#endif
