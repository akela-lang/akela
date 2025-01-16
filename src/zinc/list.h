#ifndef ZINC_LIST_H
#define ZINC_LIST_H

#include "api.h"
#include <stdbool.h>

typedef struct Zinc_list_node {
    void* item;
    struct Zinc_list_node* next;
    struct Zinc_list_node* prev;
} Zinc_list_node;

typedef struct Zinc_list {
    Zinc_list_node* head;
    Zinc_list_node* tail;
} Zinc_list;

typedef void (*Zinc_list_node_destroy)(void* item);

#ifdef __cplusplus
extern "C" {
#endif

ZINC_API void Zinc_list_create(Zinc_list** l);
ZINC_API void Zinc_list_node_init(Zinc_list_node* ln);
ZINC_API void Zinc_list_init(Zinc_list* l);
ZINC_API void Zinc_list_add(Zinc_list* l, Zinc_list_node* ln);
ZINC_API void Zinc_list_add_item(Zinc_list* l, void* item);
ZINC_API bool Zinc_list_has_item(Zinc_list* l, void* item);
ZINC_API void* list_remove_first_item(Zinc_list* l);
ZINC_API void Zinc_list_destroy(Zinc_list* l, Zinc_list_node_destroy lnd);
ZINC_API void* Zinc_list_get(Zinc_list* l, int index);

#ifdef __cplusplus
}
#endif

#endif