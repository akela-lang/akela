#ifndef _SCAN_H
#define _SCAN_H

#include "alba_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "token.h"
#include "lookahead_char.h"
#include "source.h"
#include "symbol_table.h"

struct scan_state {
    struct lookahead_char* lc;
    struct symbol_table* st;
    struct compile_error_list* el;
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
    state_number_fraction,
    state_number_exponent_start,
    state_number_exponent_sign_start,
    state_number_exponent,
    state_string,
    state_string_backslash,
    state_compound_operator
};

/* dynamic-output-none */
/* initialize-output sns{} */
ALBA_API void scan_state_init(struct scan_state* sns, struct lookahead_char* lc, struct compile_error_list* el, struct symbol_table* st);

/* dynamic-output sns{wt{} el{}} t t{} */
/* dynamic-temp: tf tf{} */
ALBA_API bool scan_get_token(struct scan_state* sns, int* got_token, struct token** t);

/* dynamic-output-none */
ALBA_API void get_scan_location(struct scan_state* sns, struct location* loc);

#endif
