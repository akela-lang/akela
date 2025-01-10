#ifndef CENTIPEDE_COMP_UNIT_H
#define CENTIPEDE_COMP_UNIT_H

#include "zinc/input_unicode.h"
#include "zinc/hash.h"

typedef enum Cent_comp_unit_status {
    Cent_comp_unit_status_parse,
    Cent_comp_unit_status_build,
} Cent_comp_unit_status;

typedef struct Cent_comp_unit {
    struct buffer name;
    Cent_comp_unit_status status;
    void* pd;
    void* input;
    InputUnicodeVTable* input_vtable;
    struct error_list* errors;
} Cent_comp_unit;

void Cent_comp_unit_init(Cent_comp_unit *cu);
void Cent_comp_unit_create(Cent_comp_unit **cu);
void Cent_comp_unit_destroy(Cent_comp_unit* cu);

#endif