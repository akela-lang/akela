#ifndef CENTIPEDE_ELEMENT_H
#define CENTIPEDE_ELEMENT_H

#include "zinc/buffer.h"
#include "zinc/hash.h"
#include "zinc/list.h"
#include "value.h"

/* the name and types of an element */
typedef struct element {
    struct buffer name;
    Cent_value_type type;
    Cent_number_type number_type;
    struct hash_table properties;
    struct list children;
} Cent_element;

void Cent_element_init(Cent_element* element);
void Cent_element_create(Cent_element** element);
void Cent_element_destroy(Cent_element* element);
void Cent_element_set(Cent_element* element, struct buffer* name, void* value);
void Cent_element_set_str(Cent_element* element, char* name, void* value);
void Cent_element_add(Cent_element* element, Cent_element* element2);

#endif