#ifndef LAVA_LEX_DATA_H
#define LAVA_LEX_DATA_H

#include <cobble/compile.h>
#include "zinc/input_unicode.h"

typedef struct Lava_lex_data Lava_lex_data;
struct Lava_lex_data {
    void* input;
    Zinc_input_unicode_vtable* vtable;
    Zinc_error_list* errors;
    Cob_re header_re;
};

void Lava_lex_data_init(
    Lava_lex_data* ld,
    Zinc_error_list* errors,
    void* input,
    Zinc_input_unicode_vtable* vtable);
void Lava_lex_data_create(
    Lava_lex_data** ld,
    Zinc_error_list* errors,
    void* input,
    Zinc_input_unicode_vtable* vtable);

#endif