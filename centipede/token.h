#ifndef CENTIPEDE_TOKEN_H
#define CENTIPEDE_TOKEN_H

#include "zinc/buffer.h"
#include "zinc/error.h"

typedef enum Cent_token_type {
    Cent_token_none,
    Cent_token_eof,
    Cent_token_id,
    Cent_token_element,
    Cent_token_end,
    Cent_token_newline,
    Cent_token_properties,
    Cent_token_children,
    Cent_token_colon,
    Cent_token_enum,
    Cent_token_equal,
    Cent_token_left_curly_brace,
    Cent_token_right_curly_brace,
    Cent_token_dot,
    Cent_token_double_colon,
    Cent_token_string,
    Cent_token_number,
    Cent_token_true,
    Cent_token_false,
    Cent_token_modifier,
} Cent_token_type;

typedef enum Cent_number_type {
    Cent_number_type_none,
    Cent_number_type_integer,
    Cent_number_type_fp,
    } Cent_number_type;

typedef struct Cent_token {
    Cent_token_type type;
    Cent_number_type number_type;
    struct buffer value;
    struct location loc;
    union {
        long long integer;
        double fp;
    } number_value;
} Cent_token;

void Cent_token_init(Cent_token *t);
void Cent_token_create(Cent_token **t);
void Cent_token_destroy(Cent_token *t);

#endif