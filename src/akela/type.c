#include "type.h"
#include "zinc/memory.h"
#include <assert.h>

void Ake_TypeUseInit(Ake_TypeUse* tu)
{
    tu->kind = AKE_TYPE_USE_NONE;
}

void Ake_TypeUseCreate(Ake_TypeUse** tu)
{
    Zinc_malloc_safe((void**)tu, sizeof(Ake_TypeUse));
    Ake_TypeUseInit(*tu);
}

void Ake_TypeUseSet(Ake_TypeUse* tu, Ake_TypeUseKind kind)
{
    tu->kind = kind;
    switch (kind) {
        case AKE_TYPE_USE_OLD:
            tu->data.old = NULL;
            break;
        case AKE_TYPE_USE_SCALAR:
            tu->data.scalar.td = NULL;
            break;
        case AKE_TYPE_USE_ARRAY:
            tu->data.array.dim = 0;
            tu->data.array.tu = NULL;
            break;
        case AKE_TYPE_USE_ARRAY_CONST:
            tu->data.array.dim = 0;
            tu->data.array.tu = NULL;
            break;
        case AKE_TYPE_USE_SLICE:
            tu->data.slice.tu = NULL;
            break;
        case AKE_TYPE_USE_POINTER:
            tu->data.pointer.tu = NULL;
            break;
        case AKE_TYPE_USE_FUNCTION:
            Zinc_string_init(&tu->data.function.name);
            tu->data.function.input_head = NULL;
            tu->data.function.input_tail = NULL;
            tu->data.function.output = NULL;
            break;
        case AKE_TYPE_USE_NONE:
        default:
            assert(false && "invalid kind");
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_TypeUseDestroy(Ake_TypeUse* tu)
{
    if (tu) {
        switch (tu->kind) {
            case AKE_TYPE_USE_OLD:
                Ake_type_use_destroy(tu->data.old);
                break;
            case AKE_TYPE_USE_NONE:
            case AKE_TYPE_USE_SCALAR:
                break;
            case AKE_TYPE_USE_ARRAY:
                Ake_TypeUseDestroy(tu->data.array.tu);
                break;
            case AKE_TYPE_USE_ARRAY_CONST:
                Ake_TypeUseDestroy(tu->data.array_const.tu);
                break;
            case AKE_TYPE_USE_SLICE:
                Ake_TypeUseDestroy(tu->data.slice.tu);
                break;
            case AKE_TYPE_USE_POINTER:
                Ake_TypeUseDestroy(tu->data.pointer.tu);
                break;
            case AKE_TYPE_USE_FUNCTION:
                Zinc_string_destroy(&tu->data.function.name);

                Ake_TypeParam* tp = tu->data.function.input_head;
                while (tp) {
                    Ake_TypeParam* temp = tp;
                    tp = tp->next;
                    Ake_TypeParamDestroy(temp);
                    free(temp);
                }

                Ake_TypeUseDestroy(tu->data.function.output);
                break;
            default:
                assert(false && "invalid kind");
        }
    }
}

void Ake_TypeDefInit(Ake_TypeDef* td)
{
    td->kind = AKE_TYPE_DEF_NONE;
    Zinc_string_init(&td->name);
}

void Ake_TypeDefCreate(Ake_TypeDef** td)
{
    Zinc_malloc_safe((void**)td, sizeof(Ake_TypeDef));
    Ake_TypeDefInit(*td);
}

void Ake_TypeDefSet(Ake_TypeDef* td, Ake_TypeDefKind kind)
{
    td->kind = kind;
    switch (kind) {
        case AKE_TYPE_DEF_INTEGER:
            td->data.integer.bit_count = 0;
            break;
        case AKE_TYPE_DEF_NATURAL:
            td->data.natural.bit_count = 0;
            break;
        case AKE_TYPE_DEF_REAL:
            td->data.real.bit_count = 0;
            break;
        case AKE_TYPE_DEF_BOOLEAN:
            break;
        case AKE_TYPE_DEF_STRUCT:
            td->data.fields.head = NULL;
            td->data.fields.tail = NULL;
            break;
        case AKE_TYPE_DEF_NONE:
        default:
            assert(false && "invalid kind");
    }
}

void Ake_TypeDefDestroy(Ake_TypeDef* td)
{
    if (td) {
        Zinc_string_destroy(&td->name);
        switch (td->kind) {
            case AKE_TYPE_DEF_NONE:
            case AKE_TYPE_DEF_INTEGER:
            case AKE_TYPE_DEF_NATURAL:
            case AKE_TYPE_DEF_REAL:
            case AKE_TYPE_DEF_BOOLEAN:
                break;
            case AKE_TYPE_DEF_STRUCT:
                Ake_TypeField* tf = td->data.fields.head;
                while (tf) {
                    Ake_TypeField* temp = tf;
                    tf = tf->next;
                    Ake_TypeFieldDestroy(temp);
                    free(temp);
                }
                break;
            default:
                assert(false && "invalid kind");
        }
    }
}

void Ake_TypeDefStructAdd(Ake_TypeDef* td, Ake_TypeField* tf)
{
    assert(td->kind == AKE_TYPE_DEF_STRUCT);

    if (td->data.fields.head && td->data.fields.tail) {
        tf->prev = td->data.fields.tail;
        td->data.fields.tail->next = tf;
        td->data.fields.tail = tf;
    } else {
        td->data.fields.head = tf;
        td->data.fields.tail = tf;
    }
}

void Ake_TypeParamInit(Ake_TypeParam* tp)
{
    Zinc_string_init(&tp->name);
    tp->tu = NULL;
    tp->next = NULL;
    tp->prev = NULL;
}

void Ake_TypeParamCreate(Ake_TypeParam** tp)
{
    Zinc_malloc_safe((void**)tp, sizeof(Ake_TypeParam));
    Ake_TypeParamInit(*tp);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_TypeParamDestroy(Ake_TypeParam* tp)
{
    Zinc_string_destroy(&tp->name);
    Ake_TypeUseDestroy(tp->tu);
}

void Ake_TypeFieldInit(Ake_TypeField* tf)
{
    Zinc_string_init(&tf->name);
    tf->tu = NULL;
    tf->next = NULL;
    tf->prev = NULL;
}

void Ake_TypeFieldCreate(Ake_TypeField** tf)
{
    Zinc_malloc_safe((void**)tf, sizeof(Ake_TypeField));
    Ake_TypeFieldInit(*tf);
}

void Ake_TypeFieldDestroy(Ake_TypeField* tf)
{
    Zinc_string_destroy(&tf->name);
    Ake_TypeUseDestroy(tf->tu);
}