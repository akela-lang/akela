#ifndef _TOKEN_H
#define _TOKEN_H

#include "zinc/result.h"
#include "zinc/buffer.h"

#define WORD_TABLE_SIZE 10

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
    token_function,
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
    token_double_colon,
    token_left_square_bracket,
    token_right_square_bracket,
    token_string,
    token_count     /* keep at the end */
};

# ifdef _TOKEN_C
/* static-output */
enum result token_name_init(char** token_name)
{
    for (int i = 0; i < token_count; i++) {
        token_name[i] = NULL;
    }

    token_name[token_none] = "none";
    token_name[token_number] = "number";
    token_name[token_id] = "word";
    token_name[token_equal] = "equal";
    token_name[token_plus] = "plus";
    token_name[token_minus] = "minus";
    token_name[token_mult] = "mult";
    token_name[token_divide] = "divide";
    token_name[token_left_paren] = "left-parenthesis";
    token_name[token_right_paren] = "right-parenthesis";
    token_name[token_newline] = "newline";
    token_name[token_function] = "function";
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
    token_name[token_double_colon] = "::";
    token_name[token_left_square_bracket] = "[";
    token_name[token_right_square_bracket] = "]";
    token_name[token_string] = "string";

    for (int i = 0; i < token_count; i++) {
        if (token_name[i] == NULL) {
            return set_error("token name missing: %d\n", i);
        }
    }
    return result_ok;
}
# else

/* static-output */
enum result token_name_init(char** token_name);

# endif

struct token {
    enum token_enum type;
    struct buffer value;
    size_t line;
    size_t col;
    size_t byte_pos;
    struct token* prev;
    struct token* next;
};

struct token_list {
    struct token* head;
    struct token* tail;
};

struct word {
    enum token_enum type;
    struct buffer value;
    struct word* next;
    struct word* prev;
};

struct word_list {
    struct word* head;
    struct word* tail;
};

struct word_table {
    unsigned int size;
    struct word_list* buckets;
};

/* static-output */
/* initialize-output t{} t{value{}} */
void token_init(struct token* t);

/* dynamic-destroy t{value{}} */
void token_reset(struct token* t);

/* static-output */
void token_list_init(struct token_list* tl);

/* dynamic-destroy t{value{}} */
void token_destroy(struct token* t);

/* static-output */
struct token* get_token(struct token_list* tl, size_t pos);

/* static-output */
int token_find_first(struct token_list* tl, enum token_enum type);

/* static-output */
int token_find_last(struct token_list* tl, enum token_enum type);

/* static-output */
int token_list_count(struct token_list* tl);

/* dynamic-output tl */
void token_list_make(struct token_list** tl);

/* static-output */
/* dynamic-transfer t t{} -> tl{} */
void token_list_add(struct token_list* tl, struct token* t);

/* partition-output tl{} -> return */
struct token* token_list_pop(struct token_list* tl);

/* initialize-output tl{} */
void token_list_reset(struct token_list* tl);

/* static-output */
enum result token_list_print(struct token_list* tl);

/* static-output */
enum result print_token(struct allocator* al, struct token* t);

/* static-output */
unsigned int hash_buffer(struct buffer* bf, unsigned int size);

/* static-output */
/* initialize-output w{} w{value{}} */
void word_init(struct word* w);

/* static-output */
/* initialize-output wl{} */
void word_list_init(struct word_list* wl);

/* dynamic-output wt{} */
void word_table_init(struct word_table* wt, unsigned int size);

/* dynamic-output wt{} */
void word_table_init_reserved(struct word_table* wt);

/* dynamic-output wt{} */
void word_table_add(struct word_table* wt, struct buffer* bf, enum token_enum type);

/* static-output */
struct word* word_table_get(struct word_table* wt, struct buffer* bf);

#endif
