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

typedef void (hash_table_func)(void*);

/* dynamic-output-none */
ZINC_API unsigned int hash_calc(struct buffer* value, unsigned int size);

/* dynamic-output-none */
ZINC_API void hash_entry_init(struct hash_entry* ent);

/* dynamic-destroy ent{} */
ZINC_API void hash_entry_destroy(struct hash_entry* ent);

/* dynamic-output-none */
ZINC_API void hash_list_init(struct hash_list* hl);

/* dynamic-destroy hl{} */
ZINC_API void hash_list_destroy(struct hash_list* hl);

/* dynamic-destroy ht{} */
ZINC_API void hash_table_map(struct hash_table* ht, hash_table_func f);

/* dynamic-output ht{} */
ZINC_API void hash_table_init(struct hash_table* ht, unsigned int size);

/* dynamic-destroy ht{} */
ZINC_API void hash_table_destroy(struct hash_table* ht);

/* dynamic-output ht{} */
ZINC_API void hash_table_add(struct hash_table* ht, struct buffer* value, void* item);

/* dynamic-output-none */
ZINC_API void* hash_table_get(struct hash_table* ht, struct buffer* value);

#endif
