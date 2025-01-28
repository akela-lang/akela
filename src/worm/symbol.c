#include "symbol.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "zinc/memory.h"

void Worm_symbol_init(Worm_symbol *sym)
{
    sym->type = Worm_symbol_type_none;
    sym->is_copy = false;
}

void Worm_symbol_create(Worm_symbol** sym)
{
    Zinc_malloc_safe((void**)sym, sizeof(Worm_symbol));
    Worm_symbol_init(*sym);
}

void Worm_symbol_set_type(Worm_symbol *sym, Worm_symbol_type type)
{
    sym->type = type;

    if (type == Worm_symbol_type_variable) {
        sym->data.variable.n = NULL;
        //sym->data.variable.value = NULL;
    //} else if (type == Worm_symbol_type_element) {
        //sym->data.element = NULL;
    //} else if (type == Worm_symbol_type_enumerate) {
        //sym->data.enumerate = NULL;
    //} else if (type == Worm_symbol_type_object_value) {
        //sym->data.object_value = NULL;
    } else if (type == Worm_symbol_type_file_name) {
        sym->data.file_name.p = NULL;
        sym->data.file_name.size = 0;
    //} else if (type == Worm_symbol_type_module) {
        //sym->data.module = NULL;
    } else {
        assert(false && "invalid symbol type");
    }
}

void Worm_symbol_destroy(Worm_symbol *sym)
{
    if (!sym->is_copy) {
        //if (sym->type == Worm_symbol_type_element) {
            //if (sym->data.element) {
                //Worm_element_destroy(sym->data.element);
                //free(sym->data.element);
            //}
        //} else if (sym->type == Worm_symbol_type_enumerate) {
            //if (sym->data.enumerate) {
                //Worm_enumerate_destroy(sym->data.enumerate);
                //free(sym->data.enumerate);
            //}
        //} else if (sym->type == Worm_symbol_type_variable) {
            //if (sym->data.variable.value) {
                //Worm_value_free(sym->data.variable.value);
            //}
        //} else if (sym->type == Worm_symbol_type_module) {
            //if (sym->data.module) {
                //Worm_module_free(sym->data.module);
            //}
        //}
    }
}

void Worm_symbol_free(Worm_symbol *sym)
{
    Worm_symbol_destroy(sym);
    free(sym);
}

/* if should copy symbol for glob module import */
bool Worm_symbol_should_copy(Worm_symbol *sym)
{
    //if (sym->type == Worm_symbol_type_variable) {
        //return true;
    //}

    //if (sym->type == Worm_symbol_type_element) {
     //   return true;
    //}

    //if (sym->type == Worm_symbol_type_enumerate) {
        //return true;
    //}

    //if (sym->type == Worm_symbol_type_object_value) {
        //return true;
    //}

    //if (sym->type == Worm_symbol_type_module) {
        //return true;
    //}

    return false;
}

void Worm_symbol_copy_shallow(Worm_symbol *src, Worm_symbol *dest)
{
    *dest = *src;
    dest->is_copy = true;
}

Worm_symbol* Worm_symbol_clone_shallow(Worm_symbol *sym)
{
    Worm_symbol *new_sym = NULL;
    Zinc_malloc_safe((void**)&new_sym, sizeof(Worm_symbol));
    Worm_symbol_copy_shallow(sym, new_sym);
    return new_sym;
}
