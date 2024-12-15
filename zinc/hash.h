#ifndef _HASH_H
#define _HASH_H

#include "api.h"
#include "buffer.h"

struct hash_entry
{
    void* item;
    struct buffer value;
    struct hash_entry* next;
    struct hash_entry* prev;
};

struct hash_list {
    struct hash_entry* head;
    struct hash_entry* tail;
};

struct hash_table {
    unsigned int size;
    struct hash_list* buckets;
};

typedef void (*hash_table_func)(void*);

ZINC_API unsigned int hash_calc_buffer(struct buffer* value, unsigned int size);
ZINC_API unsigned int hash_calc_str(char* str, unsigned int size);
ZINC_API void hash_entry_init(struct hash_entry* ent);
ZINC_API void hash_entry_destroy(struct hash_entry* ent);
ZINC_API void hash_list_init(struct hash_list* hl);
ZINC_API void hash_list_destroy(struct hash_list* hl);
ZINC_API void hash_table_map(struct hash_table* ht, hash_table_func f);
ZINC_API void hash_table_init(struct hash_table* ht, unsigned int size);
ZINC_API void hash_table_destroy(struct hash_table* ht);
ZINC_API void hash_table_add(struct hash_table* ht, struct buffer* value, void* item);
ZINC_API void hash_table_add_str(struct hash_table* ht, char* str, void* item);
ZINC_API void* hash_table_get(struct hash_table* ht, struct buffer* value);
ZINC_API void* hash_table_get_str(struct hash_table* ht, char* str);

#endif
