#include "struct_element.h"
#include "zinc/memory.h"

void Ake_struct_element_init(Ake_struct_element* se)
{
    se->type = Ake_struct_element_type_none;
    se->tu = NULL;
    se->func = NULL;
    se->index = 0;
}

void Ake_struct_element_create(Ake_struct_element** se)
{
    Zinc_malloc_safe((void**)se, sizeof(Ake_struct_element));
    Ake_struct_element_init(*se);
}

void Ake_struct_element_destroy(Ake_struct_element* se)
{
    Ake_ast_destroy(se->tu);
    Ake_ast_destroy(se->func);
}