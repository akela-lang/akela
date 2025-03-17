#ifndef LAVA_TOKEN_H
#define LAVA_TOKEN_H

#include <zinc/zstring.h>
#include <zinc/error.h>

typedef enum Lava_token_kind Lava_token_kind;
enum Lava_token_kind {
    Lava_token_kind_none,
    Lava_token_kind_header,
    Lava_token_kind_backquote,
    Lava_token_kind_newline,
    Lava_token_kind_text,
    Lava_token_kind_eof,
};

typedef struct Lava_token Lava_token;
struct Lava_token
{
    Lava_token_kind kind;
    Zinc_string text;
    Zinc_location loc;
};

void Lava_token_init(Lava_token* t);
void Lava_token_create(Lava_token** t);
void Lava_token_destroy(Lava_token* t);

#endif