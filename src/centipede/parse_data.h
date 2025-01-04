#ifndef CENTIPEDE_PARSE_DATA_H
#define CENTIPEDE_PARSE_DATA_H

#include "lex_data.h"
#include "zinc/error.h"
#include "token.h"
#include "value.h"
#include "environment.h"
#include "ast.h"
#include "zinc/String_slice.h"
#include "comp_unit.h"
#include "module.h"

typedef struct Cent_parse_data {
    Cent_lex_data *ld;
    struct error_list* errors;
    Cent_token* lookahead;
    Cent_environment* top;
    String_slice file_name;
    Cent_comp_table* comp_table;
    void* module_obj;
    Cent_module_vtable* module_vtable;
} Cent_parse_data;

typedef struct Cent_parse_result {
    struct error_list* errors;
    Cent_environment* base;
    Cent_ast* root;
} Cent_parse_result;

void Cent_parse_data_init(Cent_parse_data *pd);
void Cent_parse_data_create(Cent_parse_data **pd);

void Cent_parse_result_init(Cent_parse_result *pr);
void Cent_parse_result_create(Cent_parse_result **pr);

#endif