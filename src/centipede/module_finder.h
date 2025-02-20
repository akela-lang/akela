#ifndef CENTIPEDE_MODULE_H
#define CENTIPEDE_MODULE_H

#include "zinc/input_unicode.h"

typedef struct Cent_module_finder_vtable {
    u_int8_t find_offset;
    u_int8_t destroy_offset;
} Cent_module_finder_vtable;

typedef struct Cent_input_data {
    void* input;
    Zinc_input_unicode_vtable* input_vtable;
} Cent_input_data;

typedef Cent_input_data (*Cent_module_finder_find_interface)(void* obj, Zinc_string* name);
typedef void (*Cent_module_finder_destroy_interface)(void* obj);

Cent_input_data Cent_module_finder_find(void* obj, Cent_module_finder_vtable *vtable, Zinc_string* name);
void Cent_module_finder_destroy(void* obj, Cent_module_finder_vtable *vtable);

#endif