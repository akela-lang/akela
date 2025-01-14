#ifndef AKELA_SYMBOL_H
#define AKELA_SYMBOL_H

#include "token.h"
#include "ast.h"

typedef enum Symbol_type {
    Symbol_type_none,
    Symbol_type_reserved_word,
    Symbol_type_type,
    Symbol_type_variable,
    Symbol_type_info,
} Symbol_type;

struct Ake_symbol {
    Symbol_type type;
    enum token_enum tk_type;
    struct type_def* td;
    struct Type_use* tu;
    struct Ake_symbol* constructor;
    struct Ake_ast* root;
    struct Ake_ast* root_ptr;
    void* value;
    void* reference;
    size_t assign_count;
};

AKELA_API void symbol_init(struct Ake_symbol* sym);
AKELA_API void symbol_create(struct Ake_symbol** sym);
AKELA_API struct Ake_symbol* symbol_copy(struct Ake_symbol* sym);

#endif