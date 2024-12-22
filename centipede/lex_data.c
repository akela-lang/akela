#include "lex_data.h"
#include "zinc/memory.h"

void lex_data_init(Cent_lex_data *ld)
{
    ld->errors = NULL;
    ld->input = NULL;
    ld->vtable = NULL;
}

void lex_data_create(Cent_lex_data **ld)
{
    malloc_safe((void**)ld, sizeof(Cent_lex_data));
    lex_data_init(*ld);
}