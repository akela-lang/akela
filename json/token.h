#ifndef JSON_TOKEN_H
#define JSON_TOKEN_H

#include "zinc/buffer.h"

typedef enum Json_token_type {
    Json_token_none,
    Json_token_type_string,
    Json_token_type_number,
} Json_token_type;

typedef struct Json_token {
  Json_token_type type;
  struct buffer value;
} Json_token;

#endif