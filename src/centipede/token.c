#include "token.h"
#include "zinc/memory.h"

void Cent_token_init(Cent_token *t)
{
    t->type = Cent_token_none;
    t->number_type = Cent_number_type_none;
    t->builtin_type = Cent_builtin_type_none;
    Zinc_string_init(&t->value);
    Zinc_location_init(&t->loc);
}

void Cent_token_create(Cent_token **t)
{
    malloc_safe((void**)t, sizeof(Cent_token));
    Cent_token_init(*t);
}

void Cent_token_destroy(Cent_token *t)
{
    if (t) {
        Zinc_string_destroy(&t->value);
    }
}

void Cent_print_token(Cent_token *t)
{
    printf("%s", Cent_token_name(t->type));
    if (t->value.size > 0) {
        Zinc_string_finish(&t->value);
        printf("-%s", t->value.buf);
    }
}