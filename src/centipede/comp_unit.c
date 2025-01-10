#include "comp_unit.h"
#include "zinc/memory.h"

void Cent_comp_unit_init(Cent_comp_unit *cu)
{
    buffer_init(&cu->name);
    cu->status = Cent_comp_unit_status_parse;
    cu->pd = NULL;
    cu->errors = NULL;
    cu->input = NULL;
    cu->input_vtable = NULL;
}

void Cent_comp_unit_create(Cent_comp_unit **cu)
{
    malloc_safe((void**)cu, sizeof(Cent_comp_unit));
    Cent_comp_unit_init(*cu);
}

void Cent_comp_unit_destroy(Cent_comp_unit* cu)
{
    buffer_destroy(&cu->name);
}