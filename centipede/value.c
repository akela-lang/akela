#include "value.h"

#include <akela/type_def.h>

#include "zinc/memory.h"

void Cent_value_init(Cent_value *value)
{
    value->type = Cent_value_type_none;
    value->number_type = Cent_number_type_none;
    value->has_error = false;
    hash_table_init(&value->properties, 16);
    value->next = NULL;
    value->prev = NULL;
    value->head = NULL;
    value->tail = NULL;
    value->parent = NULL;
}

void Cent_value_create(Cent_value **value)
{
    malloc_safe((void**)value, sizeof(Cent_value));
    Cent_value_init(*value);
}

void Cent_data_init(Cent_data *data, Cent_value_type type)
{
    if (type == Cent_value_type_string) {
        buffer_init(&data->string);
    } else if (type == Cent_value_type_enum) {
        buffer_init(&data->enumeration.id1);
        buffer_init(&data->enumeration.id2);
        buffer_init(&data->enumeration.display);
        location_init(&data->enumeration.loc1);
        location_init(&data->enumeration.loc2);
    } else if (type == Cent_value_type_object) {
        buffer_init(&data->name);
    }
}

void Cent_data_destroy(Cent_data *data, Cent_value_type type)
{
    if (type == Cent_value_type_string) {
        buffer_destroy(&data->string);
    } else if (type == Cent_value_type_enum) {
        buffer_destroy(&data->enumeration.id1);
        buffer_destroy(&data->enumeration.id2);
        buffer_destroy(&data->enumeration.display);
    } else if (type == Cent_value_type_object) {
        buffer_destroy(&data->name);
    }
}

void Cent_value_set_type(Cent_value *value, Cent_value_type type)
{
    value->type = type;
    Cent_data_init(&value->data, type);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_value_destroy(Cent_value *value)
{
    hash_table_map(&value->properties, (hash_table_func)Cent_value_free);
    Cent_value* p = value->head;
    while (p) {
        Cent_value* temp = p;
        p = p->next;
        Cent_value_free(temp);
    }
    hash_table_destroy(&value->properties);

    Cent_data_destroy(&value->data, value->type);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_value_free(Cent_value *value)
{
    if (value) {
        Cent_value_destroy(value);
        free(value);
    }
}

void Cent_value_set(Cent_value* value, struct buffer* name, Cent_value* value2)
{
    hash_table_add(&value->properties, name, value2);
}

void Cent_value_set_str(Cent_value* value, char* name, Cent_value* value2)
{
    hash_table_add_str(&value->properties, name, value2);
}

Cent_value* Cent_value_get(Cent_value* value, struct buffer* name)
{
    return hash_table_get(&value->properties, name);
}

Cent_value* Cent_value_get_str(Cent_value* value, char* name)
{
    return hash_table_get_str(&value->properties, name);
}

void Cent_value_add(Cent_value* parent, Cent_value* child)
{
    if (parent->head && parent->tail) {
        parent->tail->next = child;
        child->prev = parent->tail;
        parent->tail = child;
    } else {
        parent->head = child;
        parent->tail = child;
    }
    child->parent = parent;
}