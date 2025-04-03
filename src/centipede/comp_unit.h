#ifndef CENTIPEDE_COMP_UNIT_H
#define CENTIPEDE_COMP_UNIT_H

#include "zinc/input_unicode.h"
#include "zinc/hash_map_string.h"
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
    Zinc_input_unicode_vtable* vtable;
    Zinc_error_list errors;
    Cent_lex_data ld;
    Cent_parse_result pr;
    Cent_value* value;
    void* ct;
} Cent_comp_unit;

void Cent_comp_unit_init(
    Cent_comp_unit *cu,
    void* input,
    Zinc_input_unicode_vtable* input_vtable,
    Zinc_string_slice file_name,
    Cent_environment* base);
void Cent_comp_unit_create(
    Cent_comp_unit **cu,
    void* input,
    Zinc_input_unicode_vtable* input_vtable,
    Zinc_string_slice file_name,
    Cent_environment* base);
void Cent_comp_unit_destroy(Cent_comp_unit* cu);
void Cent_comp_unit_free(Cent_comp_unit* cu);
void Cent_comp_unit_parse(Cent_comp_unit* cu);
void Cent_comp_unit_build(Cent_comp_unit* cu);
void Cent_comp_unit_set_bounds(Cent_comp_unit* cu, Zinc_input_bounds* bounds);


#endif