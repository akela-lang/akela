#include "token.h"
#include "lex_tools.h"
#include "zinc/input_unicode.h"
#include "zinc/error.h"


void Json_lex_data_init(
    Json_lex_data* jld,
    struct error_list* el,
    void* input_obj,
    InputUnicodeVTable* input_vtable)
{
    jld->el = el;
    jld->input_obj = input_obj;
    jld->input_vtable = input_vtable;
}

void Json_lex_data_create(
    Json_lex_data** jld,
    struct error_list* el,
    void* input_obj,
    InputUnicodeVTable* input_vtable)
{
    malloc_safe((void**)jld, sizeof(Json_lex_data));
    Json_lex_data_init(*jld, el, input_obj, input_vtable);
}

bool Json_is_hex_digit(char c[4], int num)
{
    if (num != 1)
        return false;
    if (c[0] >= 'A' && c[0] <= 'Z') return true;
    if (c[0] >= 'a' && c[0] <= 'z') return true;
    if (c[0] >= '0' && c[0] <= '9') return true;
    return false;
}