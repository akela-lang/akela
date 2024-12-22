#ifndef CENTIPEDE_TOKEN_H
#define CENTIPEDE_TOKEN_H

#include "zinc/buffer.h"
#include "zinc/error.h"

typedef enum Cent_token_type {
    Cent_token_none,
    Cent_token_null,
    Cent_token_true,
    Cent_token_false,
    Cent_token_number,
    Cent_token_string,
    Cent_token_left_curly_brace,
    Cent_token_right_curly_brace,
    Cent_token_left_square_bracket,
    Cent_token_right_square_bracket,
    Cent_token_colon,
    Cent_token_comma,
    Cent_token_use,
    Cent_token_solo,
    Cent_token_mute,
    Cent_token_dec,
    Cent_token_cent,
} Cent_token_type;

typedef struct Cent_token {
    Cent_token_type type;
    struct buffer value;
    struct location loc;
} Cent_token;

#endif