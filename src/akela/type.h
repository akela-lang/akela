#ifndef AKELA_TYPE_H
#define AKELA_TYPE_H

#include "forward.h"
#include <stddef.h>
#include "zinc/zstring.h"
#include <inttypes.h>
#include "type.h"
#include "type_use.h"

typedef enum Ake_TypeDefKind {
    AKE_TYPE_DEF_NONE,
    AKE_TYPE_DEF_OLD,
    AKE_TYPE_DEF_INTEGER,
    AKE_TYPE_DEF_NATURAL,
    AKE_TYPE_DEF_REAL,
    AKE_TYPE_DEF_BOOLEAN,
    AKE_TYPE_DEF_STRUCT,
    AKE_TYPE_DEF_ARRAY,
    AKE_TYPE_DEF_ARRAY_CONST,
    AKE_TYPE_DEF_SLICE,
    AKE_TYPE_DEF_POINTER,
    AKE_TYPE_DEF_FUNCTION,
} Ake_TypeDefKind;

struct Ake_TypeDef {
    Ake_TypeDefKind kind;
    Zinc_string name;
    union {
        Ake_type_use* old;
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
};

struct Ake_TypeParam {
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
void Ake_TypeDefSet(Ake_TypeDef* td, Ake_TypeDefKind kind);
void Ake_TypeDefDestroy(Ake_TypeDef* td);
void Ake_TypeDefStructAdd(Ake_TypeDef* td, Ake_TypeField* tf);
void Ake_TypeDefInputAdd(Ake_TypeDef* td, Ake_TypeParam* tp);
bool Ake_TypeDefMatch(Ake_TypeDef* a, Ake_TypeDef* b, bool* cast);

void Ake_TypeParamInit(Ake_TypeParam* tp);
void Ake_TypeParamCreate(Ake_TypeParam** tp);
void Ake_TypeParamDestroy(Ake_TypeParam* tp);

void Ake_TypeFieldInit(Ake_TypeField* tf);
void Ake_TypeFieldCreate(Ake_TypeField** tf);
void Ake_TypeFieldDestroy(Ake_TypeField* tf);

#endif