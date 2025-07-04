#include "type.h"
#include "zinc/memory.h"
#include <assert.h>

void Ake_TypeInit(Ake_Type* type)
{
    type->kind = AKE_TYPE_NONE;
    Zinc_string_init(&type->name);
}

void Ake_TypeCreate(Ake_Type** type)
{
    Zinc_malloc_safe((void**)type, sizeof(Ake_Type));
    Ake_TypeInit(*type);
}

void Ake_TypeSet(Ake_Type* type, Ake_TypeKind kind)
{
    type->kind = kind;
    switch (kind) {
        case AKE_TYPE_INTEGER:
            type->data.integer.bit_count = 0;
            break;
        case AKE_TYPE_NATURAL:
            type->data.natural.bit_count = 0;
            break;
        case AKE_TYPE_REAL:
            type->data.real.bit_count = 0;
            break;
        case AKE_TYPE_BOOLEAN:
            break;
        case AKE_TYPE_STRUCT:
            type->data.fields.head = NULL;
            type->data.fields.tail = NULL;
            break;
        case AKE_TYPE_ARRAY:
            type->data.array.is_const = false;
            type->data.array.dim = 0;
            type->data.array.type = NULL;
            break;
        case AKE_TYPE_SLICE:
            type->data.slice.type = NULL;
            break;
        case AKE_TYPE_POINTER:
            type->data.pointer.type = NULL;
            break;
        case AKE_TYPE_FUNCTION:
            type->data.function.input_head = NULL;
            type->data.function.input_tail = NULL;
            type->data.function.output = NULL;
            break;
        case AKE_TYPE_NONE:
        default:
            assert(false && "invalid kind");
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_TypeDestroy(Ake_Type* type)
{
    if (type) {
        Zinc_string_destroy(&type->name);
        switch (type->kind) {
            case AKE_TYPE_NONE:
            case AKE_TYPE_INTEGER:
            case AKE_TYPE_NATURAL:
            case AKE_TYPE_REAL:
            case AKE_TYPE_BOOLEAN:
                break;
            case AKE_TYPE_STRUCT:
                Ake_TypeField* tf = type->data.fields.head;
                while (tf) {
                    Ake_TypeField* temp = tf;
                    tf = tf->next;
                    Ake_TypeFieldDestroy(temp);
                    free(temp);
                }
                break;
            case AKE_TYPE_ARRAY:
                Ake_TypeDestroy(type->data.array.type);
                free(type->data.array.type);
                break;
            case AKE_TYPE_SLICE:
                Ake_TypeDestroy(type->data.slice.type);
                free(type->data.slice.type);
                break;
            case AKE_TYPE_POINTER:
                Ake_TypeDestroy(type->data.pointer.type);
                free(type->data.pointer.type);
                break;
            case AKE_TYPE_FUNCTION:
                Ake_TypeParam* tp = type->data.function.input_head;
                while (tp) {
                    Ake_TypeParam* temp = tp;
                    tp = tp->next;
                    Ake_TypeParamDestroy(temp);
                    free(temp);
                }

                Ake_TypeDestroy(type->data.function.output);
                free(type->data.function.output);
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

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Ake_TypeMatch(Ake_Type* a, Ake_Type* b, bool* cast)
{
    if (a->kind != b->kind) {
        if (cast && a->kind == AKE_TYPE_ARRAY) {
            if (a->data.array.is_const || !b->data.array.is_const) {
                if (!*cast) {
                    return true;
                }
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
                if (!Ake_TypeMatch(a_tf->type, b_tf->type, NULL)) {
                    return false;
                }
                a_tf = a_tf->next;
                b_tf = b_tf->next;
            }
            break;
        case AKE_TYPE_ARRAY:
            if (!a->data.array.is_const && b->data.array.is_const) {
                return false;
            }
            if (a->data.array.dim < b->data.array.dim) {
                return false;
            }
            return Ake_TypeMatch(a->data.array.type, b->data.array.type, cast);
        case AKE_TYPE_SLICE:
            return Ake_TypeMatch(a->data.slice.type, b->data.slice.type, NULL);
        case AKE_TYPE_POINTER:
            return Ake_TypeMatch(a->data.pointer.type, b->data.pointer.type, cast);
        case AKE_TYPE_FUNCTION:
            Ake_TypeParam* a_tp = a->data.function.input_head;
            Ake_TypeParam* b_tp = b->data.function.input_head;
            while (a_tp || b_tp) {
                if (!a_tp || !b_tp) {
                    return false;
                }
                if (!Ake_TypeMatch(a_tp->type, b_tp->type, NULL)) {
                    return false;
                }
                a_tp = a_tp->next;
                b_tp = b_tp->next;
            }

            if (a->data.function.output || b->data.function.output) {
                if (!a->data.function.output || !b->data.function.output) {
                    return false;
                }

                if (!Ake_TypeMatch(a->data.function.output, b->data.function.output, NULL)) {
                    return false;
                }
            }
            break;
        default:
            assert(false && "invalid kind");
    }

    return true;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Type* Ake_TypeClone(Ake_Type* type)
{
    Ake_Type* new_type = NULL;
    if (type) {
        Ake_TypeCreate(&new_type);
        Ake_TypeSet(new_type, type->kind);
        Zinc_string_add_string(&new_type->name, &type->name);
        switch (type->kind) {
            case AKE_TYPE_INTEGER:
            case AKE_TYPE_NATURAL:
            case AKE_TYPE_REAL:
                new_type->data = type->data;
                break;
            case AKE_TYPE_BOOLEAN:
                break;
            case AKE_TYPE_STRUCT:
                Ake_TypeField* tf = type->data.fields.head;
                while (tf) {
                    Ake_TypeField* new_tf = NULL;
                    Ake_TypeFieldCreate(&new_tf);
                    Zinc_string_add_string(&new_tf->name, &tf->name);
                    new_tf->type = Ake_TypeClone(tf->type);
                    Ake_TypeStructAdd(new_type, new_tf);
                    tf = tf->next;
                }
                break;
            case AKE_TYPE_ARRAY:
                new_type->data.array.is_const = type->data.array.is_const;
                new_type->data.array.dim = type->data.array.dim;
                new_type->data.array.type = Ake_TypeClone(type->data.array.type);
                break;
            case AKE_TYPE_SLICE:
                new_type->data.slice.type = Ake_TypeClone(type->data.slice.type);
                break;
            case AKE_TYPE_POINTER:
                new_type->data.pointer.type = Ake_TypeClone(type->data.pointer.type);
                break;
            case AKE_TYPE_FUNCTION:
                Ake_TypeParam* tp = type->data.function.input_head;
                while (tp) {
                    Ake_TypeParam* new_tp = NULL;
                    Ake_TypeParamCreate(&new_tp);
                    Zinc_string_add_string(&new_tp->name, &tp->name);
                    new_tp->type = Ake_TypeClone(tp->type);
                    Ake_TypeInputAdd(new_type, new_tp);
                    tp = tp->next;
                }
                new_type->data.function.output = Ake_TypeClone(type->data.function.output);
                break;
            default:
                assert(false && "invalid kind");
        }
    }

    return new_type;
}

void Ake_TypeCopy(Ake_Type* a, Ake_Type* b)
{
    Ake_TypeDestroy(b);
    Ake_TypeInit(b);
    Ake_TypeSet(b, a->kind);
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
                new_tf->type = Ake_TypeClone(tf->type);
                Ake_TypeStructAdd(b, new_tf);
                tf = tf->next;
            }
            break;
        case AKE_TYPE_ARRAY:
            b->data.array.is_const = a->data.array.is_const;
            b->data.array.dim = a->data.array.dim;
            b->data.array.type = Ake_TypeClone(a->data.array.type);
            break;
        case AKE_TYPE_SLICE:
            b->data.slice.type = Ake_TypeClone(a->data.slice.type);
            break;
        case AKE_TYPE_POINTER:
            b->data.pointer.type = Ake_TypeClone(a->data.pointer.type);
            break;
        case AKE_TYPE_FUNCTION:
            Ake_TypeParam* tp = a->data.function.input_head;
            while (tp) {
                Ake_TypeParam* new_tp = NULL;
                Ake_TypeParamCreate(&new_tp);
                Zinc_string_add_string(&new_tp->name, &tp->name);
                new_tp->type = Ake_TypeClone(tp->type);
                Ake_TypeInputAdd(b, new_tp);
                tp = tp->next;
            }
            b->data.function.output = Ake_TypeClone(a->data.function.output);
            break;
        default:
            assert(false && "invalid kind");
    }
}

void Ake_TypeStructAdd(Ake_Type* type, Ake_TypeField* tf)
{
    assert(type->kind == AKE_TYPE_STRUCT);

    if (type->data.fields.head && type->data.fields.tail) {
        tf->prev = type->data.fields.tail;
        type->data.fields.tail->next = tf;
        type->data.fields.tail = tf;
    } else {
        type->data.fields.head = tf;
        type->data.fields.tail = tf;
    }
}

void Ake_TypeInputAdd(Ake_Type* type, Ake_TypeParam* tp)
{
    assert(type->kind == AKE_TYPE_FUNCTION);

    if (type->data.function.input_head && type->data.function.input_tail) {
        type->data.function.input_tail->next = tp;
        tp->prev = type->data.function.input_tail;
        type->data.function.input_tail = tp;
    } else {
        type->data.function.input_head = tp;
        type->data.function.input_tail = tp;
    }
}

void Ake_TypeParamInit(Ake_TypeParam* tp)
{
    tp->kind = AKE_TYPE_PARAM_REGULAR;
    Zinc_string_init(&tp->name);
    tp->type = NULL;
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
    Ake_TypeDestroy(tp->type);
    free(tp->type);
}

void Ake_TypeFieldInit(Ake_TypeField* tf)
{
    Zinc_string_init(&tf->name);
    tf->type = NULL;
    tf->next = NULL;
    tf->prev = NULL;
}

void Ake_TypeFieldCreate(Ake_TypeField** tf)
{
    Zinc_malloc_safe((void**)tf, sizeof(Ake_TypeField));
    Ake_TypeFieldInit(*tf);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_TypeFieldDestroy(Ake_TypeField* tf)
{
    Zinc_string_destroy(&tf->name);
    Ake_TypeDestroy(tf->type);
    free(tf->type);
}