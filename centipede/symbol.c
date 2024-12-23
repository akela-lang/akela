#include "symbol.h"

#include <akela/symbol.h>

void Cent_symbol_init(Cent_symbol *sym)
{
    sym->type = Symbol_type_none;
    sym->reserved_token_type = Cent_token_none;
    buffer_init(&sym->value);
}

void Cent_symbol_create(Cent_symbol** sym)
{
    malloc_safe((void**)sym, sizeof(Cent_symbol));
    Cent_symbol_init(*sym);
}

void Cent_symbol_destroy(Cent_symbol *sym)
{
    buffer_destroy(&sym->value);
}