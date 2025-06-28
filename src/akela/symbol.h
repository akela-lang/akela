#ifndef AKELA_SYMBOL_H
#define AKELA_SYMBOL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Ake_Type Ake_Type;
#include "token.h"
#include "ast.h"
#include "type.h"

typedef enum Ake_symbol_type {
    Ake_symbol_type_none,
    Ake_symbol_type_reserved_word,
    Ake_symbol_type_type,
    Ake_symbol_type_variable,
    Ake_symbol_type_info,
} Ake_symbol_type;

typedef struct Ake_symbol {
    Ake_symbol_type type;
    Ake_token_enum tk_type;
    Ake_Type* td;
    Ake_Type* tu;
    Ake_ast* root;
    Ake_ast* root_ptr;
    void* value;
    void* reference;
    size_t assign_count;
    bool is_copy;
    void* struct_type;
    bool is_const;
} Ake_symbol;

AKELA_API void Ake_symbol_init(Ake_symbol* sym);
AKELA_API void Ake_symbol_create(Ake_symbol** sym);
AKELA_API void Ake_symbol_destroy(Ake_symbol* sym);
AKELA_API Ake_symbol* Ake_symbol_clone_shallow(Ake_symbol* sym);

#ifdef __cplusplus
}
#endif

#endif