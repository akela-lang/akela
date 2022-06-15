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
};

enum state_enum {
    state_start,
    state_word,
    state_number
};

enum result scan_get_token(struct allocator* al, struct input_state* is, int* got_token, struct token** t);
void set_char_values(struct char_value* cv);

#endif
