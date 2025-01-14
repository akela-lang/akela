#include "symbol.h"
#include "zinc/memory.h"
#include "type_def.h"

void symbol_init(struct Ake_symbol* sym)
{
    sym->type = Symbol_type_none;
    sym->tk_type = token_none;
    sym->td = NULL;
    sym->tu = NULL;
    sym->constructor = NULL;
    sym->root = NULL;
    sym->root_ptr = NULL;
    sym->value = NULL;
    sym->reference = NULL;
    sym->assign_count = 0;
}

void symbol_create(struct Ake_symbol** sym)
{
    malloc_safe((void**)sym, sizeof(struct Ake_symbol));
    symbol_init(*sym);
}

struct Ake_symbol* symbol_copy(struct Ake_symbol* sym)
{
    struct Ake_symbol* new_sym = NULL;
    if (sym) {
        malloc_safe((void**)&new_sym, sizeof(struct Ake_symbol));
        symbol_init(new_sym);
        new_sym->type = sym->type;
        new_sym->tk_type = sym->tk_type;
        new_sym->tu = Type_use_clone(sym->tu);
        new_sym->td = type_def_copy(sym->td);
        new_sym->constructor = symbol_copy(sym->constructor);
        new_sym->root = Ake_ast_clone(sym->root);
        new_sym->root_ptr = sym->root_ptr;
    }
    return new_sym;
}
