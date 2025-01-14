#ifndef AKELA_SYMBOL_H
#define AKELA_SYMBOL_H

#include "token.h"
#include "ast.h"

typedef enum Ake_symbol_type {
    Ake_symbol_type_none,
    Ake_symbol_type_reserved_word,
    Ake_symbol_type_type,
    Ake_symbol_type_variable,
    Ake_symbol_type_info,
} Ake_symbol_type;

typedef struct Ake_symbol {
    Ake_symbol_type type;
    enum Ake_token_enum tk_type;
    struct Ake_type_def* td;
    struct Ake_type_use* tu;
    struct Ake_symbol* constructor;
    struct Ake_ast* root;
    struct Ake_ast* root_ptr;
    void* value;
    void* reference;
    size_t assign_count;
} Ake_symbol;

AKELA_API void Ake_symbol_init(struct Ake_symbol* sym);
AKELA_API void Ake_symbol_create(struct Ake_symbol** sym);
AKELA_API struct Ake_symbol* Ake_symbol_copy(struct Ake_symbol* sym);

#endif