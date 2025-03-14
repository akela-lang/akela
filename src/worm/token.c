#include "token.h"
#include "zinc/memory.h"

void Worm_token_init(Worm_token *t)
{
    t->type = Worm_token_type_none;
    Zinc_string_init(&t->value);
    Zinc_location_init(&t->loc);
}

void Worm_token_create(Worm_token** t)
{
    Zinc_malloc_safe((void**)t, sizeof(Worm_token));
    Worm_token_init(*t);
}

void Worm_token_destroy(Worm_token *t)
{
    if (t) {
        Zinc_string_destroy(&t->value);
    }
}
