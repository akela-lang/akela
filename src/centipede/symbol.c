#include "symbol.h"

#include <akela/symbol.h>
#include <assert.h>

void Cent_symbol_init(Cent_symbol *sym)
{
    sym->type = Ake_symbol_type_none;
    sym->is_copy = false;
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
    } else if (type == Cent_symbol_type_module) {
        sym->data.module = NULL;
    } else {
        assert(false && "invalid symbol type");
    }
}

void Cent_symbol_destroy(Cent_symbol *sym)
{
    if (!sym->is_copy) {
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
        } else if (sym->type == Cent_symbol_type_module) {
            if (sym->data.module) {
                Cent_module_free(sym->data.module);
            }
        }
    }
}

void Cent_symbol_free(Cent_symbol *sym)
{
    Cent_symbol_destroy(sym);
    free(sym);
}

/* if should copy symbol for glob module import */
bool Cent_symbol_should_copy(Cent_symbol *sym)
{
    if (sym->type == Cent_symbol_type_variable) {
        return true;
    }

    if (sym->type == Cent_symbol_type_element) {
        return true;
    }

    if (sym->type == Cent_symbol_type_enumerate) {
        return true;
    }

    if (sym->type == Cent_symbol_type_object_value) {
        return true;
    }

    if (sym->type == Cent_symbol_type_module) {
        return true;
    }

    return false;
}

void Cent_symbol_copy_shallow(Cent_symbol *src, Cent_symbol *dest)
{
    *dest = *src;
    dest->is_copy = true;
}

Cent_symbol* Cent_symbol_clone_shallow(Cent_symbol *sym)
{
    Cent_symbol *new_sym = NULL;
    malloc_safe((void**)&new_sym, sizeof(Cent_symbol));
    Cent_symbol_copy_shallow(sym, new_sym);
    return new_sym;
}