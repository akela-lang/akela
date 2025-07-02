#ifndef AKELA_TYPE_TO_CENT_H
#define AKELA_TYPE_TO_CENT_H

#include "type_list.h"
#include "zinc/vector.h"

typedef struct Ake_TypeSlots Ake_TypeSlots;

struct Ake_TypeSlots {
    Ake_TypeList list;
    Zinc_vector index_list;     // vector of size_t
};

void Ake_TypeSlotsInit(Ake_TypeSlots* slots);
void Ake_TypeSlotsDestroy(Ake_TypeSlots* slots);
void Ake_TypeSlotsProcess(Ake_TypeSlots* slots, Ake_Type* type);
size_t Ake_TypeSlotsCount(Ake_TypeSlots* slots);
Ake_Type* Ake_TypeSlotsGet(Ake_TypeSlots* slots, size_t slot);
size_t Ake_TypeSlotsPosToSlot(Ake_TypeSlots* slots, size_t pos);

#endif