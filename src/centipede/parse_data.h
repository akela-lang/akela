#ifndef CENTIPEDE_PARSE_DATA_H
#define CENTIPEDE_PARSE_DATA_H

#include "lex_data.h"
#include "zinc/error.h"
#include "token.h"
#include "value.h"
#include "environment.h"
#include "ast.h"
#include "zinc/string_slice.h"
#include "module_finder.h"
#include "zinc/priority_queue.h"

typedef struct Cent_parse_data {
    Cent_lex_data *ld;
    Zinc_error_list* errors;
    Cent_token* lookahead;
    Cent_environment* top;
    Cent_environment* base;
    void* cu;
    void* ct;
    Zinc_priority_queue pq;
} Cent_parse_data;

typedef struct Cent_parse_result {
    Zinc_error_list* errors;
    Cent_environment* base;
    Cent_ast* root;
} Cent_parse_result;

typedef struct Cent_namespace_result {
    Cent_symbol* sym;
    Cent_ast* node;
} Cent_namespace_result;

void Cent_parse_data_init(
    Cent_parse_data *pd,
    Zinc_error_list* errors,
    Cent_lex_data *ld,
    Cent_environment* base);
void Cent_parse_data_create(
    Cent_parse_data **pd,
    Zinc_error_list* errors,
    Cent_lex_data *ld,
    Cent_environment* base);
void Cent_parse_data_destroy(Cent_parse_data* pd);

void Cent_parse_result_init(Cent_parse_result *pr, Zinc_error_list* errors);
void Cent_parse_result_create(Cent_parse_result **pr, Zinc_error_list* errors);

#endif