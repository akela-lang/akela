#ifndef ZINC_LIST_H
#define ZINC_LIST_H

#include "api.h"
#include <stdbool.h>

struct list_node {
    void* item;
    struct list_node* next;
    struct list_node* prev;
};

struct list {
    struct list_node* head;
    struct list_node* tail;
};

typedef void (*list_node_destroy)(void* item);

#ifdef __cplusplus
extern "C" {
#endif

ZINC_API void list_create(struct list** l);
ZINC_API void list_node_init(struct list_node* ln);
ZINC_API void list_init(struct list* l);
ZINC_API void list_add(struct list* l, struct list_node* ln);
ZINC_API void list_add_item(struct list* l, void* item);
ZINC_API bool list_has_item(struct list* l, void* item);
ZINC_API void* list_remove_first_item(struct list* l);
ZINC_API void list_destroy(struct list* l, list_node_destroy lnd);
ZINC_API void* list_get(struct list* l, int index);

#ifdef __cplusplus
}
#endif

#endif