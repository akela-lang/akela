#ifndef CENTIPEDE_TYPE_H
#define CENTIPEDE_TYPE_H

#include <stdint.h>
#include "zinc/zstring.h"
#include "zinc/vector.h"

typedef struct Cent_type Cent_type;
typedef struct Cent_type_def Cent_type_def;

typedef enum Cent_when_kind {
    Cent_when_run_time,
    Cent_when_comp_time,
} Cent_when_kind;

typedef struct Cent_type_param Cent_type_param;

struct Cent_type_param {
    Zinc_string name;
    Cent_type* type;
    Cent_when_kind when;
    Cent_type_param* next;
    Cent_type_param* prev;
};

typedef struct Cent_type_param_list {
    Cent_type_param* head;
    Cent_type_param* tail;
} Cent_type_param_list;

typedef struct Cent_type_node Cent_type_node;

struct Cent_type_node {
    Cent_type* type;
    Cent_type_node* next;
    Cent_type_node* prev;
};

typedef struct Cent_type_list Cent_type_list;

struct Cent_type_list {
    Cent_type_node* head;
    Cent_type_node* tail;
};

typedef struct Cent_type_field Cent_type_field;

struct Cent_type_field {
    Zinc_string name;
    Cent_type* type;
    Cent_type_field* next;
    Cent_type_field* prev;
};

typedef struct Cent_type_field_list Cent_type_field_list;

struct Cent_type_field_list {
    Cent_type_field* head;
    Cent_type_field* tail;
};

struct Cent_type {
    enum {
        CENT_TYPE_NONE,
        CENT_TYPE_SCALAR,
        CENT_TYPE_ARRAY,
        CENT_TYPE_SLICE,
        CENT_TYPE_POINTER,
        CENT_TYPE_FUNCTION,
    } tag;
    union {
        struct { Cent_type_def* td; } CENT_TYPE_SCALAR;
        struct { uint64_t dim; Cent_type* type; } CENT_TYPE_ARRAY;
        struct { uint64_t dim; Cent_type* scalar; } CENT_TYPE_ARRAY_CONST;
        struct { Cent_type* type; } CENT_TYPE_SLICE;
        struct { Cent_type* type; } CENT_TYPE_POINTER;
        struct { Zinc_string name; Cent_type_param_list inputs; Cent_type_list outputs; } CENT_TYPE_FUNCTION;
        int8_t CENT_TYPE_TYPE;
    } data;
};

struct Cent_type_def {
    Zinc_string name;
    enum {
        CENT_TYPE_DEF_INTEGER,
        CENT_TYPE_DEF_NATURAL,
        CENT_TYPE_DEF_REAL,
        CENT_TYPE_DEF_BOOLEAN,
        CENT_TYPE_DEF_STRING,
        CENT_TYPE_DEF_STRUCT,
    } tag;
    union {
        struct { uint8_t bit_count; } CENT_TYPE_DEF_INTEGER;
        struct { uint8_t bit_count; } CENT_TYPE_DEF_NATURAL;
        struct { uint8_t bit_count; } CENT_TYPE_DEF_REAL;
        uint8_t CENT_TYPE_DEF_BOOLEAN;
        uint8_t CENT_TYPE_DEF_STRING;
        struct { Cent_type_field_list fields; } CENT_TYPE_DEF_STRUCT;
    } data;
};

void Cent_type_param_init(Cent_type_param* tp);
void Cent_type_param_create(Cent_type_param** tp);
void Cent_type_param_destroy(Cent_type_param* tp);
void Cent_type_param_list_init(Cent_type_param_list* list);
void Cent_type_param_list_create(Cent_type_param_list** list);
void Cent_type_param_list_add(Cent_type_param_list* list, Cent_type_param* tp);
void Cent_type_param_list_destroy(Cent_type_param_list* list);

void Cent_type_node_init(Cent_type_node* tn);
void Cent_type_node_create(Cent_type_node** tn);
void Cent_type_node_destroy(Cent_type_node* tn);
void Cent_type_list_init(Cent_type_list* list);
void Cent_type_list_create(Cent_type_list** list);
void Cent_type_list_add(Cent_type_list* list, Cent_type_node* tn);
void Cent_type_list_destroy(Cent_type_list* list);

void Cent_type_field_init(Cent_type_field* tf);
void Cent_type_field_create(Cent_type_field** tf);
void Cent_type_field_destroy(Cent_type_field* tf);
void Cent_type_field_list_init(Cent_type_field_list* list);
void Cent_type_field_list_create(Cent_type_field_list** list);
void Cent_type_field_list_add(Cent_type_field_list* list, Cent_type_field* tf);
void Cent_type_field_list_destroy(Cent_type_field_list* list);

#endif
