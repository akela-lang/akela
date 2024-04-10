#include "lex_tools.h"
#include "zinc/utf8.h"
#include <ctype.h>

/**
 * initialize scanner state
 * @param sns scanner data
 * @param lc lookahead buffer
 * @param el error list
 * @param st symbol table
 */
void scan_state_init(struct scan_state* sns, void* input_obj, InputCharVTable* input_vtable, struct error_list* el, struct symbol_table* st)
{
    sns->input_obj = input_obj;
    sns->input_vtable = input_vtable;
    sns->el = el;
    sns->st = st;
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

bool get_uc_char(struct scan_state* sns, char c[4], int* num, struct location* loc)
{
    bool done;
    done = InputCharNext(sns->input_obj, sns->input_vtable, c, loc);
    if (done) {
        num = 0;
        return done;
    }
    *num = NUM_BYTES(*c);
    for (int i = 1; i < *num; i++) {
        struct location loc2;
        done = InputCharNext(sns->input_obj, sns->input_vtable, c + i, &loc2);
        if (done) {
            break;
        }
        loc->size++;
    }
    return done;
}

bool is_word(const char c[4])
{
    return isalpha(*c);
}

bool is_num(const char c[4])
{
    return isdigit(*c);
}