#include "comp_unit.h"
#include "zinc/memory.h"
#include "parse_data.h"
#include "parse.h"
#include "build.h"
#include "lex_data.h"

void Cent_comp_unit_init(
    Cent_comp_unit *cu,
    void* input,
    InputUnicodeVTable* input_vtable,
    String_slice file_name,
    void* module_finder_obj,
    Cent_module_vtable* module_finder_vtable)
{
    cu->status = Cent_comp_unit_status_start;
    error_list_init(&cu->errors);
    cu->input = input;
    cu->input_vtable = input_vtable;
    Cent_lex_data_init(&cu->ld, &cu->errors, input, input_vtable);
    Cent_parse_data_init(&cu->pd, &cu->errors, &cu->ld, file_name);
    cu->value = NULL;
}

void Cent_comp_unit_create(
    Cent_comp_unit **cu,
    void* input,
    InputUnicodeVTable* input_vtable,
    String_slice file_name,
    void* module_finder_obj,
    Cent_module_vtable* module_finder_vtable)
{
    malloc_safe((void**)cu, sizeof(Cent_comp_unit));
    Cent_comp_unit_init(*cu, input, input_vtable, file_name, module_finder_obj, module_finder_vtable);
}

void Cent_comp_unit_destroy(Cent_comp_unit* cu)
{
    error_list_destroy(&cu->errors);
    Cent_lex_data_destroy(&cu->ld);
}

void Cent_comp_unit_parse(Cent_comp_unit* cu)
{
    if (cu->status == Cent_comp_unit_status_start) {
        cu->pr = Cent_parse(&cu->pd);
        cu->status = Cent_comp_unit_status_parse;
    }
}

void Cent_comp_unit_build(Cent_comp_unit* cu)
{
    if (cu->status == Cent_comp_unit_status_parse) {
        cu->value = Cent_build(&cu->pr);
        cu->status = Cent_comp_unit_status_build;
    }
}