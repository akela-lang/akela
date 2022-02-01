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
    token_count
};

#define TOKEN_NAME_SIZE 20
extern char* token_name[token_count];

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
enum result_enum token_list_print(struct token_list* tl);
void token_name_init();

#endif
