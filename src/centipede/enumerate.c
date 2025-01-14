#include "enumerate.h"
#include "zinc/memory.h"

void Cent_enum_value_init(Cent_enum_value* ev)
{
    Zinc_string_init(&ev->display);
    ev->value = 0;
    location_init(&ev->loc);
    ev->next = NULL;
    ev->prev = NULL;
}

void Cent_enum_value_create(Cent_enum_value** ev)
{
    malloc_safe((void**)ev, sizeof(Cent_enum_value));
    Cent_enum_value_init(*ev);
}

void Cent_enum_value_destroy(Cent_enum_value* ev)
{
    Zinc_string_destroy(&ev->display);
}

void Cent_enumerate_init(Cent_enum_type* en)
{
    Zinc_string_init(&en->name);
    location_init(&en->loc);
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
    Zinc_string_destroy(&en->name);

    Cent_enum_value* ev = en->head;
    while (ev) {
        Cent_enum_value* temp = ev;
        ev = ev->next;
        Cent_enum_value_destroy(temp);
        free(temp);
    }
}

void Cent_enumerate_add(Cent_enum_type* en, Cent_enum_value* ev)
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
    Cent_enum_value* ev = en->head;
    while (ev) {
        count++;
        ev = ev->next;
    }

    return count;
}

void Cent_enumerate_add_name(Cent_enum_type* en, struct Zinc_string* name, struct location* loc)
{
    Cent_enum_value* ev = NULL;
    Cent_enum_value_create(&ev);
    Zinc_string_copy(name, &ev->display);
    ev->value = Cent_enumerate_count(en);
    ev->loc = *loc;
    Cent_enumerate_add(en, ev);
}

Cent_enum_value* Cent_enumerate_get(Cent_enum_type* en, struct Zinc_string* name)
{
    Cent_enum_value* ev = en->head;
    while (ev) {
        if (Zinc_string_compare(name, &ev->display)) {
            return ev;
        }
        ev = ev->next;
    }

    return NULL;
}