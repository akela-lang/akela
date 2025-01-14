//
// Created by miguel on 6/6/23.
//

#include "token.h"
#include <stdio.h>
#include <ctype.h>

void Cob_token_init(Cob_token* t)
{
    t->type = Cob_token_none;
    t->num = 0;
    Zinc_location_init(&t->loc);
}

Cob_token_type Cob_token_classify(char c[4], int num, bool done)
{
    if (done) {
        return Cob_token_eof;
    }

    if (num == 1) {
        if (isdigit(c[0])) {
            return Cob_token_digit;
        }

        switch (c[0]) {
        case '|':
            return Cob_token_union;
        case '*':
            return Cob_token_closure;
        case '+':
            return Cob_token_plus;
        case '(':
            return Cob_token_open_paren;
        case ')':
            return Cob_token_close_paren;
        case '{':
            return Cob_token_open_repeat;
        case '}':
            return Cob_token_close_repeat;
        case ',':
            return Cob_token_comma;
        case '?':
            return Cob_token_question;
        case '.':
            return Cob_token_wildcard;
        case '^':
            return Cob_token_caret;
        case '$':
            return Cob_token_dollar;
        case '\\':
            return Cob_token_backslash;
        case '[':
            return Cob_token_left_square_bracket;
        case ']':
            return Cob_token_right_square_bracket;
        case '-':
            return Cob_token_dash;
        default:
            return Cob_token_literal;
        }
    }

    return Cob_token_literal;
}
