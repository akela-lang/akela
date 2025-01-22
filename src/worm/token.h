#ifndef WORM_TOKEN_H
#define WORM_TOKEN_H

#include <stdint.h>
#include <sys/types.h>

#include "zinc/zstring.h"

typedef enum {
    Worm_token_type_none,
    Worm_token_type_id,
    Worm_token_type_dot,
    Worm_token_type_left_curly_brace,
    Worm_token_type_right_curly_brace,
    Worm_token_type_equal,
    Worm_token_type_string,
    Worm_token_type_semicolon,
    Worm_token_type_number,
} Worm_token_type;

typedef enum {
    Worm_number_type_none,
    Worm_number_type_int,
    Worm_number_type_nat,
    Worm_number_type_real,
} Worm_number_type;

typedef struct {
    Worm_token_type type;
    Zinc_string value;
    Worm_number_type number_type;
    union {
        int64_t integer;
        u_int64_t natural;
        double real;
    } number;
} Worm_token;

#endif