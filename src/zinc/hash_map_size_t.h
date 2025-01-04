#ifndef ZINC_HASH_MAP_SIZE_T_H
#define ZINC_HASH_MAP_SIZE_T_H

#include <stdlib.h>

typedef struct Hash_map_size_t_entry {
    size_t value;
    void* item;
    struct Hash_map_size_t_entry* next;
    struct Hash_map_size_t_entry* prev;
} Hash_map_size_t_entry;

typedef struct Hash_map_size_t_list {
    Hash_map_size_t_entry* head;
    Hash_map_size_t_entry* tail;
} Hash_map_size_t_list;

typedef struct Hash_map_size_t {
    size_t bucket_count;
    Hash_map_size_t_list* buckets;
} Hash_map_size_t;

typedef void (*Hash_map_size_t_func)(size_t value, void* item);

void Hash_map_size_t_entry_init(Hash_map_size_t_entry* entry);

void Hash_map_size_t_list_init(Hash_map_size_t_list* list);
void Hash_map_size_t_list_destroy(Hash_map_size_t_list* list);

void Hash_map_size_t_init(Hash_map_size_t* set, size_t bucket_count);
void Hash_map_size_t_create(Hash_map_size_t** set, size_t bucket_count);
void Hash_map_size_t_destroy(Hash_map_size_t* set);
void Hash_map_size_t_map(Hash_map_size_t* set, Hash_map_size_t_func f);
void Hash_map_size_t_add(Hash_map_size_t* set, size_t value, void* item);
void* Hash_map_size_t_get(Hash_map_size_t* set, size_t value);
void* Hash_map_size_t_remove(Hash_map_size_t* set, size_t value);

#endif
