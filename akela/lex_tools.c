#include "lex_tools.h"
#include "zinc/utf8.h"
#include <ctype.h>

/**
 * initialize scanner state
 * @param ls scanner data
 * @param lc lookahead buffer
 * @param el error list
 * @param st symbol table
 */
void lex_state_init(
        struct lex_state* ls,
        void* input_obj,
        InputUnicodeVTable* input_vtable,
        struct error_list* el,
        struct symbol_table* st)
{
    ls->input_obj = input_obj;
    ls->input_vtable = input_vtable;
    ls->el = el;
    ls->st = st;
}

/**
 * check if compound operator
 * @param uc utf32 character
 * @return true if a compound operator, otherwise false
 */
bool compound_operator_start(int num, const char c[4])
{
    return *c == '=' || *c == '!' || *c == '<' || *c == '>' || *c == '&' || *c == '|' || *c == ':';
}

bool is_word(const char c[4])
{
    return isalpha(*c);
}

bool is_num(const char c[4])
{
    return isdigit(*c);
}