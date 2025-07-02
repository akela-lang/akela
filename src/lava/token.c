#include "token.h"
#include <zinc/memory.h>

void Lava_token_init(Lava_token* t)
{
    t->kind = Lava_token_kind_none;
    Zinc_string_init(&t->text);
    Zinc_location_init(&t->loc);
}

void Lava_token_create(Lava_token** t)
{
    Zinc_malloc_safe((void**)t, sizeof(Lava_token));
    Lava_token_init(*t);
}

void Lava_token_destroy(Lava_token* t)
{
    if (t) {
        Zinc_string_destroy(&t->text);
    }
}