#include "value.h"
#include "zinc/memory.h"

void Cent_value_init(Cent_value *value)
{
    value->type = Cent_value_type_none;
    value->number_type = Cent_number_type_none;
    buffer_init(&value->display);
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

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_value_destroy(Cent_value *value)
{
    buffer_destroy(&value->display);
    hash_table_map(&value->properties, (hash_table_func)Cent_value_free);
    Cent_value* p = value->head;
    while (p) {
        Cent_value* temp = p;
        p = p->next;
        Cent_value_free(temp);
    }
    hash_table_destroy(&value->properties);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_value_free(Cent_value *value)
{
    Cent_value_destroy(value);
    free(value);
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

void Cent_value_add(Cent_value* value, Cent_value* value2)
{
    if (value->head && value->tail) {
        value->tail->next = value2;
        value2->prev = value->tail;
        value->tail = value2;
    } else {
        value->head = value2;
        value->tail = value2;
    }
}