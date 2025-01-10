#ifndef CENTIPEDE_MODULE_STRING_H
#define CENTIPEDE_MODULE_STRING_H

#include "comp_unit.h"
#include "module.h"

typedef struct Cent_module_string {
    struct hash_table ht;
    Cent_module_find find;
    Cent_module_vtable* vtable;
} Cent_module_string;

extern Cent_module_vtable Cent_module_string_vtable;

void Cent_module_string_init(Cent_module_string* ms);
void Cent_module_string_create(Cent_module_string** ms);
void Cent_module_string_destroy(Cent_module_string* ms);
void Cent_module_string_add_module(Cent_module_string* ms, struct buffer* name, struct buffer* text);
void Cent_module_string_add_module_str(Cent_module_string* ms, char* name, struct buffer* text);
void Cent_module_string_add_module_str_str(Cent_module_string* ms, char* name, char* text);
Cent_input_data Cent_module_find_string(Cent_module_string* ms, struct buffer* name);

#endif