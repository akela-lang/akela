#ifndef LEX_TOOLS_H
#define LEX_TOOLS_H

#include "zinc/input_unicode.h"
#include "zinc/error.h"
#include "symbol_table.h"
#include "zinc/error.h"

struct lex_state {
    void* input_obj;
    InputUnicodeVTable* input_vtable;
    struct location loc;
    struct symbol_table* st;
    struct error_list* el;
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

void lex_state_init(
        struct lex_state* ls,
        void* input_obj,
        InputUnicodeVTable* input_vtable,
        struct error_list* el,
        struct symbol_table* st);
bool compound_operator_start(int num, const char c[4]);
bool is_word(const char c[4], int num);
bool is_num(const char c[4], int num);

#endif