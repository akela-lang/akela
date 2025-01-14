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

void Json_parse_data_init(Json_parse_data* pd, struct Zinc_error_list* el, Json_lex_data* ld);
void Json_parse_data_create(Json_parse_data** pd, struct Zinc_error_list* el, Json_lex_data* ld);
void Json_get_lookahead(Json_parse_data* pd);
bool Json_match(Json_parse_data* pd, Json_token_type type, Json_token** t, Json_dom* dom);

#endif