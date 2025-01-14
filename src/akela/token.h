#ifndef AKELA_TOKEN_H
#define AKELA_TOKEN_H

#include "api.h"
#include <stdbool.h>
#include "zinc/result.h"
#include "zinc/buffer.h"
#include "zinc/error.h"

typedef enum Ake_token_enum {
    Ake_token_none,
    Ake_token_number,
    Ake_token_id,
    Ake_token_equal,
    Ake_token_plus,
    Ake_token_minus,
    Ake_token_mult,
    Ake_token_divide,
    Ake_token_left_paren,
    Ake_token_right_paren,
    Ake_token_newline,
    Ake_token_fn,
    Ake_token_end,
    Ake_token_comma,
    Ake_token_semicolon,
    Ake_token_if,
    Ake_token_elseif,
    Ake_token_else,
    Ake_token_double_equal,
    Ake_token_not_equal,
    Ake_token_less_than,
    Ake_token_less_than_or_equal,
    Ake_token_greater_than,
    Ake_token_greater_than_or_equal,
    Ake_token_not,
    Ake_token_ampersand,
    Ake_token_and,
    Ake_token_vertical_bar,
    Ake_token_or,
    Ake_token_while,
    Ake_token_for,
    Ake_token_in,
    Ake_token_colon,
    Ake_token_left_square_bracket,
    Ake_token_right_square_bracket,
    Ake_token_string,
    Ake_token_let,
    Ake_token_left_curly_brace,
    Ake_token_right_curly_brace,
    Ake_token_boolean,
    Ake_token_caret,
    Ake_token_module,
    Ake_token_dot,
    Ake_token_struct,
    Ake_token_return,
    Ake_token_eof,
    Ake_token_extern,
    Ake_token_mut,
    Ake_token_const,
    Ake_token_arrow,
    Ake_token_ellipsis,
    Ake_token_range,
    Ake_token_impl,
    Ake_token_self,
    Ake_token_count     /* keep at the end */
} Ake_token_enum;

static enum result Ake_token_name_init(char** token_name)
{
    for (int i = 0; i < Ake_token_count; i++) {
        token_name[i] = NULL;
    }

    token_name[Ake_token_none] = "none";
    token_name[Ake_token_number] = "number";
    token_name[Ake_token_id] = "id";
    token_name[Ake_token_equal] = "equal";
    token_name[Ake_token_plus] = "plus";
    token_name[Ake_token_minus] = "minus";
    token_name[Ake_token_mult] = "mult";
    token_name[Ake_token_divide] = "divide";
    token_name[Ake_token_left_paren] = "left-parenthesis";
    token_name[Ake_token_right_paren] = "right-parenthesis";
    token_name[Ake_token_newline] = "newline";
    token_name[Ake_token_fn] = "fn";
    token_name[Ake_token_end] = "end";
    token_name[Ake_token_if] = "if";
    token_name[Ake_token_elseif] = "elseif";
    token_name[Ake_token_else] = "else";
    token_name[Ake_token_comma] = "comma";
    token_name[Ake_token_semicolon] = "semicolon";
    token_name[Ake_token_if] = "if";
    token_name[Ake_token_elseif] = "elseif";
    token_name[Ake_token_else] = "else";
    token_name[Ake_token_double_equal] = "double-equal";
    token_name[Ake_token_not_equal] = "not-equal";
    token_name[Ake_token_less_than] = "less-than";
    token_name[Ake_token_less_than_or_equal] = "less-than-or-equal";
    token_name[Ake_token_greater_than] = "greater-than";
    token_name[Ake_token_greater_than_or_equal] = "greater-than-or-equal";
    token_name[Ake_token_not] = "not";
    token_name[Ake_token_ampersand] = "ampersand";
    token_name[Ake_token_and] = "and";
    token_name[Ake_token_vertical_bar] = "vertical-bar";
    token_name[Ake_token_or] = "or";
    token_name[Ake_token_while] = "while";
    token_name[Ake_token_for] = "for";
    token_name[Ake_token_in] = "in";
    token_name[Ake_token_colon] = ":";
    token_name[Ake_token_left_square_bracket] = "[";
    token_name[Ake_token_right_square_bracket] = "]";
    token_name[Ake_token_string] = "string";
    token_name[Ake_token_let] = "var";
    token_name[Ake_token_left_curly_brace] = "{";
    token_name[Ake_token_right_curly_brace] = "}";
    token_name[Ake_token_boolean] = "boolean";
    token_name[Ake_token_caret] = "^";
    token_name[Ake_token_module] = "module";
    token_name[Ake_token_dot] = "dot";
    token_name[Ake_token_struct] = "struct";
    token_name[Ake_token_return] = "return";
    token_name[Ake_token_eof] = "EOF";
    token_name[Ake_token_extern] = "extern";
    token_name[Ake_token_mut] = "mut";
    token_name[Ake_token_const] = "const";
    token_name[Ake_token_arrow] = "arrow";
    token_name[Ake_token_ellipsis] = "ellipsis";
    token_name[Ake_token_range] = "range";
    token_name[Ake_token_impl] = "impl";
    token_name[Ake_token_self] = "self";

    for (int i = 0; i < Ake_token_count; i++) {
        if (token_name[i] == NULL) {
            return set_error("token name missing: %d\n", i);
        }
    }
    return result_ok;
}

typedef struct Ake_token {
    enum Ake_token_enum type;
    struct buffer value;
    bool is_integer;
    bool is_float;
    struct location loc;
    struct Ake_token* prev;
    struct Ake_token* next;
} Ake_token;

typedef struct Ake_token_list {
    struct Ake_token* head;
    struct Ake_token* tail;
} Ake_token_list;

AKELA_API void Ake_token_init(struct Ake_token* t);
AKELA_API void Ake_token_reset(struct Ake_token* t);
AKELA_API void Ake_token_list_init(struct Ake_token_list* tl);
AKELA_API void Ake_token_list_destroy(struct Ake_token_list* tl);
AKELA_API void Ake_token_destroy(struct Ake_token* t);
AKELA_API int Ake_token_find_first(struct Ake_token_list* tl, enum Ake_token_enum type);
AKELA_API int Ake_token_find_last(struct Ake_token_list* tl, enum Ake_token_enum type);
AKELA_API int Ake_token_list_count(struct Ake_token_list* tl);
AKELA_API void Ake_token_list_make(struct Ake_token_list** tl);
AKELA_API void Ake_token_list_add(struct Ake_token_list* tl, struct Ake_token* t);
AKELA_API struct Ake_token* Ake_token_list_pop(struct Ake_token_list* tl);
AKELA_API void Ake_token_list_reset(struct Ake_token_list* tl);
AKELA_API enum result Ake_token_list_print(struct Ake_token_list* tl);
AKELA_API enum result Ake_print_token(struct Ake_token* t);
AKELA_API void Ake_get_token_location(struct Ake_token* t, struct location* loc);

#endif
