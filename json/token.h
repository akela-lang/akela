#ifndef JSON_TOKEN_H
#define JSON_TOKEN_H

#include "zinc/buffer.h"

typedef enum Json_token_type {
    Json_token_type_none,
    Json_token_type_string,
    Json_token_type_number,
    Json_token_type_eof,
} Json_token_type;

typedef struct Json_token {
    Json_token_type type;
    struct buffer value;
} Json_token;

void Json_token_init(Json_token* t);
void Json_token_create(Json_token** t);
void Json_token_destroy(Json_token* t);

#endif