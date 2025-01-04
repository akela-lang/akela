#include "value.h"

#include <akela/type_def.h>

#include "zinc/memory.h"
#include "zinc/hash_map_size_t.h"
#include <assert.h>

void Cent_value_init(Cent_value *value)
{
    buffer_init(&value->name);
    value->type = Cent_value_type_none;
    value->number_type = Cent_number_type_none;
    value->has_error = false;
    value->next = NULL;
    value->prev = NULL;
    value->parent = NULL;
    value->n = NULL;
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
        hash_table_init(&data->object.properties, 16);
        data->object.head = NULL;
        data->object.tail = NULL;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_data_destroy(Cent_data *data, Cent_value_type type)
{
    if (type == Cent_value_type_string) {
        buffer_destroy(&data->string);
    } else if (type == Cent_value_type_enum) {
        buffer_destroy(&data->enumeration.id1);
        buffer_destroy(&data->enumeration.id2);
        buffer_destroy(&data->enumeration.display);
    } else if (type == Cent_value_type_object) {
        hash_table_map(&data->object.properties, (hash_table_func)Cent_value_free);
        Cent_value* p = data->object.head;
        while (p) {
            Cent_value* temp = p;
            p = p->next;
            Cent_value_free(temp);
        }
        hash_table_destroy(&data->object.properties);
    }
}

void Cent_data_free(Cent_data *data, Cent_value_type type)
{
    Cent_data_destroy(data, type);
    free(data);
}

void Cent_value_set_type(Cent_value *value, Cent_value_type type)
{
    value->type = type;
    Cent_data_init(&value->data, type);
}

Hash_map_size_t* Cent_value_hash_map = NULL;

bool Cent_value_destroy_setup()
{
    if (!Cent_value_hash_map) {
        Hash_map_size_t_create(&Cent_value_hash_map, 32);
        return true;
    }

    return false;
}

void Cent_value_destroy_teardown()
{
    if (Cent_value_hash_map) {
        Hash_map_size_t_destroy(Cent_value_hash_map);
        free(Cent_value_hash_map);
        Cent_value_hash_map = NULL;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_value_destroy(Cent_value *value)
{
    buffer_destroy(&value->name);
    if (Cent_value_hash_map) {
        Cent_value* value2 = Hash_map_size_t_get(Cent_value_hash_map, (size_t)value);
        if (value2) {
            return;
        }
    }

    Cent_data_destroy(&value->data, value->type);

    if (Cent_value_hash_map) {
        Hash_map_size_t_add(Cent_value_hash_map, (size_t)value, value);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_value_free(Cent_value *value)
{
    if (Cent_value_hash_map) {
        Cent_value* value2 = Hash_map_size_t_get(Cent_value_hash_map, (size_t)value);
        if (value2) {
            return;
        }
    }

    if (value) {
        Cent_value_destroy(value);
        free(value);
    }

    if (Cent_value_hash_map) {
        Hash_map_size_t_add(Cent_value_hash_map, (size_t)value, value);
    }
}

void Cent_value_set(Cent_value* value, struct buffer* name, Cent_value* value2)
{
    assert(value->type == Cent_value_type_object);
    hash_table_add(&value->data.object.properties, name, value2);
}

void Cent_value_set_str(Cent_value* value, char* name, Cent_value* value2)
{
    assert(value->type == Cent_value_type_object);
    hash_table_add_str(&value->data.object.properties, name, value2);
}

Cent_value* Cent_value_get(Cent_value* value, struct buffer* name)
{
    assert(value->type == Cent_value_type_object);
    return hash_table_get(&value->data.object.properties, name);
}

Cent_value* Cent_value_get_str(Cent_value* value, char* name)
{
    assert(value->type == Cent_value_type_object);
    return hash_table_get_str(&value->data.object.properties, name);
}

void Cent_value_add(Cent_value* parent, Cent_value* child)
{
    assert(parent->type == Cent_value_type_object);
    if (parent->data.object.head && parent->data.object.tail) {
        parent->data.object.tail->next = child;
        child->prev = parent->data.object.tail;
        parent->data.object.tail = child;
    } else {
        parent->data.object.head = child;
        parent->data.object.tail = child;
    }
    child->parent = parent;
}