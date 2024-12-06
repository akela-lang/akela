#include "hash_map_size_t.h"
#include "memory.h"

void Hash_map_size_t_entry_init(Hash_map_size_t_entry* entry)
{
    entry->value = 0;
    entry->item = NULL;
    entry->next = NULL;
    entry->prev = NULL;
}

void Hash_map_size_t_list_init(Hash_map_size_t_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Hash_map_size_t_list_destroy(Hash_map_size_t_list* list)
{
    Hash_map_size_t_entry* entry = list->head;
    while (entry) {
        Hash_map_size_t_entry* temp = entry;
        entry = entry->next;
        free(temp);
    }
}

void Hash_map_size_t_init(Hash_map_size_t* set, size_t bucket_count)
{
    set->bucket_count = bucket_count;
    malloc_safe((void**)&set->buckets, sizeof(Hash_map_size_t_list) * bucket_count);

    for (int i = 0; i < bucket_count; i++) {
        Hash_map_size_t_list_init(&set->buckets[i]);
    }
}

void Hash_map_size_t_destroy(Hash_map_size_t* set)
{
    for (size_t i = 0; i < set->bucket_count; i++) {
        Hash_map_size_t_list_destroy(&set->buckets[i]);
    }
    free(set->buckets);
}

void Hash_map_size_t_map(Hash_map_size_t* set, Hash_map_size_t_func f)
{
    for (size_t i = 0; i < set->bucket_count; i++) {
        Hash_map_size_t_entry* entry = set->buckets[i].head;
        while (entry) {
            f(entry->value, entry->item);
            entry = entry->next;
        }
    }
}

void Hash_map_size_t_add(Hash_map_size_t* set, size_t value, void* item)
{
    size_t index = value % set->bucket_count;
    Hash_map_size_t_list* list = &set->buckets[index];
    Hash_map_size_t_entry* p = list->head;
    bool found = false;
    while (p) {
        Hash_map_size_t_entry* temp = p;
        p = p->next;
        if (temp->value == value) {
            found = true;
        }
    }

    if (!found) {
        Hash_map_size_t_entry* entry = NULL;
        malloc_safe((void**)&entry, sizeof(Hash_map_size_t_entry));
        Hash_map_size_t_entry_init(entry);
        entry->value = value;
        entry->item = item;
        if (list->head && list->tail) {
            entry->prev = list->tail;
            list->tail->next = entry;
            list->tail = entry;
        } else {
            list->head = entry;
            list->tail = entry;
        }
    }
}

void* Hash_map_size_t_get(Hash_map_size_t* set, size_t value)
{
    size_t index = value % set->bucket_count;
    Hash_map_size_t_list* list = &set->buckets[index];
    Hash_map_size_t_entry* entry = list->head;
    while (entry) {
        if (entry->value == value) {
            return entry->item;
        }
        entry = entry->next;
    }

    return NULL;
}

void* Hash_map_size_t_remove(Hash_map_size_t* set, size_t value)
{
    size_t index = value % set->bucket_count;
    Hash_map_size_t_list* list = &set->buckets[index];
    Hash_map_size_t_entry* entry = list->head;
    while (entry) {
        if (entry->value == value) {
            if (list->head == entry) {
                list->head = entry->next;
            }
            if (list->tail == entry) {
                list->tail = entry->prev;
            }
            if (entry->prev) {
                entry->prev->next = entry->next;
            }
            if (entry->next) {
                entry->next->prev = entry->prev;
            }
            entry->prev = NULL;
            entry->next = NULL;
            void* item = entry->item;
            free(entry);
            return item;
        }
    }

    return NULL;
}