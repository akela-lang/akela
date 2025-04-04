#include "lex_tools.h"
#include "zinc/utf8.h"
#include <ctype.h>
#include "zinc/memory.h"

/**
 * initialize lexer state
 * @param ls lexer data
 * @param lc lookahead buffer
 * @param el error list
 * @param st symbol table
 */
void Ake_lex_state_init(
        Ake_lex_state* ls,
        void* input_obj,
        Zinc_input_unicode_vtable* input_vtable,
        Zinc_error_list* el,
        Ake_symbol_table* st)
{
    ls->input = input_obj;
    ls->vtable = input_vtable;
    ls->el = el;
    ls->st = st;
}

void Ake_lex_state_create(
    Ake_lex_state** ls,
    void* input,
    Zinc_input_unicode_vtable* vtable,
    Zinc_error_list* el,
    Ake_symbol_table* st)
{
    Zinc_malloc_safe((void**)ls, sizeof(Ake_lex_state));
    Ake_lex_state_init(*ls, input, vtable, el, st);
}

/**
 * check if compound operator
 * @param uc utf32 character
 * @return true if a compound operator, otherwise false
 */
bool Ake_compound_operator_start(int num, const char c[4])
{
    if (num != 1) return false;
    return *c == '=' ||
        *c == '!' ||
        *c == '<' ||
        *c == '>' ||
        *c == '&' ||
        *c == '|' ||
        *c == '-' ||
        *c == '.';
}

bool Ake_utf8_match(const char a[4], int a_num, const char b[4], int b_num)
{
    if (a_num != b_num) return false;
    for (int i = 0; i < a_num; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

bool Ake_is_word(const char c[4], int num)
{
    if (num == 1 && isalpha(c[0])) {
        return true;
    }

    if (num >= 2) {
        char greek[] = "ΑαΒβΓγΔδΕεΖζΗηΘθΙιΚκΛλΜμΝνΞξΟοΠπΡρΣσςΤτΥυΦφΧχΨψΩω";
        int pos = 0;
        int num2;
        while (greek[pos]) {
            num2 = ZINC_NUM_BYTES(greek[pos]);
            if (num2 != num) {
                return false;
            }
            if (Ake_utf8_match(c, num, greek+pos, num2)) {
                return true;
            }
            pos += num2;
        }
    }

    return false;
}

bool Ake_is_num(const char c[4], int num)
{
    if (num != 1) return false;
    return isdigit(*c);
}