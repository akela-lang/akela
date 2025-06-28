#include "type.h"
#include "zinc/memory.h"
#include <assert.h>

void Ake_TypeDefInit(Ake_TypeDef* td)
{
    td->kind = AKE_TYPE_NONE;
    Zinc_string_init(&td->name);
    td->lhs_allocation = NULL;
    td->context = Ake_type_context_value;
}

void Ake_TypeDefCreate(Ake_TypeDef** td)
{
    Zinc_malloc_safe((void**)td, sizeof(Ake_TypeDef));
    Ake_TypeDefInit(*td);
}

void Ake_TypeDefSet(Ake_TypeDef* td, Ake_TypeKind kind)
{
    td->kind = kind;
    switch (kind) {
        case AKE_TYPE_INTEGER:
            td->data.integer.bit_count = 0;
            break;
        case AKE_TYPE_NATURAL:
            td->data.natural.bit_count = 0;
            break;
        case AKE_TYPE_REAL:
            td->data.real.bit_count = 0;
            break;
        case AKE_TYPE_BOOLEAN:
            break;
        case AKE_TYPE_STRUCT:
            td->data.fields.head = NULL;
            td->data.fields.tail = NULL;
            break;
        case AKE_TYPE_ARRAY:
            td->data.array.dim = 0;
            td->data.array.td = NULL;
            break;
        case AKE_TYPE_ARRAY_CONST:
            td->data.array.dim = 0;
            td->data.array.td = NULL;
            break;
        case AKE_TYPE_SLICE:
            td->data.slice.td = NULL;
            break;
        case AKE_TYPE_POINTER:
            td->data.pointer.td = NULL;
            break;
        case AKE_TYPE_FUNCTION:
            td->data.function.input_head = NULL;
            td->data.function.input_tail = NULL;
            td->data.function.output = NULL;
            break;
        case AKE_TYPE_NONE:
        default:
            assert(false && "invalid kind");
    }
}

