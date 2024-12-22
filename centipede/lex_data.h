#ifndef CENTIPEDE_LEX_DATA_H
#define CENTIPEDE_LEX_DATA_H

#include "zinc/error.h"
#include "zinc/input_unicode.h"

typedef struct Cent_lex_data {
    struct error_list* errors;
    void* input;
    InputUnicodeVTable* input_vtable;
} Cent_lex_data;

void Cent_lex_data_init(
    Cent_lex_data *ld,
    struct error_list* errors,
    void* input,
    InputUnicodeVTable* input_vtable);
void Cent_lex_data_create(
    Cent_lex_data **ld,
    struct error_list* errors,
    void* input,
    InputUnicodeVTable* input_vtable);

#endif