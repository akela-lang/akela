#include "comp_unit.h"
#include "zinc/memory.h"
#include "parse_data.h"
#include "parse.h"
#include "build.h"
#include "lex_data.h"
#include "check_parent.h"

void Cent_comp_unit_init(
    Cent_comp_unit *cu,
    void* input,
    Zinc_input_unicode_vtable* input_vtable,
    Zinc_string_slice file_name,
    void* module_finder_obj,
    Cent_module_vtable* module_finder_vtable,
    Cent_environment* base)
{
    cu->status = Cent_comp_unit_status_start;
    Zinc_error_list_init(&cu->errors);
    cu->input = input;
    cu->input_vtable = input_vtable;
    Cent_lex_data_init(&cu->ld, &cu->errors, input, input_vtable);
    Cent_parse_data_init(&cu->pd, &cu->errors, &cu->ld, file_name, base);
    cu->value = NULL;
}

void Cent_comp_unit_create(
    Cent_comp_unit **cu,
    void* input,
    Zinc_input_unicode_vtable* input_vtable,
    Zinc_string_slice file_name,
    void* module_finder_obj,
    Cent_module_vtable* module_finder_vtable,
    Cent_environment* base)
{
    Zinc_malloc_safe((void**)cu, sizeof(Cent_comp_unit));
    Cent_comp_unit_init(
        *cu,
        input,
        input_vtable,
        file_name,
        module_finder_obj,
        module_finder_vtable,
        base);
}

void Cent_comp_unit_destroy(Cent_comp_unit* cu)
{
    /* destroy value tree first while next and prev pointers are still valid */
    Cent_value_free(cu->value);

    Cent_ast_destroy(cu->pr.root);
    free(cu->pr.root);

    Zinc_error_list_destroy(&cu->errors);
    Cent_lex_data_destroy(&cu->ld);
}

void Cent_comp_unit_free(Cent_comp_unit* cu)
{
    Cent_comp_unit_destroy(cu);
    free(cu);
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
        Cent_check_parent(cu);
        cu->status = Cent_comp_unit_status_build;
    }
}