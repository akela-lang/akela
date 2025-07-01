#ifndef AKELA_TYPE_LIST_H
#define AKELA_TYPE_LIST_H

#include "type.h"

typedef struct Ake_TypeNode Ake_TypeNode;
typedef struct Ake_TypeList Ake_TypeList;

struct Ake_TypeNode {
    Ake_Type* type;
    Ake_TypeNode* next;
    Ake_TypeNode* prev;
};

struct Ake_TypeList {
    Ake_TypeNode* head;
    Ake_TypeNode* tail;
};

void Ake_TypeNodeInit(Ake_TypeNode* node, Ake_Type* type);
void Ake_TypeNodeCreate(Ake_TypeNode** node, Ake_Type* type);
void Ake_TypeNodeDestroy(Ake_TypeNode* node);

void Ake_TypeListInit(Ake_TypeList* list);
void Ake_TypeListCreate(Ake_TypeList** list);
void Ake_TypeListAdd(Ake_TypeList* list, Ake_TypeNode* node);
void Ake_TypeListAddType(Ake_TypeList* list, Ake_Type* type);
void Ake_TypeListDestroy(Ake_TypeList* list);

#endif