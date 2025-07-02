#ifndef AKELA_TYPE_TO_CENT_H
#define AKELA_TYPE_TO_CENT_H

#include "type_list.h"
#include "zinc/vector.h"
#include "zinc/hash_map_size_t.h"

typedef struct Ake_TypeSlots Ake_TypeSlots;

struct Ake_TypeSlots {
    Ake_TypeList list;
    Zinc_vector index_list;
    Zinc_hash_map_size_t map;
};

#define AKE_TYPE_SLOT_MAP_SIZE 32

void Ake_TypeSlotsInit(Ake_TypeSlots* slots);
void Ake_TypeSlotsDestroy(Ake_TypeSlots* slots);
void Ake_TypeSlotsProcess(Ake_TypeSlots* slots, Ake_Type* type);
size_t Ake_TypeSlotsCount(Ake_TypeSlots* slots);
Ake_Type* Ake_TypeSlotsGetType(Ake_TypeSlots* slots, size_t slot);
size_t Ake_TypeSlotsGetSlot(Ake_TypeSlots* slots, Ake_Type* type);
void Ake_TypeSlotsScan(Ake_TypeSlots* slots, Ake_Ast* ast);

#endif