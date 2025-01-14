#ifndef AKELA_LEX_TOOLS_H
#define AKELA_LEX_TOOLS_H

#include "zinc/input_unicode.h"
#include "zinc/error.h"
#include "symbol_table.h"
#include "zinc/error.h"

typedef struct Ake_lex_state {
    void* input_obj;
    InputUnicodeVTable* input_vtable;
    struct location loc;
    struct Ake_symbol_table* st;
    struct error_list* el;
} Ake_lex_state;

enum state_enum {
    Ake_state_start,
    Ake_state_id,
    Ake_state_id_underscore,
    Ake_state_number_whole,
    Ake_state_number_fraction_start,
    Ake_state_number_fraction,
    Ake_state_number_exponent_start,
    Ake_state_number_exponent_sign_start,
    Ake_state_number_exponent,
    Ake_state_string,
    Ake_state_string_backslash,
    Ake_state_compound_operator
};

void Ake_lex_state_init(
        struct Ake_lex_state* ls,
        void* input_obj,
        InputUnicodeVTable* input_vtable,
        struct error_list* el,
        struct Ake_symbol_table* st);
bool Ake_compound_operator_start(int num, const char c[4]);
bool Ake_is_word(const char c[4], int num);
bool Ake_is_num(const char c[4], int num);

#endif