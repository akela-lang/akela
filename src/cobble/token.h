#ifndef COBBLE_TOKEN_H
#define COBBLE_TOKEN_H

#include "zinc/error.h"

typedef enum Cob_token_type {
    Cob_token_none,
    Cob_token_eof,
    Cob_token_literal,
    Cob_token_union,
    Cob_token_closure,
    Cob_token_plus,
    Cob_token_open_paren,
    Cob_token_close_paren,
    Cob_token_open_repeat,
    Cob_token_close_repeat,
    Cob_token_comma,
    Cob_token_digit,
    Cob_token_question,
    Cob_token_wildcard,
    Cob_token_caret,
    Cob_token_dollar,
    Cob_token_backslash,
    Cob_token_left_square_bracket,
    Cob_token_right_square_bracket,
    Cob_token_dash,
} Cob_token_type;

typedef struct Cob_token {
    Cob_token_type type;
    int num;
    char c[5];
    struct Zinc_location loc;
} Cob_token;

void Cob_token_init(Cob_token* t);
Cob_token_type Cob_token_classify(char c[4], int num, bool done);

#endif //COBBLE_TOKEN_H
