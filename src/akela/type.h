#ifndef AKELA_TYPE_H
#define AKELA_TYPE_H

#include "forward.h"
#include <stddef.h>
#include "zinc/zstring.h"
#include <inttypes.h>
#include <assert.h>

typedef enum Ake_TypeKind {
    AKE_TYPE_NONE,
    AKE_TYPE_INTEGER,
    AKE_TYPE_NATURAL,
    AKE_TYPE_REAL,
    AKE_TYPE_BOOLEAN,
    AKE_TYPE_STRUCT,
    AKE_TYPE_ARRAY,
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
        "slice",
        "pointer",
        "function",
    };
    return name[kind];
}

struct Ake_Type {
    Ake_TypeKind kind;
    Zinc_string name;
    union {
        struct { uint8_t bit_count; } integer;
        struct { uint8_t bit_count; } natural;
        struct { uint8_t bit_count; } real;
        struct { Ake_TypeField* head; Ake_TypeField* tail; } fields;
        struct { bool is_const; size_t dim; Ake_Type* type; } array;
        struct { Ake_Type* type; } slice;
        struct { Ake_Type* type; } pointer;
        struct {
            Ake_TypeParam* input_head;
            Ake_TypeParam* input_tail;
            Ake_Type* output;
        } function;
    } data;
};

typedef enum Ake_TypeParamKind {
    AKE_TYPE_PARAM_REGULAR,
    AKE_TYPE_PARAM_SELF,
    AKE_TYPE_PARAM_ELLIPSIS,
} Ake_TypeParamKind;

struct Ake_TypeParam {
    Ake_TypeParamKind kind;
    Zinc_string name;
    Ake_Type* type;
    Ake_TypeParam* next;
    Ake_TypeParam* prev;
};

struct Ake_TypeField {
    Zinc_string name;
    Ake_Type* type;
    Ake_TypeField* next;
    Ake_TypeField* prev;
};

void Ake_TypeInit(Ake_Type* type);
void Ake_TypeCreate(Ake_Type** type);
void Ake_TypeSet(Ake_Type* type, Ake_TypeKind kind);
void Ake_TypeDestroy(Ake_Type* type);
void Ake_TypeStructAdd(Ake_Type* type, Ake_TypeField* tf);
void Ake_TypeInputAdd(Ake_Type* type, Ake_TypeParam* tp);
bool Ake_TypeMatch(Ake_Type* a, Ake_Type* b, bool* cast);
Ake_Type* Ake_TypeClone(Ake_Type* type);
void Ake_TypeCopy(Ake_Type* a, Ake_Type* b);

void Ake_TypeParamInit(Ake_TypeParam* tp);
void Ake_TypeParamCreate(Ake_TypeParam** tp);
void Ake_TypeParamDestroy(Ake_TypeParam* tp);

void Ake_TypeFieldInit(Ake_TypeField* tf);
void Ake_TypeFieldCreate(Ake_TypeField** tf);
void Ake_TypeFieldDestroy(Ake_TypeField* tf);

#endif