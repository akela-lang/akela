#ifndef ZINC_HASH_MAP_SIZE_T_H
#define ZINC_HASH_MAP_SIZE_T_H

#include <stdlib.h>

typedef struct Zinc_hash_map_size_t_entry {
    size_t value;
    void* item;
    struct Zinc_hash_map_size_t_entry* next;
    struct Zinc_hash_map_size_t_entry* prev;
} Zinc_hash_map_size_t_entry;

typedef struct Zinc_hash_map_size_t_list {
    Zinc_hash_map_size_t_entry* head;
    Zinc_hash_map_size_t_entry* tail;
} Zinc_hash_map_size_t_list;

typedef struct Zinc_hash_map_size_t {
    size_t bucket_count;
    Zinc_hash_map_size_t_list* buckets;
} Zinc_hash_map_size_t;

typedef void (*Zinc_hash_map_size_t_func)(size_t value, void* item);

void Zinc_hash_map_size_t_entry_init(Zinc_hash_map_size_t_entry* entry);

void Zinc_hash_map_size_t_list_init(Zinc_hash_map_size_t_list* list);
void Zinc_hash_map_size_t_list_destroy(Zinc_hash_map_size_t_list* list);

void Zinc_hash_map_size_t_init(Zinc_hash_map_size_t* set, size_t bucket_count);
void Zinc_hash_map_size_t_create(Zinc_hash_map_size_t** set, size_t bucket_count);
void Zinc_hash_map_size_t_destroy(Zinc_hash_map_size_t* set);
void Zinc_hash_map_size_t_map(Zinc_hash_map_size_t* set, Zinc_hash_map_size_t_func f);
void Zinc_hash_map_size_t_add(Zinc_hash_map_size_t* set, size_t value, void* item);
void* Zinc_hash_map_size_t_get(Zinc_hash_map_size_t* set, size_t value);
void* Zinc_hash_map_size_t_remove(Zinc_hash_map_size_t* set, size_t value);

#endif
