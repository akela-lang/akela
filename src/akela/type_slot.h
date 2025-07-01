#ifndef AKELA_TYPE_TO_CENT_H
#define AKELA_TYPE_TO_CENT_H

#include "type_list.h"
#include "zinc/vector.h"

typedef struct Ake_Slot Ake_TypeSlot;

struct Ake_Slot {
    Ake_TypeList list;
    Zinc_vector index_list;     // vector of size_t
};

void Ake_TypeSlotInit(Ake_TypeSlot* slot);
void Ake_TypeSlotProcess(Ake_TypeSlot* slot, Ake_Type* type);

#endif