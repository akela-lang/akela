#include "token.h"
#include "zinc/memory.h"

void Json_token_init(Json_token* t)
{
    t->type = Json_token_none;
    buffer_init(&t->value);
}

void Json_token_create(Json_token** t)
{
    malloc_safe((void**)&t, sizeof(Json_token));
    Json_token_init(*t);
}

void Json_token_destroy(Json_token* t)
{
    buffer_destroy(&t->value);
}