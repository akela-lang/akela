#ifndef _TOKEN_H
#define _TOKEN_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/result.h"
#include "zinc/buffer.h"
#include "zinc/error.h"

enum token_enum {
    token_none,
    token_number,
    token_id,
    token_equal,
    token_plus,
    token_minus,
    token_mult,
    token_divide,
    token_left_paren,
    token_right_paren,
    token_newline,
    token_fn,
    token_end,
    token_comma,
    token_semicolon,
    token_if,
    token_elseif,
    token_else,
    token_double_equal,
    token_not_equal,
    token_less_than,
    token_less_than_or_equal,
    token_greater_than,
    token_greater_than_or_equal,
    token_not,
    token_ampersand,
    token_and,
    token_vertical_bar,
    token_or,
    token_while,
    token_for,
    token_in,
    token_colon,
    token_left_square_bracket,
    token_right_square_bracket,
    token_string,
    token_let,
    token_left_curly_brace,
    token_right_curly_brace,
    token_boolean,
    token_caret,
    token_module,
    token_dot,
    token_struct,
    token_return,
    token_eof,
    token_extern,
    token_mut,
    token_const,
    token_arrow,
    token_ellipsis,
    token_range,
    token_impl,
    token_self,
    token_count     /* keep at the end */
};

# ifdef _TOKEN_C
/* dynamic-output-none */
enum result token_name_init(char** token_name)
{
    for (int i = 0; i < token_count; i++) {
        token_name[i] = NULL;
    }

    token_name[token_none] = "none";
    token_name[token_number] = "number";
    token_name[token_id] = "id";
    token_name[token_equal] = "equal";
    token_name[token_plus] = "plus";
    token_name[token_minus] = "minus";
    token_name[token_mult] = "mult";
    token_name[token_divide] = "divide";
    token_name[token_left_paren] = "left-parenthesis";
    token_name[token_right_paren] = "right-parenthesis";
    token_name[token_newline] = "newline";
    token_name[token_fn] = "fn";
    token_name[token_end] = "end";
    token_name[token_if] = "if";
    token_name[token_elseif] = "elseif";
    token_name[token_else] = "else";
    token_name[token_comma] = "comma";
    token_name[token_semicolon] = "semicolon";
    token_name[token_if] = "if";
    token_name[token_elseif] = "elseif";
    token_name[token_else] = "else";
    token_name[token_double_equal] = "double-equal";
    token_name[token_not_equal] = "not-equal";
    token_name[token_less_than] = "less-than";
    token_name[token_less_than_or_equal] = "less-than-or-equal";
    token_name[token_greater_than] = "greater-than";
    token_name[token_greater_than_or_equal] = "greater-than-or-equal";
    token_name[token_not] = "not";
    token_name[token_ampersand] = "ampersand";
    token_name[token_and] = "and";
    token_name[token_vertical_bar] = "vertical-bar";
    token_name[token_or] = "or";
    token_name[token_while] = "while";
    token_name[token_for] = "for";
    token_name[token_in] = "in";
    token_name[token_colon] = ":";
    token_name[token_left_square_bracket] = "[";
    token_name[token_right_square_bracket] = "]";
    token_name[token_string] = "string";
    token_name[token_let] = "var";
    token_name[token_left_curly_brace] = "{";
    token_name[token_right_curly_brace] = "}";
    token_name[token_boolean] = "boolean";
    token_name[token_caret] = "^";
    token_name[token_module] = "module";
    token_name[token_dot] = "dot";
    token_name[token_struct] = "struct";
    token_name[token_return] = "return";
    token_name[token_eof] = "EOF";
    token_name[token_extern] = "extern";
    token_name[token_mut] = "mut";
    token_name[token_const] = "const";
    token_name[token_arrow] = "arrow";
    token_name[token_ellipsis] = "ellipsis";
    token_name[token_range] = "range";
    token_name[token_impl] = "impl";
    token_name[token_self] = "self";

    for (int i = 0; i < token_count; i++) {
        if (token_name[i] == NULL) {
            return set_error("token name missing: %d\n", i);
        }
    }
    return result_ok;
}
# else

/* dynamic-output-none */
AKELA_API enum result token_name_init(char** token_name);

# endif

struct token {
    enum token_enum type;
    struct buffer value;
    bool is_integer;
    bool is_float;
    struct location loc;
    struct token* prev;
    struct token* next;
};

struct token_list {
    struct token* head;
    struct token* tail;
};

AKELA_API void token_init(struct token* t);
AKELA_API void token_reset(struct token* t);
AKELA_API void token_list_init(struct token_list* tl);
AKELA_API void token_list_destroy(struct token_list* tl);
AKELA_API void token_destroy(struct token* t);
AKELA_API int token_find_first(struct token_list* tl, enum token_enum type);
AKELA_API int token_find_last(struct token_list* tl, enum token_enum type);
AKELA_API int token_list_count(struct token_list* tl);
AKELA_API void token_list_make(struct token_list** tl);
AKELA_API void token_list_add(struct token_list* tl, struct token* t);
AKELA_API struct token* token_list_pop(struct token_list* tl);
AKELA_API void token_list_reset(struct token_list* tl);
AKELA_API enum result token_list_print(struct token_list* tl);
AKELA_API enum result print_token(struct token* t);
AKELA_API void get_token_location(struct token* t, struct location* loc);

#endif
