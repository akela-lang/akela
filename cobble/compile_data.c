#include "compile_data.h"
#include "zinc/memory.h"
#include "token.h"

void compile_data_init(
    Compile_data* cd,
    void* input_obj,
    InputUnicodeVTable* input_vtable,
    struct error_list* el)
{
    cd->input_obj = input_obj;
    cd->input_vtable = input_vtable;
    cd->lookahead = NULL;
    cd->el = el;
    cd->group_number = 0;
}

void compile_data_create(
    Compile_data** cd,
    void* input_obj,
    InputUnicodeVTable* input_vtable,
    struct error_list* el)
{
    malloc_safe((void**)cd, sizeof(Compile_data));
    compile_data_init(*cd, input_obj, input_vtable, el);
}

void compile_data_destroy(Compile_data* cd)
{
    free(cd->lookahead);
}
