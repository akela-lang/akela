#ifndef _SCAN_H
#define _SCAN_H

#include "result.h"

enum token_enum {
    number_token,
    variable_token,
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

#define STRING_CHUNK 64

struct string {
    size_t size;
    size_t buf_size;
    char* buf;
};

enum result_enum next_line(FILE* f, struct string* s, int is_utf8, int* last_line);
void string_init(struct string* s);
enum result_enum string_add_char(struct string* s, char c);
void string_reset(struct string* s);
void string_clear(struct string* s);
enum result_enum string2array(struct string* s, char** array);
enum result_enum scan(struct string* line);

#endif
