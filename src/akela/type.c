#include "type.h"
#include "zinc/memory.h"
#include <assert.h>

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
        case AKE_TYPE_DEF_OLD:
            td->data.old = NULL;
            break;
        case AKE_TYPE_DEF_ARRAY:
            td->data.array.dim = 0;
            td->data.array.td = NULL;
            break;
        case AKE_TYPE_DEF_ARRAY_CONST:
            td->data.array.dim = 0;
            td->data.array.td = NULL;
            break;
        case AKE_TYPE_DEF_SLICE:
            td->data.slice.td = NULL;
            break;
        case AKE_TYPE_DEF_POINTER:
            td->data.pointer.td = NULL;
            break;
        case AKE_TYPE_DEF_FUNCTION:
            Zinc_string_init(&td->data.function.name);
            td->data.function.input_head = NULL;
            td->data.function.input_tail = NULL;
            td->data.function.output = NULL;
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
            case AKE_TYPE_DEF_OLD:
                Ake_type_use_destroy(td->data.old);
                break;
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
            case AKE_TYPE_DEF_ARRAY:
                Ake_TypeDefDestroy(td->data.array.td);
                break;
            case AKE_TYPE_DEF_ARRAY_CONST:
                Ake_TypeDefDestroy(td->data.array_const.td);
                break;
            case AKE_TYPE_DEF_SLICE:
                Ake_TypeDefDestroy(td->data.slice.td);
                break;
            case AKE_TYPE_DEF_POINTER:
                Ake_TypeDefDestroy(td->data.pointer.td);
                break;
            case AKE_TYPE_DEF_FUNCTION:
                Zinc_string_destroy(&td->data.function.name);

                Ake_TypeParam* tp = td->data.function.input_head;
                while (tp) {
                    Ake_TypeParam* temp = tp;
                    tp = tp->next;
                    Ake_TypeParamDestroy(temp);
                    free(temp);
                }

                Ake_TypeDefDestroy(td->data.function.output);
                break;
            default:
                assert(false && "invalid kind");
        }
    }
}

bool Ake_TypeDefMatch(Ake_TypeDef* a, Ake_TypeDef* b)
{
    if (a->kind != b->kind) {
        return false;
    }

    switch (a->kind) {
        case AKE_TYPE_DEF_INTEGER:
            if (a->data.integer.bit_count != b->data.integer.bit_count) {
                return false;
            }
            break;
        case AKE_TYPE_DEF_NATURAL:
            if (a->data.natural.bit_count != b->data.natural.bit_count) {
                return false;
            }
            break;
        case AKE_TYPE_DEF_REAL:
            if (a->data.real.bit_count != b->data.real.bit_count) {
                return false;
            }
            break;
        case AKE_TYPE_DEF_BOOLEAN:
            break;
        case AKE_TYPE_DEF_STRUCT:
            Ake_TypeField* a_tf = a->data.fields.head;
            Ake_TypeField* b_tf = b->data.fields.head;
            while (a_tf || b_tf) {
                if (!a_tf || !b_tf) {
                    return false;
                }
                if (!Ake_TypeDefMatch(a_tf->td, b_tf->td)) {
                    return false;
                }
                a_tf = a_tf->next;
                b_tf = b_tf->next;
            }
            break;
        case AKE_TYPE_DEF_ARRAY:
            if (a->data.array.dim != b->data.array.dim) {
                return false;
            }
            return Ake_TypeDefMatch(a->data.array.td, b->data.array.td);
        case AKE_TYPE_DEF_ARRAY_CONST:
            if (a->data.array_const.dim != b->data.array_const.dim) {
                return false;
            }
            return Ake_TypeDefMatch(a->data.array_const.td, b->data.array_const.td);
        case AKE_TYPE_DEF_SLICE:
            return Ake_TypeDefMatch(a->data.slice.td, b->data.slice.td);
        case AKE_TYPE_DEF_POINTER:
            return Ake_TypeDefMatch(a->data.pointer.td, b->data.pointer.td);
        case AKE_TYPE_DEF_FUNCTION:
            Ake_TypeParam* a_tp = a->data.function.input_head;
            Ake_TypeParam* b_tp = b->data.function.input_head;
            while (a_tp || b_tp) {
                if (!a_tp || !b_tp) {
                    return false;
                }
                if (!Ake_TypeDefMatch(a_tp->td, b_tp->td)) {
                    return false;
                }
                a_tp = a_tp->next;
                b_tp = b_tp->next;
            }

            if (a->data.function.output || b->data.function.output) {
                if (!a->data.function.output || !b->data.function.output) {
                    return false;
                }

                if (!Ake_TypeDefMatch(a->data.function.output, b->data.function.output)) {
                    return false;
                }
            }
            break;
        default:
            assert(false && "invalid kind");
    }

    return true;
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

void Ake_TypeDefInputAdd(Ake_TypeDef* td, Ake_TypeParam* tp)
{
    assert(td->kind == AKE_TYPE_DEF_FUNCTION);

    if (td->data.function.input_head && td->data.function.input_tail) {
        td->data.function.input_tail->next = tp;
        tp->prev = td->data.function.input_tail;
        td->data.function.input_tail = tp;
    } else {
        td->data.function.input_head = tp;
        td->data.function.input_tail = tp;
    }
}

void Ake_TypeParamInit(Ake_TypeParam* tp)
{
    Zinc_string_init(&tp->name);
    tp->td = NULL;
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
    Ake_TypeDefDestroy(tp->td);
}

void Ake_TypeFieldInit(Ake_TypeField* tf)
{
    Zinc_string_init(&tf->name);
    tf->td = NULL;
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
    Ake_TypeDefDestroy(tf->td);
}