#include "enumerate.h"
#include "zinc/memory.h"

void Cent_enumerate_value_init(Cent_enumerate_value* ev)
{
    buffer_init(&ev->display);
    ev->value = 0;
    ev->next = NULL;
    ev->prev = NULL;
}

void Cent_enumerate_value_create(Cent_enumerate_value** ev)
{
    malloc_safe((void**)ev, sizeof(Cent_enumerate_value));
    Cent_enumerate_value_init(*ev);
}

void Cent_enumerate_value_destroy(Cent_enumerate_value* ev)
{
    buffer_destroy(&ev->display);
}

void Cent_enumerate_init(Cent_enum_type* en)
{
    buffer_init(&en->name);
    en->head = NULL;
    en->tail = NULL;
}

void Cent_enumerate_create(Cent_enum_type** en)
{
    malloc_safe((void**)en, sizeof(Cent_enum_type));
    Cent_enumerate_init(*en);
}

void Cent_enumerate_destroy(Cent_enum_type* en)
{
    buffer_destroy(&en->name);

    Cent_enumerate_value* ev = en->head;
    while (ev) {
        Cent_enumerate_value* temp = ev;
        ev = ev->next;
        Cent_enumerate_value_destroy(temp);
        free(temp);
    }
}

void Cent_enumerate_add(Cent_enum_type* en, Cent_enumerate_value* ev)
{
    if (en->head && en->tail) {
        en->tail->next = ev;
        ev->prev = en->tail;
        en->tail = ev;
    } else {
        en->head = ev;
        en->tail = ev;
    }
}

size_t Cent_enumerate_count(Cent_enum_type* en)
{
    size_t count = 0;
    Cent_enumerate_value* ev = en->head;
    while (ev) {
        count++;
        ev = ev->next;
    }

    return count;
}

void Cent_enumerate_add_name(Cent_enum_type* en, struct buffer* name)
{
    Cent_enumerate_value* ev = NULL;
    Cent_enumerate_value_create(&ev);
    buffer_copy(name, &ev->display);
    ev->value = Cent_enumerate_count(en);
    Cent_enumerate_add(en, ev);
}

Cent_enumerate_value* Cent_enumerate_get(Cent_enum_type* en, struct buffer* name)
{
    Cent_enumerate_value* ev = en->head;
    while (ev) {
        if (buffer_compare(name, &ev->display)) {
            return ev;
        }
        ev = ev->next;
    }

    return NULL;
}