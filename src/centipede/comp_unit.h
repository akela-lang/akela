#ifndef CENTIPEDE_COMP_UNIT_H
#define CENTIPEDE_COMP_UNIT_H

#include "zinc/input_unicode.h"
#include "zinc/hash.h"

typedef struct Cent_comp_unit {
    void* input;
    InputUnicodeVTable* input_vtable;
    struct error_list* errors;
} Cent_comp_unit;

void Cent_comp_unit_init(Cent_comp_unit *unit);
void Cent_comp_unit_create(Cent_comp_unit **unit);

#endif