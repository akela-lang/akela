#include "element.h"
#include "zinc/memory.h"

void Cent_element_init(Cent_element_type* et)
{
    buffer_init(&et->name);
    et->type = Cent_value_type_none;
    et->number_type = Cent_number_type_none;
    hash_table_init(&et->properties, 32);
    Cent_element_type_list_init(&et->children);
    location_init(&et->loc);
    et->has_error = false;
}

void Cent_element_create(Cent_element_type** et)
{
    malloc_safe((void**)et, sizeof(Cent_element_type));
    Cent_element_init(*et);
}

void Cent_element_destroy(Cent_element_type* et)
{
    buffer_destroy(&et->name);
    hash_table_map(&et->properties, (hash_table_func)Cent_property_type_free);
    hash_table_destroy(&et->properties);
    Cent_element_type_list_destroy(&et->children);
}

void Cent_element_set(Cent_element_type* et, struct buffer* name, Cent_property_type* value)
{
    hash_table_add(&et->properties, name, value);
}

void Cent_element_set_str(Cent_element_type* et, char* name, Cent_property_type* value)
{
    hash_table_add_str(&et->properties, name, value);
}

Cent_property_type* Cent_element_get(Cent_element_type* et, struct buffer* name)
{
    return hash_table_get(&et->properties, name);
}

Cent_property_type* Cent_element_get_str(Cent_element_type* et, char* name)
{
    return hash_table_get_str(&et->properties, name);
}

void Cent_element_add(Cent_element_type* et, Cent_element_type_node* node)
{
    Cent_element_type_list_add(&et->children, node);
}

void Cent_element_add_et(
    Cent_element_type* et,
    Cent_element_type* et2,
    struct location* loc,
    bool has_error)
{
    Cent_element_type_list_add_et(&et->children, et2, loc, has_error);
}

void Cent_property_type_init(Cent_property_type* pt)
{
    buffer_init(&pt->name);
    pt->et = NULL;
    pt->required = false;
    location_init(&pt->loc);
    pt->has_error = false;
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

void Cent_property_type_free(Cent_property_type* pt)
{
    Cent_property_type_destroy(pt);
    free(pt);
}

void Cent_element_type_node_init(Cent_element_type_node* node)
{
    node->et = NULL;
    location_init(&node->loc);
    node->has_error = false;
    node->next = NULL;
    node->prev = NULL;
}

void Cent_element_type_node_create(Cent_element_type_node** node)
{
    malloc_safe((void**)node, sizeof(Cent_element_type_node));
    Cent_element_type_node_init(*node);
}

void Cent_element_type_list_init(Cent_element_type_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Cent_element_type_list_create(Cent_element_type_list** list)
{
    malloc_safe((void**)list, sizeof(Cent_element_type_list));
    Cent_element_type_list_init(*list);
}

void Cent_element_type_list_destroy(Cent_element_type_list* list)
{
    Cent_element_type_node* node = list->head;
    while (node) {
        Cent_element_type_node* temp = node;
        node = node->next;
        free(temp);
    }
}

void Cent_element_type_list_add(Cent_element_type_list* list, Cent_element_type_node* node)
{
    if (list->head && list->tail) {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    } else {
        list->head = node;
        list->tail = node;
    }
}

void Cent_element_type_list_add_et(
    Cent_element_type_list* list,
    Cent_element_type* et2,
    struct location* loc,
    bool has_error)
{
    Cent_element_type_node* node = NULL;
    Cent_element_type_node_create(&node);
    node->et = et2;
    node->loc = *loc;
    node->has_error = has_error;
    Cent_element_type_list_add(list, node);
}