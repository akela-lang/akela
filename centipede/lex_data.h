#ifndef CENTIPEDE_LEX_DATA_H
#define CENTIPEDE_LEX_DATA_H

#include "zinc/error.h"
#include "zinc/input_unicode.h"

typedef struct Cent_lex_data {
    struct error_list* errors;
    void* input;
    InputUnicodeVTable* vtable;
} Cent_lex_data;

#endif