#include "hash_map_size_t.h"
#include "memory.h"

void Zinc_hash_map_size_t_entry_init(Zinc_hash_map_size_t_entry* entry)
{
    entry->value = 0;
    entry->item = NULL;
    entry->next = NULL;
    entry->prev = NULL;
}

void Zinc_hash_map_size_t_list_init(Zinc_hash_map_size_t_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Zinc_hash_map_size_t_list_destroy(Zinc_hash_map_size_t_list* list)
{
    Zinc_hash_map_size_t_entry* entry = list->head;
    while (entry) {
        Zinc_hash_map_size_t_entry* temp = entry;
        entry = entry->next;
        free(temp);
    }
}

void Zinc_hash_map_size_t_init(Zinc_hash_map_size_t* set, size_t bucket_count)
{
    set->bucket_count = bucket_count;
    Zinc_malloc_safe((void**)&set->buckets, sizeof(Zinc_hash_map_size_t_list) * bucket_count);

    for (int i = 0; i < bucket_count; i++) {
        Zinc_hash_map_size_t_list_init(&set->buckets[i]);
    }
}

void Zinc_hash_map_size_t_create(Zinc_hash_map_size_t** set, size_t bucket_count)
{
    Zinc_malloc_safe((void**)set, sizeof(Zinc_hash_map_size_t));
    Zinc_hash_map_size_t_init(*set, bucket_count);
}

void Zinc_hash_map_size_t_destroy(Zinc_hash_map_size_t* set)
{
    for (size_t i = 0; i < set->bucket_count; i++) {
        Zinc_hash_map_size_t_list_destroy(&set->buckets[i]);
    }
    free(set->buckets);
}

void Zinc_hash_map_size_t_map(Zinc_hash_map_size_t* set, Zinc_hash_map_size_t_func f)
{
    for (size_t i = 0; i < set->bucket_count; i++) {
        Zinc_hash_map_size_t_entry* entry = set->buckets[i].head;
        while (entry) {
            f(entry->value, entry->item);
            entry = entry->next;
        }
    }
}

void Zinc_hash_map_size_t_add(Zinc_hash_map_size_t* set, size_t value, void* item)
{
    size_t index = value % set->bucket_count;
    Zinc_hash_map_size_t_list* list = &set->buckets[index];
    Zinc_hash_map_size_t_entry* p = list->head;
    bool found = false;
    while (p) {
        Zinc_hash_map_size_t_entry* temp = p;
        p = p->next;
        if (temp->value == value) {
            found = true;
        }
    }

    if (!found) {
        Zinc_hash_map_size_t_entry* entry = NULL;
        Zinc_malloc_safe((void**)&entry, sizeof(Zinc_hash_map_size_t_entry));
        Zinc_hash_map_size_t_entry_init(entry);
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

void* Zinc_hash_map_size_t_get(Zinc_hash_map_size_t* set, size_t value)
{
    size_t index = value % set->bucket_count;
    Zinc_hash_map_size_t_list* list = &set->buckets[index];
    Zinc_hash_map_size_t_entry* entry = list->head;
    while (entry) {
        if (entry->value == value) {
            return entry->item;
        }
        entry = entry->next;
    }

    return NULL;
}

void* Zinc_hash_map_size_t_remove(Zinc_hash_map_size_t* set, size_t value)
{
    size_t index = value % set->bucket_count;
    Zinc_hash_map_size_t_list* list = &set->buckets[index];
    Zinc_hash_map_size_t_entry* entry = list->head;
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