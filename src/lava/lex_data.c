#include "lex_data.h"
#include "zinc/memory.h"

void Lava_lex_data_init(
    Lava_lex_data* ld,
    Zinc_error_list* errors,
    void* input,
    Zinc_input_unicode_vtable* vtable)
{
    ld->errors = errors;
    ld->input = input;
    ld->vtable = vtable;
    ld->header_re = Cob_compile_str("^(#+) *(.+) *\n?$");
}

void Lava_lex_data_create(
    Lava_lex_data** ld,
    Zinc_error_list* errors,
    void* input,
    Zinc_input_unicode_vtable* vtable)
{
    Zinc_malloc_safe((void**)ld, sizeof(Lava_lex_data));
    Lava_lex_data_init(*ld, errors, input, vtable);
}

void Lava_lex_data_destroy(Lava_lex_data* ld)
{
    Cob_re_destroy(&ld->header_re);
}