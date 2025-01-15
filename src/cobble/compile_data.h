#ifndef COBBLE_COMPILE_DATA_H
#define COBBLE_COMPILE_DATA_H

#include "zinc/error.h"
#include "zinc/input_unicode.h"
#include "ast.h"
#include "token.h"

typedef struct Cob_compile_data {
    void* input_obj;
    Zinc_input_unicode_vtable* input_vtable;
    Cob_token* lookahead;
    struct Zinc_error_list* el;
    size_t group_number;
} Cob_compile_data;

typedef struct Cob_re {
    struct Zinc_error_list* el;
    Cob_ast* root;
    size_t group_count;
} Cob_re;

void Cob_compile_data_init(
    Cob_compile_data* cd,
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable,
    struct Zinc_error_list* el);
void Cob_compile_data_create(
    Cob_compile_data** cd,
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable,
    struct Zinc_error_list* el);
void Cob_compile_data_destroy(Cob_compile_data* cd);

void Cob_re_init(Cob_re* re, struct Zinc_error_list* el, Cob_ast* root);
void Cob_re_reeate(Cob_re** re, struct Zinc_error_list* el, Cob_ast* root);
void Cob_re_destroy(Cob_re* re);

#endif //COBBLE_COMPILE_DATA_H
