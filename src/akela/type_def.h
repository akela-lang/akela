#ifndef AKELA_TYPE_DEF_H
#define AKELA_TYPE_DEF_H

#include "api.h"
#include <stdbool.h>
#include "zinc/zstring.h"
#include "zinc/result.h"
#include "token.h"
#include "symbol_table.h"
#include "zinc/vector.h"
#include "ast.h"
#include "zinc/hash.h"
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
	enum Ake_type type;
	struct Zinc_string name;
    int bit_count;
    bool is_signed;
    struct Ake_ast* composite;
    void* composite_type;
    struct hash_table struct_impl;
    struct hash_table type_impl;
} Ake_type_def;

#define IMPL_HASH_SIZE 32

AKELA_API void Ake_type_def_create(struct Ake_type_def** n);
AKELA_API void Ake_type_def_destroy(struct Ake_type_def* n);
AKELA_API void Ake_type_def_init(struct Ake_type_def* n);
AKELA_API struct Ake_type_def* Ake_type_def_copy(struct Ake_type_def* n);
AKELA_API bool Ake_type_def_match(struct Ake_type_def* a, struct Ake_type_def* b);

#endif
