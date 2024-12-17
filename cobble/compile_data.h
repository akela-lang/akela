//
// Created by miguel on 6/6/23.
//

#ifndef COBBLE_COMPILE_DATA_H
#define COBBLE_COMPILE_DATA_H

#include "zinc/error.h"
#include "zinc/input_unicode.h"

typedef struct Compile_data {
    void* input_obj;
    InputUnicodeVTable* input_vtable;
    struct token* lookahead;
    struct error_list* el;
} Compile_data;

void compile_data_init(Compile_data* cd, void* input_obj, InputUnicodeVTable* input_vtable, struct error_list* el);
void compile_data_destroy(Compile_data* cd);

#endif //COBBLE_COMPILE_DATA_H
