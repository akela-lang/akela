#ifndef WORM_LEX_DATA_H
#define WORM_LEX_DATA_H

#include "zinc/error.h"
#include "zinc/input_unicode.h"

typedef struct {
    void* input;
    Zinc_input_unicode_vtable* vtable;
    Zinc_error_list* errors;
} Worm_lex_data;

void Worm_lex_data_init(
    Worm_lex_data* ld,
    void* input,
    Zinc_input_unicode_vtable* vtable,
    Zinc_error_list* errors);
void Worm_lex_data_create(
    Worm_lex_data** ld,
    void* input,
    Zinc_input_unicode_vtable* vtable,
    Zinc_error_list* errors);

#endif