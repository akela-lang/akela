#include "token.h"
#include <assert.h>
#include <zinc/memory.h>

#include "memory.h"

void Lava_token_init(Lava_token* t)
{
    t->kind = Lava_token_kind_none;
}

void Lava_token_create(Lava_token** t)
{
    Zinc_malloc_safe((void**)t, sizeof(Lava_token));
    Lava_token_init(*t);
}

void Lava_token_set_kind(Lava_token* t, Lava_token_kind kind)
{
    t->kind = kind;
    switch (kind) {
    case Lava_token_kind_header:
        t->data.header.level = 0;
        Zinc_string_init(&t->data.header.title);
        break;
    case Lava_token_kind_text:
        Zinc_string_init(&t->data.text);
        break;
    case Lava_token_kind_backquote:
        Zinc_string_init(&t->data.backquote);
        break;
    case Lava_token_kind_eof:
        break;
    default:
        assert(false && "not a valid token kind");
    }
}

void Lava_token_destroy(Lava_token* t)
{
    switch (t->kind) {
    case Lava_token_kind_header:
        Zinc_string_destroy(&t->data.header.title);
        break;
    case Lava_token_kind_text:
        Zinc_string_destroy(&t->data.text);
        break;
    case Lava_token_kind_backquote:
        Zinc_string_destroy(&t->data.backquote);
        break;
    case Lava_token_kind_eof:
        break;
    default:
        assert(false && "not a valid token kind");
    }
}