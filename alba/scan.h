#ifndef _SCAN_H
#define _SCAN_H

#include "ustring.h"

struct char_value {
    UChar32 equal;
    UChar32 plus;
    UChar32 minus;
    UChar32 space;
    UChar32 mult;
    UChar32 divide;
    UChar32 left_paren;
    UChar32 right_paren;
};

enum state_enum {
    state_start,
    state_word,
    state_number
};

enum result scan(struct allocator *al, struct string* line, struct token_list* tl);

#endif
