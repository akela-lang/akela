#ifndef AKELA_TOKEN_H
#define AKELA_TOKEN_H

#include "api.h"
#include <stdbool.h>
#include "zinc/result.h"
#include "zinc/zstring.h"
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
    Ake_token_left_curly_brace,
    Ake_token_right_curly_brace,
    Ake_token_boolean,
    Ake_token_caret,
    Ake_token_dot,
    Ake_token_struct,
    Ake_token_return,
    Ake_token_eof,
    Ake_token_extern,
    Ake_token_const,
    Ake_token_var,
    Ake_token_arrow,
    Ake_token_ellipsis,
    Ake_token_range,
    Ake_token_count     /* keep at the end */
} Ake_token_enum;

static char const* Ake_token_name(Ake_token_enum type)
{
    char const* name[Ake_token_count];

    name[Ake_token_none] = "none";
    name[Ake_token_number] = "number";
    name[Ake_token_id] = "id";
    name[Ake_token_equal] = "equal";
    name[Ake_token_plus] = "plus";
    name[Ake_token_minus] = "minus";
    name[Ake_token_mult] = "mult";
    name[Ake_token_divide] = "divide";
    name[Ake_token_left_paren] = "left-parenthesis";
    name[Ake_token_right_paren] = "right-parenthesis";
    name[Ake_token_newline] = "newline";
    name[Ake_token_fn] = "fn";
    name[Ake_token_end] = "end";
    name[Ake_token_if] = "if";
    name[Ake_token_elseif] = "elseif";
    name[Ake_token_else] = "else";
    name[Ake_token_comma] = "comma";
    name[Ake_token_semicolon] = "semicolon";
    name[Ake_token_if] = "if";
    name[Ake_token_elseif] = "elseif";
    name[Ake_token_else] = "else";
    name[Ake_token_double_equal] = "double-equal";
    name[Ake_token_not_equal] = "not-equal";
    name[Ake_token_less_than] = "less-than";
    name[Ake_token_less_than_or_equal] = "less-than-or-equal";
    name[Ake_token_greater_than] = "greater-than";
    name[Ake_token_greater_than_or_equal] = "greater-than-or-equal";
    name[Ake_token_not] = "not";
    name[Ake_token_ampersand] = "ampersand";
    name[Ake_token_and] = "and";
    name[Ake_token_vertical_bar] = "vertical-bar";
    name[Ake_token_or] = "or";
    name[Ake_token_while] = "while";
    name[Ake_token_for] = "for";
    name[Ake_token_in] = "in";
    name[Ake_token_colon] = ":";
    name[Ake_token_left_square_bracket] = "[";
    name[Ake_token_right_square_bracket] = "]";
    name[Ake_token_string] = "string";
    name[Ake_token_left_curly_brace] = "{";
    name[Ake_token_right_curly_brace] = "}";
    name[Ake_token_boolean] = "boolean";
    name[Ake_token_caret] = "^";
    name[Ake_token_dot] = "dot";
    name[Ake_token_struct] = "struct";
    name[Ake_token_return] = "return";
    name[Ake_token_eof] = "EOF";
    name[Ake_token_extern] = "extern";
    name[Ake_token_const] = "const";
    name[Ake_token_var] = "var";
    name[Ake_token_arrow] = "arrow";
    name[Ake_token_ellipsis] = "ellipsis";
    name[Ake_token_range] = "range";

    if (type >= 0 && type <= Ake_token_count) {
        return name[type];
    }

    return "invalid Ake_token_enum";
}

typedef struct Ake_token {
    enum Ake_token_enum type;
    struct Zinc_string value;
    bool is_integer;
    bool is_float;
    struct Zinc_location loc;
    struct Ake_token* prev;
    struct Ake_token* next;
} Ake_token;

typedef struct Ake_token_list {
    struct Ake_token* head;
    struct Ake_token* tail;
} Ake_token_list;

AKELA_API void Ake_token_init(struct Ake_token* t);
AKELA_API void Ake_token_create(Ake_token** t);
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
AKELA_API void Ake_token_list_print(struct Ake_token_list* tl);
AKELA_API void Ake_print_token(struct Ake_token* t);
AKELA_API void Ake_get_token_location(struct Ake_token* t, struct Zinc_location* loc);

#endif
