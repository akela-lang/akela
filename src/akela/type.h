#ifndef AKELA_TYPE_H
#define AKELA_TYPE_H

#include "forward.h"
#include <stddef.h>
#include "zinc/zstring.h"
#include <inttypes.h>
#include <assert.h>

typedef enum Ake_array_element_option {
    Ake_array_element_default,
    Ake_array_element_const,
} Ake_array_element_option;

typedef struct Ake_type_dimension {
    size_t size;
    Ake_array_element_option option;
} Ake_type_dimension;

typedef enum Ake_type_context {
    Ake_type_context_value,
    Ake_type_context_ptr,
} Ake_type_context;

typedef enum Ake_TypeKind {
    AKE_TYPE_NONE,
    AKE_TYPE_INTEGER,
    AKE_TYPE_NATURAL,
    AKE_TYPE_REAL,
    AKE_TYPE_BOOLEAN,
    AKE_TYPE_STRUCT,
    AKE_TYPE_ARRAY,
    AKE_TYPE_ARRAY_CONST,
    AKE_TYPE_SLICE,
    AKE_TYPE_POINTER,
    AKE_TYPE_FUNCTION,
    AKE_TYPE_COUNT,     // keep at end
} Ake_TypeKind;

static const char* Ake_TypeName(Ake_TypeKind kind)
{
    assert(kind < AKE_TYPE_COUNT);

    const char* name[AKE_TYPE_COUNT] = {
        "none",
        "integer",
        "natural",
        "real",
        "boolean",
        "struct",
        "array",
        "array-const",
        "slice",
        "pointer",
        "function",
    };
    return name[kind];
}

struct Ake_TypeDef {
    Ake_TypeKind kind;
    Zinc_string name;
    union {
        struct { uint8_t bit_count; } integer;
        struct { uint8_t bit_count; } natural;
        struct { uint8_t bit_count; } real;
        struct { Ake_TypeField* head; Ake_TypeField* tail; void* backend_type; } fields;
        struct { size_t dim; Ake_TypeDef* td; } array;
        struct { size_t dim; Ake_TypeDef* td; } array_const;
        struct { Ake_TypeDef* td; } slice;
        struct { Ake_TypeDef* td; } pointer;
        struct {
            Ake_TypeParam* input_head;
            Ake_TypeParam* input_tail;
            Ake_TypeDef* output;
        } function;
    } data;
    Ake_type_context context;
    void* lhs_allocation;
};

typedef enum Ake_TypeParamKind {
    AKE_TYPE_PARAM_REGULAR,
    AKE_TYPE_PARAM_SELF,
    AKE_TYPE_PARAM_ELLIPSIS,
} Ake_TypeParamKind;

struct Ake_TypeParam {
    Ake_TypeParamKind kind;
    Zinc_string name;
    Ake_TypeDef* td;
    Ake_TypeParam* next;
    Ake_TypeParam* prev;
};

struct Ake_TypeField {
    Zinc_string name;
    Ake_TypeDef* td;
    Ake_TypeField* next;
    Ake_TypeField* prev;
};

void Ake_TypeDefInit(Ake_TypeDef* td);
void Ake_TypeDefCreate(Ake_TypeDef** td);
void Ake_TypeDefSet(Ake_TypeDef* td, Ake_TypeKind kind);
void Ake_TypeDefDestroy(Ake_TypeDef* td);
void Ake_TypeDefStructAdd(Ake_TypeDef* td, Ake_TypeField* tf);
void Ake_TypeDefInputAdd(Ake_TypeDef* td, Ake_TypeParam* tp);
bool Ake_TypeDefMatch(Ake_TypeDef* a, Ake_TypeDef* b, bool* cast);
Ake_TypeDef* Ake_TypeDefClone(Ake_TypeDef* td);
void Ake_TypeDefCopy(Ake_TypeDef* a, Ake_TypeDef* b);

void Ake_TypeParamInit(Ake_TypeParam* tp);
void Ake_TypeParamCreate(Ake_TypeParam** tp);
void Ake_TypeParamDestroy(Ake_TypeParam* tp);

void Ake_TypeFieldInit(Ake_TypeField* tf);
void Ake_TypeFieldCreate(Ake_TypeField** tf);
void Ake_TypeFieldDestroy(Ake_TypeField* tf);

#endif