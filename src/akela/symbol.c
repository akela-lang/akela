#include "symbol.h"
#include "zinc/memory.h"
#include "type.h"

void Ake_SymbolInit(struct Ake_Symbol* sym)
{
    sym->kind = AKE_SYMBOL_NONE;
    sym->tk_type = Ake_token_none;
    sym->td = NULL;
    sym->tu = NULL;
    sym->root = NULL;
    sym->root_ptr = NULL;
    sym->value = NULL;
    sym->reference = NULL;
    sym->assign_count = 0;
    sym->is_copy = false;
    sym->struct_type = NULL;
    sym->is_const = false;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_SymbolDestroy(Ake_symbol* sym)
{
    Ake_TypeDestroy(sym->tu);
    free(sym->tu);
    Ake_TypeDestroy(sym->td);
    free(sym->td);
    Ake_AstDestroy(sym->root);
}

void Ake_SymbolCreate(struct Ake_Symbol** sym)
{
    Zinc_malloc_safe((void**)sym, sizeof(struct Ake_Symbol));
    Ake_SymbolInit(*sym);
}

Ake_symbol* Ake_SymbolCloneShallow(struct Ake_Symbol* sym)
{
    Ake_symbol* new_sym = NULL;
    if (sym) {
        Ake_SymbolCreate(&new_sym);
        new_sym->kind = sym->kind;
        new_sym->tk_type = sym->tk_type;
        new_sym->tu = sym->tu;
        new_sym->td = sym->td;
        new_sym->root = sym->root;
        new_sym->root_ptr = sym->root_ptr;
        new_sym->is_copy = true;
    }
    return new_sym;
}
