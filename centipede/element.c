#include "element.h"
#include "zinc/memory.h"

void Cent_element_init(Cent_element_type* et)
{
    buffer_init(&et->name);
    et->type = Cent_value_type_none;
    et->number_type = Cent_number_type_none;
    hash_table_init(&et->properties, 32);
    list_init(&et->children);
}

void Cent_element_create(Cent_element_type** et)
{
    malloc_safe((void**)et, sizeof(Cent_element_type));
    Cent_element_init(*et);
}

void Cent_element_destroy(Cent_element_type* et)
{
    buffer_destroy(&et->name);
    hash_table_destroy(&et->properties);
    list_destroy(&et->children, NULL);
}

void Cent_element_set(Cent_element_type* et, struct buffer* name, void* value)
{
    hash_table_add(&et->properties, name, value);
}

void Cent_element_set_str(Cent_element_type* et, char* name, void* value)
{
    hash_table_add_str(&et->properties, name, value);
}

void Cent_element_add(Cent_element_type* et, Cent_element_type* et2)
{
    list_add_item(&et->children, et2);
}

void Cent_property_type_init(Cent_property_type* pt)
{
    buffer_init(&pt->name);
    pt->et = NULL;
    pt->required = false;
}

void Cent_property_type_create(Cent_property_type** pt)
{
    malloc_safe((void**)pt, sizeof(Cent_property_type));
    Cent_property_type_init(*pt);
}

void Cent_property_type_destroy(Cent_property_type* pt)
{
    buffer_destroy(&pt->name);
}