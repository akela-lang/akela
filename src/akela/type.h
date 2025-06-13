#ifndef AKELA_TYPE_H
#define AKELA_TYPE_H

#include <stddef.h>
#include "zinc/zstring.h"
#include <inttypes.h>

typedef enum Ake_TypeDefKind Ake_TypeDefKind;
typedef struct Ake_TypeDef Ake_TypeDef;
typedef enum Ake_TypeUseKind Ake_TypeUseKind;
typedef struct Ake_TypeFieldKind Ake_TypeFieldKind;
typedef struct Ake_TypeUse Ake_TypeUse;
typedef struct Ake_FieldKind Ake_FieldKind;
typedef struct Ake_TypeField Ake_TypeField;
typedef struct Ake_TypeParam Ake_TypeParam;

enum Ake_TypeUseKind {
    AKE_TYPE_SCALAR,
    AKE_TYPE_ARRAY,
    AKE_TYPE_SLICE,
    AKE_TYPE_POINTER,
    AKE_TYPE_FUNCTION,
};

struct Ake_TypeUse {
    Ake_TypeUseKind kind;
    union {
        struct { Ake_TypeDef* td; } scalar;
        struct { size_t dim; Ake_TypeUse* tu; } array;
        struct { size_t dim; Ake_TypeUse* tu; } array_const;
        struct { Ake_TypeUse* tu; } slice;
        struct { Ake_TypeUse* tu; } pointer;
        struct {
            Zinc_string* name;
            Ake_TypeParam* input_head;
            Ake_TypeParam* input_tail;
            Ake_TypeParam* output_head;
            Ake_TypeParam* output_tail;
        } function;
    } data;
};

enum Ake_TypeDefKind {
    AKE_TYPE_DEF_INTEGER,
    AKE_TYPE_DEF_NATURAL,
    AKE_TYPE_DEF_REAL,
    AKE_TYPE_DEF_STRUCT,
};

struct Ake_TypeDef {
    Ake_TypeDefKind kind;
    union {
        struct { uint8_t bit_count; } integer;
        struct { uint8_t bit_count; } natural;
        struct { uint8_t bit_count; } real;
        struct { Ake_TypeField* head; Ake_TypeField* tail; } fields;
    } data;
};

struct Ake_TypeField {
    Zinc_string name;
    Ake_TypeUse* tu;
    Ake_TypeField* next;
    Ake_TypeField* prev;
};

struct Ake_TypeParam {
    Zinc_string name;
    Ake_TypeUse* tu;
    Ake_TypeField* next;
    Ake_TypeField* prev;
};

#endif