#ifndef _HASH_H
#define _HASH_H

#include "zinc/buffer.h"

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

unsigned int hash_calc(struct buffer* value, unsigned int size);
void hash_entry_init(struct hash_entry* ent);
void hash_entry_destroy(struct hash_entry* ent);
void hash_list_init(struct hash_list* hl);
void hash_list_destroy(struct hash_list* hl);
void hash_table_init(struct hash_table* ht, unsigned int size);
void hash_table_destroy(struct hash_table* ht);
void hash_table_add(struct hash_table* ht, struct buffer* value, void* item);
void* hash_table_get(struct hash_table* ht, struct buffer* value);

#endif;
