#include "struct_element.h"
#include "zinc/memory.h"

void Struct_element_init(Struct_element* se)
{
    se->type = Struct_element_type_none;
    se->tu = NULL;
    se->func = NULL;
    se->index = 0;
}

void Struct_element_create(Struct_element** se)
{
    malloc_safe((void**)se, sizeof(Struct_element));
    Struct_element_init(*se);
}

void Struct_element_destroy(Struct_element* se)
{
    Ake_ast_destroy(se->tu);
    Ake_ast_destroy(se->func);
}