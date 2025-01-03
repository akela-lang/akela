#include "symbol.h"

#include <akela/symbol.h>
#include <assert.h>

void Cent_symbol_init(Cent_symbol *sym)
{
    sym->type = Symbol_type_none;
}

void Cent_symbol_create(Cent_symbol** sym)
{
    malloc_safe((void**)sym, sizeof(Cent_symbol));
    Cent_symbol_init(*sym);
}

void Cent_symbol_set_type(Cent_symbol *sym, Cent_symbol_type type)
{
    sym->type = type;

    if (type == Cent_symbol_type_variable) {
        sym->data.variable.n = NULL;
        sym->data.variable.value = NULL;
    } else if (type == Cent_symbol_type_element) {
        sym->data.element = NULL;
    } else if (type == Cent_symbol_type_enumerate) {
        sym->data.enumerate = NULL;
    } else if (type == Cent_symbol_type_object_value) {
        sym->data.object_value = NULL;
    } else if (type == Cent_symbol_type_file_name) {
        sym->data.file_name.p = NULL;
        sym->data.file_name.size = 0;
    } else {
        assert(false && "invalid symbol type");
    }
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
    } else if (sym->type == Cent_symbol_type_variable) {
        if (sym->data.variable.value) {
            Cent_value_free(sym->data.variable.value);
        }
    }
}

void Cent_symbol_free(Cent_symbol *sym)
{
    Cent_symbol_destroy(sym);
    free(sym);
}