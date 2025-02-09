#ifndef JSON_PARSE_TOOLS_H
#define JSON_PARSE_TOOLS_H

#include "zinc/error.h"
#include "lex_tools.h"
#include "token.h"
#include "dom.h"

typedef struct Json_parse_data {
    struct Zinc_error_list* el;
    Json_lex_data* ld;
    Json_token* lookahead;
} Json_parse_data;

typedef struct Json_result {
    Zinc_error_list* el;
    Json_dom* root;
} Json_result;

void Json_parse_data_init(Json_parse_data* pd, struct Zinc_error_list* el, Json_lex_data* ld);
void Json_parse_data_create(Json_parse_data** pd, struct Zinc_error_list* el, Json_lex_data* ld);

void Json_get_lookahead(Json_parse_data* pd);
bool Json_match(Json_parse_data* pd, Json_token_type type, Json_token** t, Json_dom* dom);

void Json_result_init(Json_result* res);
void Json_result_create(Json_result** res);
void Json_result_destroy(Json_result* res);

#endif
