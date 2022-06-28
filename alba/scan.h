#ifndef _SCAN_H
#define _SCAN_H

#include "buffer.h"

struct char_value {
    UChar32 equal;
    UChar32 plus;
    UChar32 minus;
    UChar32 space;
    UChar32 newline;
    UChar32 mult;
    UChar32 divide;
    UChar32 left_paren;
    UChar32 right_paren;
    UChar32 comma;
    UChar32 less_than;
    UChar32 greater_than;
    UChar32 underscore;
    UChar32 semicolon;
    UChar32 exclamation;
    UChar32 ampersand;
    UChar32 vertical_bar;
    UChar32 colon;
    UChar32 left_square_bracket;
    UChar32 right_square_bracket;
};

enum state_enum {
    state_start,
    state_id,
    state_id_underscore,
    state_number,
    state_compound_operator
};

enum result scan_get_token(struct allocator* al, struct input_state* is, int* got_token, struct token** t);
void set_char_values(struct char_value* cv);

#endif
