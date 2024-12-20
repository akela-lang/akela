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

typedef struct Cob_re {
    struct error_list* el;
    Ast_node* root;
} Cob_re;

void compile_data_init(
    Compile_data* cd,
    void* input_obj,
    InputUnicodeVTable* input_vtable,
    struct error_list* el);
void compile_data_reeate(
    Compile_data** cd,
    void* input_obj,
    InputUnicodeVTable* input_vtable,
    struct error_list* el);
void compile_data_destroy(Compile_data* cd);

void Cob_re_init(Cob_re* re, struct error_list* el, Ast_node* root);
void Cob_re_reeate(Cob_re** re, struct error_list* el, Ast_node* root);
void Cob_re_destroy(Cob_re* re);

#endif //COBBLE_COMPILE_DATA_H
