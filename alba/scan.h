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
enum result token_name_init(char** token_name)
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
    return result_ok;
}
#endif

struct char_value {
    UChar32 plus;
    UChar32 minus;
    UChar32 space;
    UChar32 mult;
    UChar32 divide;
};

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

enum result token_name_init(char** token_name);
void token_list_init(struct token_list* tl);
enum result token_list_make(struct token_list** tl);
void token_list_reset(struct token_list* tl);
void token_list_destroy(struct token_list* tl);
enum result token_list_print(struct token_list* tl, char** token_name);
enum result scan(struct string* line, struct token_list* tl);
struct token* get_token(struct token_node* head, size_t pos);
int token_find_first(struct token_node* head, enum token_enum type);
int token_find_last(struct token_node* tail, enum token_enum type);
enum result token_list_slice(struct token_list* tl, int start, int end, struct token_list** slice);
int token_list_count(struct token_list* tl);

#endif
