#ifndef CENTIPEDE_ENUMERATE_H
#define CENTIPEDE_ENUMERATE_H

#include "zinc/buffer.h"
#include "zinc/error.h"

typedef struct Cent_enum_value {
    struct Zinc_string display;
    size_t value;
    struct location loc;
    struct Cent_enum_value* next;
    struct Cent_enum_value* prev;
} Cent_enum_value;

typedef struct Cent_enum_type {
    struct Zinc_string name;
    struct location loc;
    Cent_enum_value* head;
    Cent_enum_value* tail;
} Cent_enum_type;

void Cent_enum_value_init(Cent_enum_value* ev);
void Cent_enum_value_create(Cent_enum_value** ev);
void Cent_enum_value_destroy(Cent_enum_value* ev);

void Cent_enumerate_init(Cent_enum_type* en);
void Cent_enumerate_create(Cent_enum_type** en);
void Cent_enumerate_destroy(Cent_enum_type* en);
void Cent_enumerate_add(Cent_enum_type* en, Cent_enum_value* ev);
size_t Cent_enumerate_count(Cent_enum_type* en);
void Cent_enumerate_add_name(Cent_enum_type* en, struct Zinc_string* name, struct location* loc);
Cent_enum_value* Cent_enumerate_get(Cent_enum_type* en, struct Zinc_string* name);

#endif