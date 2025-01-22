#include "token.h"
#include "zinc/memory.h"

void Worm_token_init(Worm_token *t)
{
    t->type = Worm_token_type_none;
    Zinc_string_init(&t->value);
    t->number_type = Worm_number_type_none;
}

void Worm_token_create(Worm_token** t)
{
    Zinc_malloc_safe((void**)t, sizeof(Worm_token));
    Worm_token_init(*t);
}

void Worm_token_destroy(Worm_token *t)
{
    Zinc_string_destroy(&t->value);
}