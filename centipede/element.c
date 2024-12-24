#include "element.h"
#include "zinc/memory.h"

void Cent_element_init(Cent_element* element)
{
    buffer_init(&element->name);
    hash_table_init(&element->properties, 32);
    list_init(&element->children);
}

void Cent_element_create(Cent_element** element)
{
    malloc_safe((void**)element, sizeof(Cent_element));
    Cent_element_init(*element);
}

void Cent_element_destroy(Cent_element* element)
{
    buffer_destroy(&element->name);
    hash_table_destroy(&element->properties);
    list_destroy(&element->children, NULL);
}

void Cent_element_set(Cent_element* element, struct buffer* name, void* value)
{
    hash_table_add(&element->properties, name, value);
}

void Cent_element_set_str(Cent_element* element, char* name, void* value)
{
    hash_table_add_str(&element->properties, name, value);
}

void Cent_element_add(Cent_element* element, Cent_element* element2)
{
    list_add_item(&element->children, element2);
}