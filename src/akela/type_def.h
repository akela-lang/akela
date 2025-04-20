#ifndef AKELA_TYPE_DEF_H
#define AKELA_TYPE_DEF_H

typedef struct Ake_ast Ake_ast;
typedef struct Ake_type_use Ake_type_use;

#include "api.h"
#include <stdbool.h>
#include "zinc/zstring.h"
#include "zinc/result.h"
#include "token.h"
#include "symbol_table.h"
#include "zinc/vector.h"
#include "ast.h"
#include "zinc/hash_map_string.h"
#include "ast.h"

typedef enum Ake_type {
	Ake_type_none,
	Ake_type_integer,
	Ake_type_float,
    Ake_type_boolean,
    Ake_type_struct,
    Ake_type_function,
    Ake_type_enum,
    Ake_type_tuple,
    Ake_type_module,
} Ake_type;

typedef struct Ake_type_def {
	Ake_type type;
	Zinc_string name;
    int bit_count;
    bool is_signed;
    Ake_ast* composite;
    void* composite_type;
    Zinc_hash_map_string struct_impl;
    Zinc_hash_map_string type_impl;
} Ake_type_def;

#define IMPL_HASH_SIZE 32

AKELA_API void Ake_type_def_create(Ake_type_def** n);
AKELA_API void Ake_type_def_destroy(Ake_type_def* n);
AKELA_API void Ake_type_def_init(Ake_type_def* n);
AKELA_API Ake_type_def* Ake_type_def_copy(Ake_type_def* n);
AKELA_API bool Ake_type_def_match(Ake_type_def* a, Ake_type_def* b);

#endif
