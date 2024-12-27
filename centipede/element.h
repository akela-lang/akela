#ifndef CENTIPEDE_ELEMENT_H
#define CENTIPEDE_ELEMENT_H

#include "zinc/buffer.h"
#include "zinc/hash.h"
#include "zinc/list.h"
#include "value.h"

/* the name and types of an element */
typedef struct element_type {
    struct buffer name;
    Cent_value_type type;
    Cent_number_type number_type;
    struct hash_table properties;
    struct list children;
} Cent_element_type;

typedef struct Cent_property_type {
    struct buffer name;
    Cent_element_type* et;
    bool required;
} Cent_property_type;

void Cent_element_init(Cent_element_type* et);
void Cent_element_create(Cent_element_type** et);
void Cent_element_destroy(Cent_element_type* et);
void Cent_element_set(Cent_element_type* et, struct buffer* name, void* value);
void Cent_element_set_str(Cent_element_type* et, char* name, void* value);
void Cent_element_add(Cent_element_type* et, Cent_element_type* et2);

void Cent_property_type_init(Cent_property_type* pt);
void Cent_property_type_create(Cent_property_type** pt);
void Cent_property_type_destroy(Cent_property_type* pt);

#endif