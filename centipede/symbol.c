#include "symbol.h"

#include <akela/symbol.h>

void Cent_symbol_init(Cent_symbol *sym)
{
    sym->type = Symbol_type_none;
    sym->value = NULL;
}

void Cent_symbol_create(Cent_symbol** sym)
{
    malloc_safe((void**)sym, sizeof(Cent_symbol));
    Cent_symbol_init(*sym);
}

void Cent_symbol_destroy(Cent_symbol *sym)
{
}