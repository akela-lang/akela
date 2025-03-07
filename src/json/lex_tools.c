#include "lex_tools.h"
#include "zinc/input_unicode.h"
#include "zinc/error.h"
#include "zinc/memory.h"

enum Zinc_result Json_lex_data_init(
    Json_lex_data* ld,
    struct Zinc_error_list* el,
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable)
{
    ld->el = el;
    ld->input_obj = input_obj;
    ld->input_vtable = input_vtable;
    return Zinc_result_ok;
}

void Json_lex_data_create(
    Json_lex_data** ld,
    struct Zinc_error_list* el,
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable)
{
    Zinc_malloc_safe((void**)ld, sizeof(Json_lex_data));
    Json_lex_data_init(*ld, el, input_obj, input_vtable);
}

void Json_lex_data_destroy(Json_lex_data* ld)
{
}