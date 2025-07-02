#include "type_slots.h"
#include "ast.h"

bool Ake_TypeMatchExact(Ake_Type* a, Ake_Type* b);

void Ake_TypeSlotsInit(Ake_TypeSlots* slots)
{
    Ake_TypeListInit(&slots->list);
    Zinc_vector_init(&slots->index_list, sizeof(size_t));
}

void Ake_TypeSlotsDestroy(Ake_TypeSlots* slots)
{
    Ake_TypeListDestroy(&slots->list);
    Zinc_vector_destroy(&slots->index_list);
}

void Ake_TypeSlotsProcess(Ake_TypeSlots* slots, Ake_Type* type)
{
    bool found = false;
    Ake_TypeNode* node = slots->list.head;
    size_t index = 0;
    while (node) {
        if (Ake_TypeMatchExact(node->type, type)) {
            found = true;
            break;
        }
        index++;
        node = node->next;
    }

    if (found) {
        Zinc_vector_add(&slots->index_list, &index, 1);
    } else {
        Ake_TypeListAddType(&slots->list, type);
        Zinc_vector_add(&slots->index_list, &index, 1);
    }
}

size_t Ake_TypeSlotsCount(Ake_TypeSlots* slots)
{
    size_t count = 0;
    Ake_TypeNode* node = slots->list.head;
    while (node) {
        count++;
        node = node->next;
    }

    return count;
}

Ake_Type* Ake_TypeSlotsGet(Ake_TypeSlots* slots, size_t slot)
{
    size_t count = 0;
    Ake_TypeNode* node = slots->list.head;
    while (node) {
        if (count == slot) {
            return node->type;
        }
        count++;
        node = node->next;
    }

    return NULL;
}

size_t Ake_TypeSlotsPosToSlot(Ake_TypeSlots* slots, size_t pos)
{
    assert(pos < slots->index_list.count);
    size_t* slot = (size_t*)ZINC_VECTOR_PTR(&slots->index_list, pos);
    return *slot;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Ake_TypeMatchExact(Ake_Type* a, Ake_Type* b)
{
    if (a->kind != b->kind) {
        return false;
    }

    if (!Zinc_string_compare(&a->name, &b->name)) {
        return false;
    }

    switch (a->kind) {
        case AKE_TYPE_INTEGER:
            if (a->data.integer.bit_count != b->data.integer.bit_count) {
                return false;
            }
            break;
        case AKE_TYPE_NATURAL:
            if (a->data.natural.bit_count != b->data.natural.bit_count) {
                return false;
            }
            break;
        case AKE_TYPE_REAL:
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
                if (!Zinc_string_compare(&a_tf->name, &b_tf->name)) {
                    return false;
                }
                if (!Ake_TypeMatchExact(a_tf->type, b_tf->type)) {
                    return false;
                }
                a_tf = a_tf->next;
                b_tf = b_tf->next;
            }
            break;
        case AKE_TYPE_ARRAY:
            if (a->data.array.is_const != b->data.array.is_const) {
                return false;
            }
            if (a->data.array.dim != b->data.array.dim) {
                return false;
            }
            return Ake_TypeMatchExact(a->data.array.type, b->data.array.type);
        case AKE_TYPE_SLICE:
            return Ake_TypeMatchExact(a->data.slice.type, b->data.slice.type);
        case AKE_TYPE_POINTER:
            return Ake_TypeMatchExact(a->data.pointer.type, b->data.pointer.type);
        case AKE_TYPE_FUNCTION:
            Ake_TypeParam* a_tp = a->data.function.input_head;
            Ake_TypeParam* b_tp = b->data.function.input_head;
            while (a_tp || b_tp) {
                if (!a_tp || !b_tp) {
                    return false;
                }

                if (Zinc_string_compare(&a_tp->name, &b_tp->name)) {
                    return false;
                }

                if (!Ake_TypeMatchExact(a_tp->type, b_tp->type)) {
                    return false;
                }
                a_tp = a_tp->next;
                b_tp = b_tp->next;
            }

            if (a->data.function.output || b->data.function.output) {
                if (!a->data.function.output || !b->data.function.output) {
                    return false;
                }

                if (!Ake_TypeMatchExact(a->data.function.output, b->data.function.output)) {
                    return false;
                }
            }
            break;
        default:
            assert(false && "invalid kind");
    }

    return true;
}

void Ake_TypeSlotsScan(Ake_TypeSlots* slots, Ake_Ast* ast)
{
    if (ast->type) {
        Ake_TypeSlotsProcess(slots, ast->type);
    }

    Ake_Ast* p = ast->head;
    while (p) {
        Ake_TypeSlotsScan(slots, p);
        p = p->next;
    }
}