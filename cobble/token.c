//
// Created by miguel on 6/6/23.
//

#include "token.h"
#include <stdio.h>
#include <ctype.h>

void token_init(struct token* t)
{
    t->type = token_none;
    t->num = 0;
    location_init(&t->loc);
}

enum token_type token_classify(char c[4], int num, bool done)
{
    if (done) {
        return token_eof;
    }

    if (num == 1) {
        if (isdigit(c[0])) {
            return token_digit;
        }

        switch (c[0]) {
        case '|':
            return token_union;
        case '*':
            return token_closure;
        case '+':
            return token_plus;
        case '(':
            return token_open_paren;
        case ')':
            return token_close_paren;
        case '{':
            return token_open_repeat;
        case '}':
            return token_close_repeat;
        case ',':
            return token_comma;
        case '?':
            return token_question;
        case '.':
            return token_wildcard;
        case '^':
            return token_begin;
        case '$':
            return token_end;
        case '\\':
            return token_backslash;
        case '[':
            return token_left_square_bracket;
        case ']':
            return token_right_square_bracket;
        case '-':
            return token_dash;
        default:
            return token_literal;
        }
    }

    return token_literal;
}
