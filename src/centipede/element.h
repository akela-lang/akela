#ifndef CENTIPEDE_ELEMENT_H
#define CENTIPEDE_ELEMENT_H

#include "zinc/buffer.h"
#include "zinc/hash.h"
#include "zinc/list.h"
#include "value.h"
#include "enumerate.h"

typedef enum Cent_types {
    Cent_types_none,
    Cent_types_element,
    Cent_types_enum,
} Cent_types;

typedef struct Cent_types_node {
    Cent_types type;
    union {
        struct Cent_element_type* et;
        struct Cent_enum_type* en;
    } data;
    struct location loc;
    bool has_error;
    struct Cent_types_node* next;
    struct Cent_types_node* prev;
} Cent_types_node;

typedef struct Cent_types_list {
    Cent_types_node* head;
    Cent_types_node* tail;
} Cent_types_list;

typedef struct Cent_property_type {
    struct Zinc_string name;
    Cent_types type;
    union {
        struct Cent_element_type* et;
        struct Cent_enum_type* en;
    } data;
    bool required;
    struct location loc;
    bool has_error;
} Cent_property_type;

typedef struct Cent_element_type {
    struct Zinc_string name;
    Cent_value_type type;
    Cent_number_type number_type;
    struct hash_table properties;
    struct Cent_types_list children;
    struct location loc;
    bool has_error;
} Cent_element_type;

void Cent_element_init(Cent_element_type* et);
void Cent_element_create(Cent_element_type** et);
void Cent_element_destroy(Cent_element_type* et);
void Cent_element_set(Cent_element_type* et, struct Zinc_string* name, Cent_property_type* value);
void Cent_element_set_str(Cent_element_type* et, char* name, Cent_property_type* value);
Cent_property_type* Cent_element_get(Cent_element_type* et, struct Zinc_string* name);
Cent_property_type* Cent_element_get_str(Cent_element_type* et, char* name);
void Cent_element_add(Cent_element_type* et, Cent_types_node* node);
void Cent_element_add_et(
    Cent_element_type* et,
    Cent_element_type* et2,
    struct location* loc,
    bool has_error);
void Cent_element_add_en(
    Cent_element_type* et,
    Cent_enum_type* en,
    struct location* loc,
    bool has_error);

void Cent_property_type_init(Cent_property_type* pt);
void Cent_property_type_create(Cent_property_type** pt);
void Cent_property_type_set_type(Cent_property_type* pt, Cent_types type);
void Cent_property_type_destroy(Cent_property_type* pt);
void Cent_property_type_free(Cent_property_type* pt);

void Cent_types_node_init(Cent_types_node* node);
void Cent_types_node_create(Cent_types_node** node);
void Cent_types_node_set_type(Cent_types_node* node, Cent_types type);

void Cent_types_list_init(Cent_types_list* list);
void Cent_types_list_create(Cent_types_list** list);
void Cent_types_list_destroy(Cent_types_list* list);
void Cent_types_list_add(Cent_types_list* list, Cent_types_node* node);
void Cent_types_list_add_et(
    Cent_types_list* list,
    Cent_element_type* et2,
    struct location* loc,
    bool has_error);
void Cent_types_list_add_en(
    Cent_types_list* list,
    struct Cent_enum_type* en,
    struct location* loc,
    bool has_error);

#endif