#ifndef CENTIPEDE_ELEMENT_H
#define CENTIPEDE_ELEMENT_H

#include "zinc/zstring.h"
#include "zinc/hash_map_string.h"
#include "zinc/list.h"
#include "value.h"
#include "enumerate.h"

typedef struct Cent_element_type Cent_element_type;
typedef struct Cent_types_node Cent_types_node;
typedef struct Cent_types_list Cent_types_list;
typedef struct Cent_property_type Cent_property_type;
typedef enum Cent_types Cent_types;
typedef struct Cent_variant_type Cent_variant_type;
typedef struct Cent_variant_list Cent_variant_list;

enum Cent_types {
    Cent_types_none,
    Cent_types_element,
    Cent_types_enum,
};

struct Cent_types_node {
    Cent_types type;
    union {
        Cent_element_type* et;
        Cent_enum_type* en;
    } data;
    Zinc_location loc;
    bool has_error;
    Cent_types_node* next;
    Cent_types_node* prev;
};

struct Cent_types_list {
    Cent_types_node* head;
    Cent_types_node* tail;
};

struct Cent_property_type {
    Zinc_string name;
    Cent_types type;
    union {
        Cent_element_type* et;
        Cent_enum_type* en;
    } data;
    bool required;
    Zinc_location loc;
    bool has_error;
};

struct Cent_variant_type {
    Zinc_string name;
    Zinc_hash_map_string properties;
    Cent_types_list children;
    Zinc_location loc;
    bool has_error;
    Cent_variant_type* next;
    Cent_variant_type* prev;
};

struct Cent_variant_list {
    Cent_variant_type* head;
    Cent_variant_type* tail;
};

struct Cent_element_type {
    Zinc_string name;
    Cent_enum_type* tag;
    Cent_value_type type;
    Zinc_hash_map_string properties;
    Cent_types_list children;
    Cent_variant_list variants;
    Zinc_location loc;
    bool has_error;
};

void Cent_element_init(Cent_element_type* et);
void Cent_element_create(Cent_element_type** et);
void Cent_element_destroy(Cent_element_type* et);
void Cent_element_set(Cent_element_type* et, Zinc_string* name, Cent_property_type* value);
void Cent_element_set_str(Cent_element_type* et, char* name, Cent_property_type* value);
Cent_property_type* Cent_element_get(Cent_element_type* et, Zinc_string* name);
Cent_property_type* Cent_element_get_str(Cent_element_type* et, char* name);
void Cent_element_add(Cent_element_type* et, Cent_types_node* node);
void Cent_element_add_et(
    Cent_element_type* et,
    Cent_element_type* et2,
    Zinc_location* loc,
    bool has_error);
void Cent_element_add_en(
    Cent_element_type* et,
    Cent_enum_type* en,
    Zinc_location* loc,
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
    Zinc_location* loc,
    bool has_error);
void Cent_types_list_add_en(
    Cent_types_list* list,
    Cent_enum_type* en,
    Zinc_location* loc,
    bool has_error);

#endif
