#include "lex_data.h"
#include "zinc/memory.h"

void Worm_lex_data_init(
    Worm_lex_data* ld,
    void* input,
    Zinc_input_unicode_vtable* vtable,
    Zinc_error_list* errors)
{
    ld->input = input;
    ld->vtable = vtable;
    ld->errors = errors;
}

void Worm_lex_data_create(
    Worm_lex_data** ld,
    void* input,
    Zinc_input_unicode_vtable* vtable,
    Zinc_error_list* errors)
{
    Zinc_malloc_safe((void**)ld, sizeof(Worm_lex_data));
    Worm_lex_data_init(*ld, input, vtable, errors);
}