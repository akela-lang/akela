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
    Cent_token_required,
} Cent_token_type;

typedef struct Cent_token {
    Cent_token_type type;
    struct buffer value;
    struct location loc;
} Cent_token;

void Cent_token_init(Cent_token *t);
void Cent_token_create(Cent_token **t);
void Cent_token_destroy(Cent_token *t);

#endif