void Ake_TypeDefDestroy(Ake_TypeDef* td)
{
    if (td) {
        Zinc_string_destroy(&td->name);
        switch (td->kind) {
            case AKE_TYPE_NONE:
            case AKE_TYPE_INTEGER:
            case AKE_TYPE_NATURAL:
            case AKE_TYPE_REAL:
            case AKE_TYPE_BOOLEAN:
                break;
            case AKE_TYPE_STRUCT:
                Ake_TypeField* tf = td->data.fields.head;
                while (tf) {
                    Ake_TypeField* temp = tf;
                    tf = tf->next;
                    Ake_TypeFieldDestroy(temp);
                    free(temp);
                }
                break;
            case AKE_TYPE_ARRAY:
                Ake_TypeDefDestroy(td->data.array.td);
                break;
            case AKE_TYPE_ARRAY_CONST:
                Ake_TypeDefDestroy(td->data.array_const.td);
                break;
            case AKE_TYPE_SLICE:
                Ake_TypeDefDestroy(td->data.slice.td);
                break;
            case AKE_TYPE_POINTER:
                Ake_TypeDefDestroy(td->data.pointer.td);
                break;
            case AKE_TYPE_FUNCTION:
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

bool Ake_TypeIsNumeric(Ake_TypeKind kind)
{
    if (kind == AKE_TYPE_INTEGER) {
        return true;
    }

    if (kind == AKE_TYPE_NATURAL) {
        return true;
    }

    if (kind == AKE_TYPE_REAL) {
        return true;
    }

    return false;
}

bool Ake_TypeDefMatch(Ake_TypeDef* a, Ake_TypeDef* b, bool* cast)
{
    if (a->kind != b->kind) {
        if (cast && a->kind == AKE_TYPE_ARRAY_CONST && b->kind == AKE_TYPE_ARRAY) {
            if (!*cast) {
                return true;
            }
        }

        if (cast && Ake_TypeIsNumeric(a->kind) && Ake_TypeIsNumeric(b->kind)) {
            if (!*cast) {
                *cast = true;
            }
            return true;
        }

        return false;
    }

    switch (a->kind) {
        case AKE_TYPE_INTEGER:
            if (cast && a->data.integer.bit_count != b->data.integer.bit_count) {
                *cast = true;
                return true;
            }
            if (a->data.integer.bit_count != b->data.integer.bit_count) {
                return false;
            }
            break;
        case AKE_TYPE_NATURAL:
            if (cast && a->data.natural.bit_count != b->data.natural.bit_count) {
                *cast = true;
                return true;
            }
            if (a->data.natural.bit_count != b->data.natural.bit_count) {
                return false;
            }
            break;
        case AKE_TYPE_REAL:
            if (cast && a->data.real.bit_count != b->data.real.bit_count) {
                *cast = true;
                return true;
            }
            if (a->data.real.bit_count != b->data.real.bit_count) {
                return false;
            }
            break;
        case AKE_TYPE_BOOLEAN:
            break;
        case AKE_TYPE_STRUCT:
            Ake_TypeField* a_tf = a->data.fields.head;
            Ake_TypeField* b_tf = b->data.fields.head;
            while (a_tf || b_tf) {
                if (!a_tf || !b_tf) {
                    return false;
                }
                if (!Ake_TypeDefMatch(a_tf->td, b_tf->td, NULL)) {
                    return false;
                }
                a_tf = a_tf->next;
                b_tf = b_tf->next;
            }
            break;
        case AKE_TYPE_ARRAY:
            if (a->data.array.dim != b->data.array.dim) {
                return false;
            }
            return Ake_TypeDefMatch(a->data.array.td, b->data.array.td, cast);
        case AKE_TYPE_ARRAY_CONST:
            if (a->data.array_const.dim < b->data.array_const.dim) {
                return false;
            }
            return Ake_TypeDefMatch(a->data.array_const.td, b->data.array_const.td, cast);
        case AKE_TYPE_SLICE:
            return Ake_TypeDefMatch(a->data.slice.td, b->data.slice.td, NULL);
        case AKE_TYPE_POINTER:
            return Ake_TypeDefMatch(a->data.pointer.td, b->data.pointer.td, cast);
        case AKE_TYPE_FUNCTION:
            Ake_TypeParam* a_tp = a->data.function.input_head;
            Ake_TypeParam* b_tp = b->data.function.input_head;
            while (a_tp || b_tp) {
                if (!a_tp || !b_tp) {
                    return false;
                }
                if (!Ake_TypeDefMatch(a_tp->td, b_tp->td, NULL)) {
                    return false;
                }
                a_tp = a_tp->next;
                b_tp = b_tp->next;
            }

            if (a->data.function.output || b->data.function.output) {
                if (!a->data.function.output || !b->data.function.output) {
                    return false;
                }

                if (!Ake_TypeDefMatch(a->data.function.output, b->data.function.output, NULL)) {
                    return false;
                }
            }
            break;
        default:
            assert(false && "invalid kind");
    }

    return true;
}

Ake_TypeDef* Ake_TypeDefClone(Ake_TypeDef* td)
{
    Ake_TypeDef* new_td = NULL;
    if (td) {
        Ake_TypeDefCreate(&new_td);
        Ake_TypeDefSet(new_td, td->kind);
        Zinc_string_add_string(&new_td->name, &td->name);
        switch (td->kind) {
            case AKE_TYPE_INTEGER:
            case AKE_TYPE_NATURAL:
            case AKE_TYPE_REAL:
                new_td->data = td->data;
                break;
            case AKE_TYPE_BOOLEAN:
                break;
            case AKE_TYPE_STRUCT:
                Ake_TypeField* tf = td->data.fields.head;
                while (tf) {
                    Ake_TypeField* new_tf = NULL;
                    Ake_TypeFieldCreate(&new_tf);
                    Zinc_string_add_string(&new_tf->name, &tf->name);
                    new_tf->td = Ake_TypeDefClone(tf->td);
                    Ake_TypeDefStructAdd(new_td, new_tf);
                    tf = tf->next;
                }
                break;
            case AKE_TYPE_ARRAY:
                new_td->data.array.dim = td->data.array.dim;
                new_td->data.array.td = Ake_TypeDefClone(td->data.array.td);
                break;
            case AKE_TYPE_ARRAY_CONST:
                new_td->data.array_const.dim = td->data.array_const.dim;
                new_td->data.array_const.td = Ake_TypeDefClone(td->data.array_const.td);
                break;
            case AKE_TYPE_SLICE:
                new_td->data.slice.td = Ake_TypeDefClone(td->data.slice.td);
                break;
            case AKE_TYPE_POINTER:
                new_td->data.pointer.td = Ake_TypeDefClone(td->data.pointer.td);
                break;
            case AKE_TYPE_FUNCTION:
                Ake_TypeParam* tp = td->data.function.input_head;
                while (tp) {
                    Ake_TypeParam* new_tp = NULL;
                    Ake_TypeParamCreate(&new_tp);
                    Zinc_string_add_string(&new_tp->name, &tp->name);
                    new_tp->td = Ake_TypeDefClone(tp->td);
                    Ake_TypeDefInputAdd(new_td, new_tp);
                    tp = tp->next;
                }
                new_td->data.function.output = Ake_TypeDefClone(td->data.function.output);
                break;
            default:
                assert(false && "invalid kind");
        }
    }

    return new_td;
}

void Ake_TypeDefCopy(Ake_TypeDef* a, Ake_TypeDef* b)
{
    Ake_TypeDefDestroy(b);
    Ake_TypeDefInit(b);
    Ake_TypeDefSet(b, a->kind);
    Zinc_string_add_string(&b->name, &a->name);

    switch (a->kind) {
        case AKE_TYPE_INTEGER:
        case AKE_TYPE_NATURAL:
        case AKE_TYPE_REAL:
            b->data = a->data;
            break;
        case AKE_TYPE_BOOLEAN:
            break;
        case AKE_TYPE_STRUCT:
            Ake_TypeField* tf = a->data.fields.head;
            while (tf) {
                Ake_TypeField* new_tf = NULL;
                Ake_TypeFieldCreate(&new_tf);
                Zinc_string_add_string(&new_tf->name, &tf->name);
                new_tf->td = Ake_TypeDefClone(tf->td);
                Ake_TypeDefStructAdd(b, new_tf);
                tf = tf->next;
            }
            break;
        case AKE_TYPE_ARRAY:
            b->data.array.dim = a->data.array.dim;
            b->data.array.td = Ake_TypeDefClone(a->data.array.td);
            break;
        case AKE_TYPE_ARRAY_CONST:
            b->data.array_const.dim = a->data.array_const.dim;
            b->data.array_const.td = Ake_TypeDefClone(a->data.array_const.td);
            break;
        case AKE_TYPE_SLICE:
            b->data.slice.td = Ake_TypeDefClone(a->data.slice.td);
            break;
        case AKE_TYPE_POINTER:
            b->data.pointer.td = Ake_TypeDefClone(a->data.pointer.td);
            break;
        case AKE_TYPE_FUNCTION:
            Ake_TypeParam* tp = a->data.function.input_head;
            while (tp) {
                Ake_TypeParam* new_tp = NULL;
                Ake_TypeParamCreate(&new_tp);
                Zinc_string_add_string(&new_tp->name, &tp->name);
                new_tp->td = Ake_TypeDefClone(tp->td);
                Ake_TypeDefInputAdd(b, new_tp);
                tp = tp->next;
            }
            b->data.function.output = Ake_TypeDefClone(a->data.function.output);
            break;
        default:
            assert(false && "invalid kind");
    }
}

void Ake_TypeDefStructAdd(Ake_TypeDef* td, Ake_TypeField* tf)
{
    assert(td->kind == AKE_TYPE_STRUCT);

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
    assert(td->kind == AKE_TYPE_FUNCTION);

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
    tp->kind = AKE_TYPE_PARAM_REGULAR;
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