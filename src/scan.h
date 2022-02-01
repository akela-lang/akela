#ifndef _SCAN_H
#define _SCAN_H

#include "result.h"
#include "ustring.h"

enum token_enum {
    token_none,
    token_number,
    token_word,
    token_equal,
    token_plus,
    token_minus,
    token_mult,
    token_divide,
    token_left_paren,
    token_right_paren,
    token_count     /* keep at end */
};

#ifdef _SCAN_C
enum result_enum token_name_init(char** token_name)
{
    for (int i = 0; i < token_count; i++) {
        token_name[i] = NULL;
    }

    token_name[token_none] = "none";
    token_name[token_number] = "number";
    token_name[token_word] = "word";
    token_name[token_equal] = "equal";
    token_name[token_plus] = "plus";
    token_name[token_minus] = "minus";
    token_name[token_mult] = "mult";
    token_name[token_divide] = "divide";
    token_name[token_left_paren] = "left parenthesis";
    token_name[token_right_paren] = "right parenthesis";

    for (int i = 0; i < token_count; i++) {
        if (token_name[i] == NULL) {
            return set_error("token name missing: %d\n", i);
        }
    }
    return ok_result;
}
#endif

struct token {
    enum token_enum type;
    struct string value;
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

enum state_enum {
    state_start,
    state_word,
    state_number
};

void token_list_init(struct token_list* tl);
void token_list_reset(struct token_list* tl);
enum result_enum scan(struct string* line, struct token_list* tl);
enum result_enum token_list_print(struct token_list* tl, char** token_name);

#endif
