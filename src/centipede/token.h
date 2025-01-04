#ifndef CENTIPEDE_TOKEN_H
#define CENTIPEDE_TOKEN_H

#include "zinc/buffer.h"
#include "zinc/error.h"

typedef enum Cent_token_type {
    Cent_token_none,
    Cent_token_eof,
    Cent_token_id,
    Cent_token_element,
    Cent_token_end,
    Cent_token_newline,
    Cent_token_properties,
    Cent_token_children,
    Cent_token_colon,
    Cent_token_enum,
    Cent_token_equal,
    Cent_token_left_curly_brace,
    Cent_token_right_curly_brace,
    Cent_token_dot,
    Cent_token_double_colon,
    Cent_token_string,
    Cent_token_number,
    Cent_token_true,
    Cent_token_false,
    Cent_token_modifier,
    Cent_token_semicolon,
    Cent_token_left_paren,
    Cent_token_right_paren,
    Cent_token_comma,
    Cent_token_use,
    Cent_token_count,       /* keep at end */
} Cent_token_type;

static char* Cent_token_name(Cent_token_type type)
{
    char *name[] = {
        "none",
        "eof",
        "id",
        "element",
        "end",
        "newline",
        "properties",
        "children",
        "colon",
        "enum",
        "equal",
        "left-curly-brace",
        "right-curly-brace",
        "dot",
        "double-colon",
        "string",
        "number",
        "true",
        "false",
        "modifier",
        "semicolon",
        "left-parenthesis",
        "right-parenthesis",
        "comma",
        "use",
    };

    if (type < Cent_token_count) {
        return name[type];
    }

    fprintf(stderr, "invalid token: %d\n", type);
    exit(1);
}

typedef enum Cent_number_type {
    Cent_number_type_none,
    Cent_number_type_integer,
    Cent_number_type_fp,
} Cent_number_type;

typedef enum Cent_builtin_type {
    Cent_builtin_type_none,
    Cent_builtin_type_child_of,
    Cent_builtin_type_property_of,
    Cent_builtin_type_top,
    Cent_builtin_type_file_name,
} Cent_builtin_type;

typedef struct Cent_token {
    Cent_token_type type;
    Cent_number_type number_type;
    Cent_builtin_type builtin_type;
    struct buffer value;
    struct location loc;
    union {
        long long integer;
        double fp;
    } number_value;
} Cent_token;

void Cent_token_init(Cent_token *t);
void Cent_token_create(Cent_token **t);
void Cent_token_destroy(Cent_token *t);
void Cent_print_token(Cent_token *t);

#endif