#include "token.h"
#include "zinc/memory.h"

void Cent_token_init(Cent_token *t)
{
    t->type = Cent_token_none;
    buffer_init(&t->value);
    location_init(&t->loc);
}

void Cent_token_create(Cent_token **t)
{
    malloc_safe((void**)t, sizeof(Cent_token));
    Cent_token_init(*t);
}

void Cent_token_destroy(Cent_token *t)
{
    buffer_destroy(&t->value);
}