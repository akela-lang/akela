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

struct string {
    size_t size;
    size_t buf_size;
    char* buf;
};

enum result_enum next_line(char** buf, int is_utf8, int* last_line);

#endif
