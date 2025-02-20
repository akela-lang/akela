#ifndef CENTIPEDE_MODULE_STRING_H
#define CENTIPEDE_MODULE_STRING_H

#include "comp_unit.h"
#include "module_finder.h"

typedef struct Cent_module_finder_string {
    Zinc_hash_map_string ht;
    Cent_module_finder_find_interface find;
    Cent_module_finder_destroy_interface destroy;
    Cent_module_finder_vtable* vtable;
} Cent_module_finder_string;

extern Cent_module_finder_vtable Cent_module_string_vtable;

void Cent_module_finder_string_init(Cent_module_finder_string* ms);
void Cent_module_finder_string_create(Cent_module_finder_string** ms);
void Cent_module_finder_string_destroy(Cent_module_finder_string* ms);
void Cent_module_finder_string_add_module(
    Cent_module_finder_string* ms,
    struct Zinc_string* name,
    struct Zinc_string* text);
void Cent_module_finder_string_add_module_str(
    Cent_module_finder_string* ms,
    char* name,
    struct Zinc_string* text);
void Cent_module_finder_string_add_module_str_str(
    Cent_module_finder_string* ms,
    char* name,
    char* text);
Cent_input_data Cent_module_finder_string_find(Cent_module_finder_string* ms, struct Zinc_string* name);

#endif
