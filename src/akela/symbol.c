#include "symbol.h"
#include "zinc/memory.h"
#include "type_def.h"

void Ake_symbol_init(struct Ake_symbol* sym)
{
    sym->type = Ake_symbol_type_none;
    sym->tk_type = Ake_token_none;
    sym->td = NULL;
    sym->tu = NULL;
    sym->constructor = NULL;
    sym->root = NULL;
    sym->root_ptr = NULL;
    sym->value = NULL;
    sym->reference = NULL;
    sym->assign_count = 0;
    sym->is_copy = false;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_symbol_destroy(Ake_symbol* sym)
{
    Ake_type_use_destroy(sym->tu);
    Ake_TypeDefDestroy(sym->td);
    free(sym->td);
    if (sym->constructor) {
        Ake_symbol_destroy(sym->constructor);
        free(sym->constructor);
    }
    Ake_ast_destroy(sym->root);
}

void Ake_symbol_create(struct Ake_symbol** sym)
{
    Zinc_malloc_safe((void**)sym, sizeof(struct Ake_symbol));
    Ake_symbol_init(*sym);
}

Ake_symbol* Ake_symbol_clone_shallow(struct Ake_symbol* sym)
{
    Ake_symbol* new_sym = NULL;
    if (sym) {
        Ake_symbol_create(&new_sym);
        new_sym->type = sym->type;
        new_sym->tk_type = sym->tk_type;
        new_sym->tu = sym->tu;
        new_sym->td = sym->td;
        new_sym->constructor = sym->constructor;
        new_sym->root = sym->root;
        new_sym->root_ptr = sym->root_ptr;
    }
    return new_sym;
}
