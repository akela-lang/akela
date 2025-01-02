#ifndef CENTIPEDE_ENUMERATE_H
#define CENTIPEDE_ENUMERATE_H

#include "zinc/buffer.h"

typedef struct Cent_enumerate_value {
    struct buffer display;
    size_t value;
    struct Cent_enumerate_value* next;
    struct Cent_enumerate_value* prev;
} Cent_enumerate_value;

typedef struct Cent_enum_type {
    struct buffer name;
    Cent_enumerate_value* head;
    Cent_enumerate_value* tail;
} Cent_enum_type;

void Cent_enumerate_value_init(Cent_enumerate_value* ev);
void Cent_enumerate_value_create(Cent_enumerate_value** ev);
void Cent_enumerate_value_destroy(Cent_enumerate_value* ev);

void Cent_enumerate_init(Cent_enum_type* en);
void Cent_enumerate_create(Cent_enum_type** en);
void Cent_enumerate_destroy(Cent_enum_type* en);
void Cent_enumerate_add(Cent_enum_type* en, Cent_enumerate_value* ev);
size_t Cent_enumerate_count(Cent_enum_type* en);
void Cent_enumerate_add_name(Cent_enum_type* en, struct buffer* name);
Cent_enumerate_value* Cent_enumerate_get(Cent_enum_type* en, struct buffer* name);

#endif