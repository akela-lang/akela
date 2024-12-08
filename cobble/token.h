#ifndef COBBLE_TOKEN_H
#define COBBLE_TOKEN_H

#include "zinc/error.h"

enum token_type {
    token_none,
    token_eof,
    token_literal,
    token_union,
    token_closure,
    token_plus,
    token_open_paren,
    token_close_paren,
    token_open_repeat,
    token_close_repeat,
    token_comma,
    token_digit,
    token_question,
    token_wildcard,
    token_begin,
    token_end,
    token_backslash,
    token_left_square_bracket,
    token_right_square_bracket,
    token_dash,
};

struct token {
    enum token_type type;
    int num;
    char c[4];
    struct location loc;
};

void token_init(struct token* t);
enum token_type token_classify(char c[4], int num, bool done);

#endif //COBBLE_TOKEN_H
