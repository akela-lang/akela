#include "section.h"
#include "zinc/memory.h"

void Lava_item_init(Lava_item* item)
{
    item->kind = Lava_item_kind_none;
    Zinc_string_init(&item->text);
    item->next = NULL;
    item->prev = NULL;
}

void Lava_item_create(Lava_item** item)
{
    Zinc_malloc_safe((void**)item, sizeof(Lava_item));
    Lava_item_init(*item);
}

void Lava_item_destroy(Lava_item* item)
{
    Zinc_string_destroy(&item->text);
}

void Lava_item_list_init(Lava_item_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Lava_item_list_create(Lava_item_list** list)
{
    Zinc_malloc_safe((void**)list, sizeof(Lava_item_list));
    Lava_item_list_init(*list);
}

void Lava_item_list_add(Lava_item_list* list, Lava_item* item)
{
    if (list->head && list->tail) {
        list->tail->next = item;
        item->prev = list->tail;
        list->tail = item;
    } else {
        list->head = item;
        list->tail = item;
    }
}

void Lava_item_list_destroy(Lava_item_list* list)
{
    Lava_item* p = list->head;
    while (p) {
        Lava_item* temp = p;
        p = p->next;
        Lava_item_destroy(temp);
        free(temp);
    }
}