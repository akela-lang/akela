#ifndef _SCAN_H
#define _SCAN_H

#include "result.h"

enum token_enum {
    number_token,
    word_token,
    equal_token,
    plus_token,
    minus_token,
    mult_token,
    divide_token,
    left_paren_token,
    right_paren_token
};

struct token {
    enum token_enum type;
    char* value;
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
    start_state,
    word_state,
    number_state
};

enum result_enum scan(struct string* line);

#endif
