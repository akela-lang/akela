#ifndef AKELA_TYPE_H
#define AKELA_TYPE_H

#include "forward.h"
#include <stddef.h>
#include "zinc/zstring.h"
#include <inttypes.h>
#include "type_def.h"
#include "type_use.h"

typedef enum Ake_TypeUseKind {
    AKE_TYPE_USE_NONE,
    AKE_TYPE_USE_OLD,
    AKE_TYPE_USE_SCALAR,
    AKE_TYPE_USE_ARRAY,
    AKE_TYPE_USE_ARRAY_CONST,
    AKE_TYPE_USE_SLICE,
    AKE_TYPE_USE_POINTER,
    AKE_TYPE_USE_FUNCTION,
} Ake_TypeUseKind;

struct Ake_TypeUse {
    Ake_TypeUseKind kind;
    union {
        Ake_type_use* old;
        struct { Ake_TypeDef* td; } scalar;
        struct { size_t dim; Ake_TypeUse* tu; } array;
        struct { size_t dim; Ake_TypeUse* tu; } array_const;
        struct { Ake_TypeUse* tu; } slice;
        struct { Ake_TypeUse* tu; } pointer;
        struct {
            Zinc_string name;
            Ake_TypeParam* input_head;
            Ake_TypeParam* input_tail;
            Ake_TypeUse* output;
        } function;
    } data;
};

typedef enum Ake_TypeDefKind {
    AKE_TYPE_DEF_NONE,
    AKE_TYPE_DEF_OLD,
    AKE_TYPE_DEF_INTEGER,
    AKE_TYPE_DEF_NATURAL,
    AKE_TYPE_DEF_REAL,
    AKE_TYPE_DEF_STRUCT,
} Ake_TypeDefKind;

struct Ake_TypeDef {
    Ake_TypeDefKind kind;
    union {
        Ake_type_def* old;
        struct { uint8_t bit_count; } integer;
        struct { uint8_t bit_count; } natural;
        struct { uint8_t bit_count; } real;
        struct { Ake_TypeField* head; Ake_TypeField* tail; } fields;
    } data;
};

struct Ake_TypeParam {
    Zinc_string name;
    Ake_TypeUse* tu;
    Ake_TypeParam* next;
    Ake_TypeParam* prev;
};

struct Ake_TypeField {
    Zinc_string name;
    Ake_TypeUse* tu;
    Ake_TypeField* next;
    Ake_TypeField* prev;
};

void Ake_TypeUseInit(Ake_TypeUse* tu);
void Ake_TypeUseCreate(Ake_TypeUse** tu);
void Ake_TypeUseSet(Ake_TypeUse* tu, Ake_TypeUseKind kind);
void Ake_TypeUseDestroy(Ake_TypeUse* tu);

void Ake_TypeDefInit(Ake_TypeDef* td);
void Ake_TypeDefCreate(Ake_TypeDef** td);
void Ake_TypeDefSet(Ake_TypeDef* td, Ake_TypeDefKind kind);
void Ake_TypeDefDestroy(Ake_TypeDef* td);

void Ake_TypeParamInit(Ake_TypeParam* tp);
void Ake_TypeParamCreate(Ake_TypeParam** tp);
void Ake_TypeParamDestroy(Ake_TypeParam* tp);

void Ake_TypeFieldInit(Ake_TypeField* tf);
void Ake_TypeFieldCreate(Ake_TypeField** tf);
void Ake_TypeFieldDestroy(Ake_TypeField* tf);

#endif