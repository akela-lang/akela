#ifndef CENTIPEDE_PARSE_DATA_H
#define CENTIPEDE_PARSE_DATA_H

#include "lex_data.h"
#include "zinc/error.h"
#include "token.h"
#include "value.h"
#include "environment.h"

typedef struct Cent_parse_data {
    Cent_lex_data *ld;
    struct error_list* errors;
    Cent_token* lookahead;
} Cent_parse_data;

typedef struct Cent_parse_result {
    struct error_list* errors;
    Cent_environment* base_env;
    Cent_value* value;
} Cent_parse_result;

void Cent_parse_data_init(Cent_parse_data *pd);
void Cent_parse_data_create(Cent_parse_data **pd);

void Cent_parse_result_init(Cent_parse_result *pr);
void Cent_parse_result_create(Cent_parse_result **pr);

#endif