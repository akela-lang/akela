#ifndef CENTIPEDE_COMP_UNIT_H
#define CENTIPEDE_COMP_UNIT_H

#include "zinc/input_unicode.h"
#include "zinc/hash.h"
#include "parse_data.h"
#include "lex_data.h"

typedef enum Cent_comp_unit_status {
    Cent_comp_unit_status_start,
    Cent_comp_unit_status_parse,
    Cent_comp_unit_status_build,
} Cent_comp_unit_status;

typedef struct Cent_comp_unit {
    Cent_comp_unit_status status;
    Cent_parse_data pd;
    void* input;
    InputUnicodeVTable* input_vtable;
    struct error_list errors;
    Cent_lex_data ld;
    Cent_parse_result pr;
    Cent_value* value;
    void* ct;
} Cent_comp_unit;

void Cent_comp_unit_init(
    Cent_comp_unit *cu,
    void* input,
    InputUnicodeVTable* input_vtable,
    String_slice file_name,
    void* module_finder_obj,
    Cent_module_vtable* module_finder_vtable,
    Cent_environment* base);
void Cent_comp_unit_create(
    Cent_comp_unit **cu,
    void* input,
    InputUnicodeVTable* input_vtable,
    String_slice file_name,
    void* module_finder_obj,
    Cent_module_vtable* module_finder_vtable,
    Cent_environment* base);
void Cent_comp_unit_destroy(Cent_comp_unit* cu);
void Cent_comp_unit_parse(Cent_comp_unit* cu);
void Cent_comp_unit_build(Cent_comp_unit* cu);


#endif