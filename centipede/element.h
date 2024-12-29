#ifndef CENTIPEDE_ELEMENT_H
#define CENTIPEDE_ELEMENT_H

#include "zinc/buffer.h"
#include "zinc/hash.h"
#include "zinc/list.h"
#include "value.h"

typedef struct Cent_element_type_node {
    struct Cent_element_type* et;
    struct location loc;
    bool has_error;
    struct Cent_element_type_node* next;
    struct Cent_element_type_node* prev;
} Cent_element_type_node;

typedef struct Cent_element_type_list
{
    Cent_element_type_node* head;
    Cent_element_type_node* tail;
} Cent_element_type_list;

typedef struct Cent_property_type {
    struct buffer name;
    struct Cent_element_type* et;
    bool required;
    struct location loc;
    bool has_error;
} Cent_property_type;

typedef struct Cent_element_type {
    struct buffer name;
    Cent_value_type type;
    Cent_number_type number_type;
    struct hash_table properties;
    struct Cent_element_type_list children;
    struct location loc;
    bool has_error;
} Cent_element_type;

void Cent_element_init(Cent_element_type* et);
void Cent_element_create(Cent_element_type** et);
void Cent_element_destroy(Cent_element_type* et);
void Cent_element_set(Cent_element_type* et, struct buffer* name, Cent_property_type* value);
void Cent_element_set_str(Cent_element_type* et, char* name, Cent_property_type* value);
Cent_property_type* Cent_element_get(Cent_element_type* et, struct buffer* name);
Cent_property_type* Cent_element_get_str(Cent_element_type* et, char* name);
void Cent_element_add(Cent_element_type* et, Cent_element_type_node* node);
void Cent_element_add_et(
    Cent_element_type* et,
    Cent_element_type* et2,
    struct location* loc,
    bool has_error);

void Cent_property_type_init(Cent_property_type* pt);
void Cent_property_type_create(Cent_property_type** pt);
void Cent_property_type_destroy(Cent_property_type* pt);
void Cent_property_type_free(Cent_property_type* pt);

void Cent_element_type_node_init(Cent_element_type_node* node);
void Cent_element_type_node_create(Cent_element_type_node** node);

void Cent_element_type_list_init(Cent_element_type_list* list);
void Cent_element_type_list_create(Cent_element_type_list** list);
void Cent_element_type_list_destroy(Cent_element_type_list* list);
void Cent_element_type_list_add(Cent_element_type_list* list, Cent_element_type_node* node);
void Cent_element_type_list_add_et(
    Cent_element_type_list* list,
    Cent_element_type* et2,
    struct location* loc,
    bool has_error);

#endif