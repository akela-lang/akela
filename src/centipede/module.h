#ifndef CENTIPEDE_MODULE_H
#define CENTIPEDE_MODULE_H

#include "comp_unit.h"

typedef Cent_comp_unit* (*Cent_module_find)(void* obj, struct buffer* name);

typedef struct Cent_module_vtable {
    u_int8_t find_offset;
} Cent_module_vtable;

Cent_comp_unit* Cent_module_find_interface(void* obj, Cent_module_vtable *vtable, struct buffer* name);

#endif