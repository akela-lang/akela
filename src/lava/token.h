#ifndef LAVA_TOKEN_H
#define LAVA_TOKEN_H

#include <zinc/zstring.h>
#include <zinc/error.h>
#include <assert.h>

typedef enum Lava_token_kind Lava_token_kind;
enum Lava_token_kind {
    Lava_token_kind_none,
    Lava_token_kind_header,
    Lava_token_kind_backquote,
    Lava_token_kind_newline,
    Lava_token_kind_text,
    Lava_token_kind_eof,
    Lava_token_kind_count,
};

static char* Lava_token_kind_name(Lava_token_kind kind)
{
    char* name[Lava_token_kind_count];
    name[Lava_token_kind_none] = "none";
    name[Lava_token_kind_header] = "header";
    name[Lava_token_kind_backquote] = "backquote";
    name[Lava_token_kind_newline] = "newline";
    name[Lava_token_kind_text] = "text";
    name[Lava_token_kind_eof] = "eof";

    if (kind >= 0 && kind < Lava_token_kind_count) {
        return name[kind];
    }

    assert(false && "invalid token kind");
}

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