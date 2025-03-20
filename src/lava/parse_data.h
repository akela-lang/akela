#ifndef LAVA_PARSE_DATA_H
#define LAVA_PARSE_DATA_H

#include "lex_data.h"
#include "zinc/error.h"
#include "token.h"
#include "dom.h"

typedef struct Lava_stack Lava_stack;
struct Lava_stack {
    Lava_dom* dom;
    Lava_stack* prev;
};

typedef struct Lava_parse_data Lava_parse_data;
struct Lava_parse_data {
    Lava_lex_data *ld;
    Zinc_error_list* errors;
    Lava_token* lookahead;
    Lava_stack* stack;
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

void Lava_stack_push(Lava_stack** stack, Lava_dom* dom);
void Lava_stack_pop(Lava_stack** stack);
void Lava_stack_destroy(Lava_stack* stack);

#endif