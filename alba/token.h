#ifndef _TOKEN_H
#define _TOKEN_H

#include "result.h"
#include "buffer.h"

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
    token_if,
    token_elseif,
    token_else,
    token_double_equal,
    token_less_than,
    token_less_than_or_equal,
    token_greater_than,
    token_greater_than_or_equal,
    token_count     /* keep at the end */
};

#ifdef _TOKEN_C
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
    token_name[token_if] = "if";
    token_name[token_elseif] = "elseif";
    token_name[token_else] = "else";
    token_name[token_double_equal] = "double-equal";
    token_name[token_less_than] = "less-than";
    token_name[token_less_than_or_equal] = "less-than-or-equal";
    token_name[token_greater_than] = "greater-than";
    token_name[token_greater_than_or_equal] = "greater-than-or-equal";

    for (int i = 0; i < token_count; i++) {
        if (token_name[i] == NULL) {
            return set_error("token name missing: %d\n", i);
        }
    }
    return result_ok;
}
#endif

struct token {
    enum token_enum type;
    struct buffer value;
};

struct token_node {
    struct token* t;
    struct token_node* prev;
    struct token_node* next;
};

struct token_list {
    struct token_node* head;
    struct token_node* tail;
};

void token_init(struct token* t);
enum result token_name_init(char** token_name);
void token_reset(struct token* t);
void token_list_init(struct token_list* tl);
struct token* get_token(struct token_list* tl, size_t pos);
int token_find_first(struct token_list* tl, enum token_enum type);
int token_find_last(struct token_list* tl, enum token_enum type);
int token_list_count(struct token_list* tl);
enum result token_list_make(struct allocator* al, struct token_list** tl);
enum result token_list_add(struct allocator* al, struct token_list* tl, struct token* t);
enum result token_list_slice(struct allocator *al, struct token_list* tl, int start, int end, struct token_list** slice);
void token_list_reset(struct token_list* tl);
enum result token_list_print(struct allocator* al, struct token_list* tl, char** token_name);
struct token* token_list_pop(struct token_list* tl);

#endif
