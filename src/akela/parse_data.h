#ifndef AKELA_PARSE_DATA_H
#define AKELA_PARSE_DATA_H

#include "ast.h"
#include "zinc/error.h"

typedef struct Ake_parse_result Ake_parse_result;
struct Ake_parse_result {
    Ake_Ast* root;
    Zinc_error_list* errors;
};

void Ake_parse_result_init(Ake_parse_result* pr, Ake_Ast* root, Zinc_error_list* errors);
void Ake_parse_result_destroy(Ake_parse_result* pr);

#endif