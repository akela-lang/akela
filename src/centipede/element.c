#include "element.h"
#include "zinc/memory.h"
#include <assert.h>

void Cent_element_init(Cent_element_type* et)
{
    Zinc_string_init(&et->name);
    et->type = Cent_value_type_none;
    et->number_type = Cent_number_type_none;
    Zinc_hash_map_string_init(&et->properties, 32);
    Cent_types_list_init(&et->children);
    Zinc_location_init(&et->loc);
    et->has_error = false;
}

void Cent_element_create(Cent_element_type** et)
{
    malloc_safe((void**)et, sizeof(Cent_element_type));
    Cent_element_init(*et);
}

void Cent_element_destroy(Cent_element_type* et)
{
    Zinc_string_destroy(&et->name);
    Zinc_hash_map_str_map(&et->properties, (Zinc_hash_map_string_func)Cent_property_type_free);
    Zinc_hash_map_string_destroy(&et->properties);
    Cent_types_list_destroy(&et->children);
}

void Cent_element_set(Cent_element_type* et, struct Zinc_string* name, Cent_property_type* value)
{
    Zinc_hash_map_string_add(&et->properties, name, value);
}

void Cent_element_set_str(Cent_element_type* et, char* name, Cent_property_type* value)
{
    Zinc_hash_map_string_add_str(&et->properties, name, value);
}

Cent_property_type* Cent_element_get(Cent_element_type* et, struct Zinc_string* name)
{
    return Zinc_hash_map_string_get(&et->properties, name);
}

Cent_property_type* Cent_element_get_str(Cent_element_type* et, char* name)
{
    return Zinc_hash_map_string_get_str(&et->properties, name);
}

void Cent_element_add(Cent_element_type* et, Cent_types_node* node)
{
    Cent_types_list_add(&et->children, node);
}

void Cent_element_add_et(
    Cent_element_type* et,
    Cent_element_type* et2,
    struct Zinc_location* loc,
    bool has_error)
{
    Cent_types_list_add_et(&et->children, et2, loc, has_error);
}

void Cent_element_add_en(
    Cent_element_type* et,
    Cent_enum_type* en,
    struct Zinc_location* loc,
    bool has_error)
{
    Cent_types_list_add_en(&et->children, en, loc, has_error);
}

void Cent_property_type_init(Cent_property_type* pt)
{
    Zinc_string_init(&pt->name);
    pt->type = Cent_types_none;
    pt->required = false;
    Zinc_location_init(&pt->loc);
    pt->has_error = false;
}

void Cent_property_type_create(Cent_property_type** pt)
{
    malloc_safe((void**)pt, sizeof(Cent_property_type));
    Cent_property_type_init(*pt);
}

void Cent_property_type_set_type(Cent_property_type* pt, Cent_types type)
{
    pt->type = type;
    if (pt->type == Cent_types_element) {
        pt->data.et = NULL;
    } else if (pt->type == Cent_types_enum) {
        pt->data.en = NULL;
    } else {
        assert(false && "invalid property type type");
    }
}

void Cent_property_type_destroy(Cent_property_type* pt)
{
    Zinc_string_destroy(&pt->name);
}

void Cent_property_type_free(Cent_property_type* pt)
{
    Cent_property_type_destroy(pt);
    free(pt);
}

void Cent_types_node_init(Cent_types_node* node)
{
    node->type = Cent_types_none;
    Zinc_location_init(&node->loc);
    node->has_error = false;
    node->next = NULL;
    node->prev = NULL;
}

void Cent_types_node_create(Cent_types_node** node)
{
    malloc_safe((void**)node, sizeof(Cent_types_node));
    Cent_types_node_init(*node);
}

void Cent_types_node_set_type(Cent_types_node* node, Cent_types type)
{
    node->type = type;
    if (type == Cent_types_element) {
        node->data.et = NULL;
    } else if (type == Cent_types_enum) {
        node->data.en = NULL;
    } else {
        assert(false && "invalid node types type");
    }
}

void Cent_types_list_init(Cent_types_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Cent_types_list_create(Cent_types_list** list)
{
    malloc_safe((void**)list, sizeof(Cent_types_list));
    Cent_types_list_init(*list);
}

void Cent_types_list_destroy(Cent_types_list* list)
{
    Cent_types_node* node = list->head;
    while (node) {
        Cent_types_node* temp = node;
        node = node->next;
        free(temp);
    }
}

void Cent_types_list_add(Cent_types_list* list, Cent_types_node* node)
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

void Cent_types_list_add_et(
    Cent_types_list* list,
    Cent_element_type* et2,
    struct Zinc_location* loc,
    bool has_error)
{
    Cent_types_node* node = NULL;
    Cent_types_node_create(&node);
    Cent_types_node_set_type(node, Cent_types_element);
    node->data.et = et2;
    node->loc = *loc;
    node->has_error = has_error;
    Cent_types_list_add(list, node);
}

void Cent_types_list_add_en(
    Cent_types_list* list,
    struct Cent_enum_type* en,
    struct Zinc_location* loc,
    bool has_error)
{
    Cent_types_node* node = NULL;
    Cent_types_node_create(&node);
    Cent_types_node_set_type(node, Cent_types_enum);
    node->data.en = en;
    node->loc = *loc;
    node->has_error = has_error;
    Cent_types_list_add(list, node);
}