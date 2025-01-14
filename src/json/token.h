#ifndef JSON_TOKEN_H
#define JSON_TOKEN_H

#include "zinc/zstring.h"
#include "zinc/error.h"

typedef enum Json_token_type {
    Json_token_type_none,
    Json_token_type_string,
    Json_token_type_number,
    Json_token_type_eof,
    Json_token_type_left_square_bracket,
    Json_token_type_right_square_bracket,
    Json_token_type_comma,
    Json_token_type_left_curly_brace,
    Json_token_type_right_curly_brace,
    Json_token_type_colon,
    Json_token_type_true,
    Json_token_type_false,
    Json_token_type_null,
    Json_token_type_count,      /* keep at end */
} Json_token_type;


static char* Json_token_type_name(Json_token_type type)
{
    char* name_array[Json_token_type_count] = {
        "none",
        "string",
        "number",
        "eof",
        "left square bracket",
        "right square bracket",
        "comma",
        "left curly brace",
        "right curly brace",
        "colon",
        "true",
        "false",
        "null",
    };

    return name_array[type];
}

typedef enum Json_number_type {
    Json_number_type_none,
    Json_number_type_integer,
    Json_number_type_fp,
} Json_number_type;

typedef struct Json_token {
    Json_token_type type;
    struct Zinc_string value;
    struct Zinc_location loc;
    Json_number_type number_type;
    union {
        long long integer;
        double fp;
    } number;
} Json_token;

void Json_token_init(Json_token* t);
void Json_token_create(Json_token** t);
void Json_token_destroy(Json_token* t);

#endif