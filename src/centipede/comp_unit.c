#include "comp_unit.h"
#include "zinc/memory.h"

void Cent_comp_unit_init(Cent_comp_unit *unit)
{
    unit->errors = NULL;
    unit->input = NULL;
    unit->input_vtable = NULL;
}

void Cent_comp_unit_create(Cent_comp_unit **unit)
{
    malloc_safe((void**)unit, sizeof(Cent_comp_unit));
    Cent_comp_unit_init(*unit);
}