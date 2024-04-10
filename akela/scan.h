#ifndef _SCAN_H
#define _SCAN_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "token.h"
#include "lookahead_char.h"
#include "source.h"
#include "symbol_table.h"
#include "zinc/input_char.h"

struct scan_state {
    void* input_obj;
    InputCharVTable* input_vtable;
    struct location loc;
    struct symbol_table* st;
    struct error_list* el;
};

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
    UChar32 double_quote;
    UChar32 backslash;
};

enum state_enum {
    state_start,
    state_id,
    state_id_underscore,
    state_number_whole,
    state_number_fraction_start,
    state_number_fraction,
    state_number_exponent_start,
    state_number_exponent_sign_start,
    state_number_exponent,
    state_string,
    state_string_backslash,
    state_compound_operator
};

AKELA_API void scan_state_init(struct scan_state* sns, void* input_obj, InputCharVTable* input_vtable, struct error_list* el, struct symbol_table* st);
AKELA_API bool scan_get_token(struct scan_state* sns, struct token** t);

#endif
