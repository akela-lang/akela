#include "type_list.h"
#include "zinc/memory.h"

void Ake_TypeNodeInit(Ake_TypeNode* node, Ake_Type* type)
{
    node->type = type;
    node->next = NULL;
    node->prev = NULL;
}

void Ake_TypeNodeCreate(Ake_TypeNode** node, Ake_Type* type)
{
    Zinc_malloc_safe((void**)node, sizeof(Ake_TypeNode));
    Ake_TypeNodeInit(*node, type);
}

void Ake_TypeNodeDestroy(Ake_TypeNode* node)
{
    Ake_TypeDestroy(node->type);
    free(node->type);
}

void Ake_TypeListInit(Ake_TypeList* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Ake_TypeListCreate(Ake_TypeList** list)
{
    Zinc_malloc_safe((void**)list, sizeof(Ake_TypeList));
    Ake_TypeListInit(*list);
}

void Ake_TypeListAdd(Ake_TypeList* list, Ake_TypeNode* node)
{
    if (list->head && list->tail) {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    } else {
        list->head = node;
        list->tail = node;
    }
}

void Ake_TypeListAddType(Ake_TypeList* list, Ake_Type* type)
{
    Ake_TypeNode* node = NULL;
    Ake_TypeNodeCreate(&node, type);
    Ake_TypeListAdd(list, node);
}

void Ake_TypeListDestroy(Ake_TypeList* list)
{
    Ake_TypeNode* node = list->head;
    while (node) {
        Ake_TypeNode* temp = node;
        node = node->next;
        //Ake_TypeNodeDestroy(temp);
        free(temp);
    }
}