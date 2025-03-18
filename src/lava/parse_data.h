#ifndef LAVA_PARSE_DATA_H
#define LAVA_PARSE_DATA_H

#include "lex_data.h"
#include "zinc/error.h"
#include "token.h"
#include "dom.h"

typedef struct Lava_parse_data Lava_parse_data;
struct Lava_parse_data {
    Lava_lex_data *ld;
    Zinc_error_list* errors;
    Lava_token* lookahead;
};

typedef struct Lava_result Lava_result;
struct Lava_result {
    Zinc_error_list* errors;
    Lava_dom* root;
};

void Lava_parse_data_init(Lava_parse_data* pd, Lava_lex_data* ld, Zinc_error_list* errors);
void Lava_parse_data_create(Lava_parse_data** pd, Lava_lex_data* ld, Zinc_error_list* errors);
void Lava_result_init(Lava_result* lr, Zinc_error_list* errors, Lava_dom* root);
void Lava_result_create(Lava_result** lr, Zinc_error_list* errors, Lava_dom* root);
void Lava_result_destroy(Lava_result* lr);

#endif