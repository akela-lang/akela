#include "enumerate.h"
#include "zinc/memory.h"

void Cent_enumerate_init(Cent_enumerate* en)
{
    buffer_init(&en->name);
    buffer_list_init(&en->names);
}

void Cent_enumerate_create(Cent_enumerate** en)
{
    malloc_safe((void**)en, sizeof(Cent_enumerate*));
    Cent_enumerate_init(*en);
}

void Cent_enumerate_destroy(Cent_enumerate* en)
{
    buffer_destroy(&en->name);
    buffer_list_destroy(&en->names);
}

void Cent_enumerate_add_name(Cent_enumerate* en, struct buffer* name)
{
    buffer_list_add_bf(&en->names, name);
}

void Cent_enumerate_add_name_str(Cent_enumerate* en, char* name)
{
    buffer_list_add_str(&en->names, name);
}