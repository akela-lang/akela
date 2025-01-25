#ifndef WORM_TOKEN_H
#define WORM_TOKEN_H

#include <stdint.h>
#include <sys/types.h>
#include <zinc/error.h>

#include "zinc/zstring.h"

typedef enum {
    Worm_token_type_none,
    Worm_token_type_id,
    Worm_token_type_string,
    Worm_token_type_integer,
    Worm_token_type_natural,
    Worm_token_type_real,
    Worm_token_type_dot,
    Worm_token_type_left_curly_brace,
    Worm_token_type_right_curly_brace,
    Worm_token_type_equal,
    Worm_token_type_semicolon,
    Worm_token_type_newline,
    Worm_token_type_modifier,
    Worm_token_type_true,
    Worm_token_type_false,
    Worm_token_type_eof,
    Worm_token_type_count,      /* leave at end */
} Worm_token_type;

static char const* Worm_token_name(Worm_token_type type)
{
    char const* name[Worm_token_type_count];
    name[Worm_token_type_none] = "none";
    name[Worm_token_type_id] = "id";
    name[Worm_token_type_string] = "string";
    name[Worm_token_type_integer] = "integer";
    name[Worm_token_type_natural] = "natural";
    name[Worm_token_type_real] = "real";
    name[Worm_token_type_dot] = "dot";
    name[Worm_token_type_left_curly_brace] = "left-curly-brace";
    name[Worm_token_type_right_curly_brace] = "right-curly-brace";
    name[Worm_token_type_equal] = "equal";
    name[Worm_token_type_semicolon] = "semicolon";
    name[Worm_token_type_newline] = "newline";
    name[Worm_token_type_modifier] = "modifier";
    name[Worm_token_type_true] = "true";
    name[Worm_token_type_false] = "false";
    name[Worm_token_type_eof] = "eof";

    return name[type];
}

typedef struct {
    Worm_token_type type;
    Zinc_string value;
    union {
        int64_t integer;
        u_int64_t natural;
        double real;
    } number;
    Zinc_location loc;
} Worm_token;

void Worm_token_init(Worm_token *t);
void Worm_token_create(Worm_token** t);
void Worm_token_destroy(Worm_token *t);

#endif
