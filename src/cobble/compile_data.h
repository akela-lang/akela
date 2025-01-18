#ifndef COBBLE_COMPILE_DATA_H
#define COBBLE_COMPILE_DATA_H

#include "zinc/error.h"
#include "zinc/input_unicode.h"
#include "ast.h"
#include "token.h"

typedef struct Cob_compile_data {
    Zinc_string* text;
    Cob_token* lookahead;
    Zinc_error_list* errors;
    size_t group_number;
    size_t pos;
    Zinc_location loc;
} Cob_compile_data;

typedef struct Cob_re {
    Zinc_error_list* errors;
    Cob_ast* root;
    size_t group_count;
} Cob_re;

void Cob_compile_data_init(Cob_compile_data* cd, Zinc_string* text, Zinc_error_list* errors);
void Cob_compile_data_create(Cob_compile_data** cd, Zinc_string* text, Zinc_error_list* errors);
void Cob_compile_data_destroy(Cob_compile_data* cd);

void Cob_re_init(Cob_re* re, Zinc_error_list* el, Cob_ast* root);
void Cob_re_create(Cob_re** re, Zinc_error_list* el, Cob_ast* root);
void Cob_re_destroy(Cob_re* re);

#endif //COBBLE_COMPILE_DATA_H
