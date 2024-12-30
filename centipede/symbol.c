#include "symbol.h"

#include <akela/symbol.h>

void Cent_symbol_init(Cent_symbol *sym)
{
    sym->type = Symbol_type_none;
}

void Cent_symbol_create(Cent_symbol** sym)
{
    malloc_safe((void**)sym, sizeof(Cent_symbol));
    Cent_symbol_init(*sym);
}

void Cent_symbol_destroy(Cent_symbol *sym)
{
    if (sym->type == Cent_symbol_type_element) {
        if (sym->data.element) {
            Cent_element_destroy(sym->data.element);
            free(sym->data.element);
        }
    } else if (sym->type == Cent_symbol_type_enumerate) {
        if (sym->data.enumerate) {
            Cent_enumerate_destroy(sym->data.enumerate);
            free(sym->data.enumerate);
        }
    }
}

void Cent_symbol_free(Cent_symbol *sym)
{
    Cent_symbol_destroy(sym);
    free(sym);
}