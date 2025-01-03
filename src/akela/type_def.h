#ifndef AKELA_TYPE_DEF_H
#define AKELA_TYPE_DEF_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "symbol_table.h"
#include "zinc/vector.h"
#include "ast.h"
#include "zinc/hash.h"
#include "ast.h"

enum type {
	type_none,
	type_integer,
	type_float,
    type_boolean,
    type_struct,
    type_function,
    type_enum,
    type_tuple,
    type_module,
};

struct type_def {
	enum type type;
	struct buffer name;
    int bit_count;
    bool is_signed;
    struct Ake_ast* composite;
    void* composite_type;
    struct hash_table struct_impl;
    struct hash_table type_impl;
};

#define IMPL_HASH_SIZE 32

AKELA_API void type_def_create(struct type_def** n);
AKELA_API void type_def_destroy(struct type_def* n);
AKELA_API void type_def_init(struct type_def* n);
AKELA_API struct type_def* type_def_copy(struct type_def* n);
AKELA_API bool type_def_match(struct type_def* a, struct type_def* b);

#endif
