#ifndef AKELA_SYMBOL_H
#define AKELA_SYMBOL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Ake_Type Ake_Type;
#include "token.h"
#include "ast.h"
#include "type.h"

typedef enum Ake_SymbolKind {
    AKE_SYMBOL_NONE,
    AKE_SYMBOL_RESERVED_WORD,
    AKE_SYMBOL_TYPE,
    AKE_SYMBOL_VARIABLE,
    AKE_SYMBOL_INFO,
} Ake_SymbolKind;

typedef struct Ake_Symbol {
    Ake_SymbolKind kind;
    Ake_token_enum tk_type;
    Ake_Type* td;
    Ake_Type* tu;
    Ake_Ast* root;
    Ake_Ast* root_ptr;
    void* value;
    void* reference;
    size_t assign_count;
    bool is_copy;
    void* struct_type;
    bool is_const;
} Ake_symbol;

AKELA_API void Ake_SymbolInit(Ake_symbol* sym);
AKELA_API void Ake_SymbolCreate(Ake_symbol** sym);
AKELA_API void Ake_SymbolDestroy(Ake_symbol* sym);
AKELA_API Ake_symbol* Ake_SymbolCloneShallow(Ake_symbol* sym);

#ifdef __cplusplus
}
#endif

#endif