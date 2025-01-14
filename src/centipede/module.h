#ifndef CENTIPEDE_MODULE_H
#define CENTIPEDE_MODULE_H

#include "zinc/input_unicode.h"

typedef struct Cent_module_vtable {
    u_int8_t find_offset;
} Cent_module_vtable;

typedef struct Cent_input_data {
    void* input;
    InputUnicodeVTable* input_vtable;
} Cent_input_data;

typedef Cent_input_data (*Cent_module_find)(void* obj, struct Zinc_string* name);
Cent_input_data Cent_module_find_interface(void* obj, Cent_module_vtable *vtable, struct Zinc_string* name);

#endif