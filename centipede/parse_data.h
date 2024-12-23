#ifndef CENTIPEDE_PARSE_DATA_H
#define CENTIPEDE_PARSE_DATA_H

#include "lex_data.h"
#include "zinc/error.h"
#include "token.h"
#include "value.h"

typedef struct Cent_parse_data {
    Cent_lex_data *ld;
    struct error_list* errors;
    Cent_token* lookahead;
} Cent_parse_data;

void Cent_parse_data_init(Cent_parse_data *pd);
void Cent_parse_data_create(Cent_parse_data **pd);

typedef struct Cent_parse_result {
    struct error_list* errors;
    Cent_value* value;
} Cent_parse_result;

#endif