//
// Created by miguel on 6/6/23.
//

#ifndef COBBLE_COMPILE_DATA_H
#define COBBLE_COMPILE_DATA_H

#include "zinc/error.h"
#include "zinc/input_unicode.h"
#include "Ast_node.h"

typedef struct Compile_data {
    void* input_obj;
    InputUnicodeVTable* input_vtable;
    struct token* lookahead;
    struct error_list* el;
    size_t group_number;
} Compile_data;

typedef struct Cob_compile_result {
    struct error_list* el;
    Ast_node* root;
} Cob_compile_result;

void compile_data_init(
    Compile_data* cd,
    void* input_obj,
    InputUnicodeVTable* input_vtable,
    struct error_list* el);
void compile_data_create(
    Compile_data** cd,
    void* input_obj,
    InputUnicodeVTable* input_vtable,
    struct error_list* el);
void compile_data_destroy(Compile_data* cd);

void Cob_compile_result_init(Cob_compile_result* cr, struct error_list* el, Ast_node* root);
void Cob_compile_result_create(Cob_compile_result** cr, struct error_list* el, Ast_node* root);
void Cob_compile_result_destroy(Cob_compile_result* cr);

#endif //COBBLE_COMPILE_DATA_H
