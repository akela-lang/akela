#include "value.h"
#include <akela/type_def.h>
#include "zinc/memory.h"
#include "zinc/hash_map_size_t.h"
#include <assert.h>

void Cent_data_init(Cent_data *data, Cent_value_type type)
{
    if (type == Cent_value_type_string) {
        Zinc_string_init(&data->string);
    } else if (type == Cent_value_type_enum) {
        data->enumeration.enum_type = NULL;
        data->enumeration.enum_value = NULL;
    } else if (type == Cent_value_type_list) {
        data->list.head = NULL;
        data->list.tail = NULL;
    } else if (type == Cent_value_type_dict) {
        Zinc_hash_map_string_init(&data->dict.properties, 16);
    } else if (type == Cent_value_type_dag) {
        Zinc_hash_map_string_init(&data->dag.properties, 16);
        data->dag.head = NULL;
        data->dag.tail = NULL;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_data_destroy(Cent_data *data, Cent_value_type type)
{
    if (type == Cent_value_type_string) {
        Zinc_string_destroy(&data->string);
    } else if (type == Cent_value_type_list) {
        Cent_value* p = data->list.head;
        while (p) {
            Cent_value* temp = p;
            p = p->next;
            Cent_value_free(temp);
        }
    } else if (type == Cent_value_type_dict) {
        Zinc_hash_map_string_destroy(&data->dict.properties);
    } else if (type == Cent_value_type_dag) {
        Zinc_hash_map_string_map(&data->dag.properties, (Zinc_hash_map_string_func)Cent_value_free);
        Cent_value* p = data->dag.head;
        while (p) {
            Cent_value* temp = p;
            p = p->next;
            Cent_value_free(temp);
        }
        Zinc_hash_map_string_destroy(&data->dag.properties);
    }
}

void Cent_data_free(Cent_data *data, Cent_value_type type)
{
    Cent_data_destroy(data, type);
    free(data);
}

void Cent_value_init(Cent_value *value)
{
    Zinc_string_init(&value->name);
    value->type = Cent_value_type_none;
    value->has_error = false;
    value->next = NULL;
    value->prev = NULL;
    value->n = NULL;
    value->parent = NULL;
}

void Cent_value_create(Cent_value **value)
{
    Zinc_malloc_safe((void**)value, sizeof(Cent_value));
    Cent_value_init(*value);
}

void Cent_value_set_type(Cent_value *value, Cent_value_type type)
{
    value->type = type;
    Cent_data_init(&value->data, type);
}

Zinc_hash_map_size_t* Cent_value_hash_map = NULL;

bool Cent_value_destroy_setup()
{
    if (!Cent_value_hash_map) {
        Zinc_hash_map_size_t_create(&Cent_value_hash_map, 32);
        return true;
    }

    return false;
}

void Cent_value_destroy_teardown()
{
    if (Cent_value_hash_map) {
        Zinc_hash_map_size_t_destroy(Cent_value_hash_map);
        free(Cent_value_hash_map);
        Cent_value_hash_map = NULL;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_value_destroy(Cent_value *value)
{
    // if (value) {
    //     if (Cent_value_hash_map) {
    //         Cent_value* value2 = Zinc_hash_map_size_t_get(Cent_value_hash_map, (size_t)value);
    //         if (value2) {
    //             return;
    //         }
    //     }

        Zinc_string_destroy(&value->name);
        Cent_data_destroy(&value->data, value->type);

    //     if (Cent_value_hash_map) {
    //         Zinc_hash_map_size_t_add(Cent_value_hash_map, (size_t)value, value);
    //     }
    // }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_value_free(Cent_value *value)
{
    // if (Cent_value_hash_map) {
    //     Cent_value* value2 = Zinc_hash_map_size_t_get(Cent_value_hash_map, (size_t)value);
    //     if (value2) {
    //         return;
    //     }
    // }

    if (value) {
        Cent_value_destroy(value);
        free(value);
    }

    // if (Cent_value_hash_map) {
    //     Zinc_hash_map_size_t_add(Cent_value_hash_map, (size_t)value, value);
    // }
}

void Cent_value_set(Cent_value* value, struct Zinc_string* name, Cent_value* value2)
{
    assert(value->type == Cent_value_type_dag);
    Zinc_hash_map_string_add(&value->data.dag.properties, name, value2);
    value2->parent = value;
}

void Cent_value_set_str(Cent_value* value, char* name, Cent_value* value2)
{
    assert(value->type == Cent_value_type_dag);
    Zinc_hash_map_string_add_str(&value->data.dag.properties, name, value2);
    value2->parent = value;
}

Cent_value* Cent_value_get(Cent_value* value, struct Zinc_string* name)
{
    assert(value->type == Cent_value_type_dag);
    return Zinc_hash_map_string_get(&value->data.dag.properties, name);
}

Cent_value* Cent_value_get_str(Cent_value* value, char* name)
{
    assert(value->type == Cent_value_type_dag);
    return Zinc_hash_map_string_get_str(&value->data.dag.properties, name);
}

void Cent_value_add(Cent_value* parent, Cent_value* child)
{
    assert(parent->type == Cent_value_type_dag);
    if (parent->data.dag.head && parent->data.dag.tail) {
        parent->data.dag.tail->next = child;
        child->prev = parent->data.dag.tail;
        parent->data.dag.tail = child;
    } else {
        parent->data.dag.head = child;
        parent->data.dag.tail = child;
    }
    child->parent = parent;
}

Cent_value* Cent__data_copy_dst = NULL;
void Cent_data_copy_property(Zinc_string* name, Cent_value* value)
{
    Cent_value* value2 = Cent_value_clone(value);
    Zinc_hash_map_string_add(&Cent__data_copy_dst->data.dag.properties, name, value2);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_value_copy(Cent_value* src, Cent_value* dst)
{
    Zinc_string_add_string(&dst->name, &src->name);
    Cent_value_set_type(dst, src->type);
    dst->has_error = src->has_error;
    dst->next = NULL;
    dst->prev = NULL;
    dst->n = src->n;
    dst->parent = NULL;

    switch (src->type) {
    case Cent_value_type_integer:
        dst->data.integer = src->data.integer;
        break;
    case Cent_value_type_natural:
        dst->data.natural = src->data.natural;
        break;
    case Cent_value_type_real:
        dst->data.real = src->data.real;
        break;
    case Cent_value_type_string:
        Zinc_string_add_string(&dst->data.string, &src->data.string);
        break;
    case Cent_value_type_boolean:
        dst->data.boolean = src->data.boolean;
        break;
    case Cent_value_type_enum:
        dst->data.enumeration = src->data.enumeration;
        break;
    case Cent_value_type_dag:
        // properties
        Cent__data_copy_dst = dst;
        Zinc_hash_map_string_map_name(
            &src->data.dag.properties,
            (Zinc_hash_map_string_func_name)Cent_data_copy_property);
        // children
        Cent_value* p = src->data.dag.head;
        while (p) {
            Cent_value* p2 = Cent_value_clone(p);
            Cent_value_add(dst, p2);
            p = p->next;
        }
        break;
    default:
        assert(false && "invalid type");
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_value* Cent_value_clone(Cent_value* value)
{
    Cent_value* value2;
    Cent_value_create(&value2);
    Cent_value_copy(value, value2);
    return value2;
}