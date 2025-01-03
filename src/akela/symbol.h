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

struct symbol {
    Symbol_type type;
    enum token_enum tk_type;
    struct type_def* td;
    struct Type_use* tu;
    struct symbol* constructor;
    struct Ake_ast* root;
    struct Ake_ast* root_ptr;
    void* value;
    void* reference;
    size_t assign_count;
};

AKELA_API void symbol_init(struct symbol* sym);
AKELA_API void symbol_create(struct symbol** sym);
AKELA_API struct symbol* symbol_copy(struct symbol* sym);

#endif