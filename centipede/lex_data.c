#include "lex_data.h"
#include "zinc/memory.h"

void Cent_lex_data_init(
    Cent_lex_data *ld,
    struct error_list* errors,
    void* input,
    InputUnicodeVTable* input_vtable)
{
    ld->errors = errors;
    ld->input = input;
    ld->input_vtable = input_vtable;
}

void Cent_lex_data_create(
    Cent_lex_data **ld,
    struct error_list* errors,
    void* input,
    InputUnicodeVTable* input_vtable)
{
    malloc_safe((void**)ld, sizeof(Cent_lex_data));
    Cent_lex_data_init(*ld, errors, input, input_vtable);
}