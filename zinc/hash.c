#include <stdlib.h>
#include "hash.h"
#include "buffer.h"
#include "memory.h"

/* dynamic-output-none */
unsigned int hash_calc(struct buffer* value, unsigned int size)
{
    unsigned int val = 0;

    for (int i = 0; i < value->size; i++) {
        val += (unsigned int)value->buf[i];
        val %= size;
    }

    return val;
}

/* dynamic-output-none */
void hash_entry_init(struct hash_entry* ent)
{
    ent->item = NULL;
    buffer_init(&ent->value);
    ent->next = NULL;
    ent->prev = NULL;
}

/* dynamic-destroy ent{} */
void hash_entry_destroy(struct hash_entry* ent)
{
    buffer_destroy(&ent->value);
}

/* dynamic-output-none */
void hash_list_init(struct hash_list* hl)
{
    hl->head = NULL;
    hl->tail = NULL;
}

/* dynamic-destroy hl{} */
void hash_list_destroy(struct hash_list* hl)
{
    struct hash_entry* p = hl->head;
    while (p) {
        struct hash_entry* temp = p;
        p = p->next;
        hash_entry_destroy(temp);
        free(temp);
    }
}

/* dynamic-output ht{} */
void hash_table_init(struct hash_table* ht, unsigned int size)
{
    ht->size = size;

    /* allocate ht{} */
    malloc_safe((void**)&ht->buckets, sizeof(struct hash_list) * size);

    for (int i = 0; i < size; i++) {
        hash_list_init(&ht->buckets[i]);
    }
}

/* dynamic-destroy ht{} */
void hash_table_destroy(struct hash_table* ht)
{
    for (int i = 0; i < ht->size; i++) {
        hash_list_destroy(&ht->buckets[i]);
    }
    free(ht->buckets);
}

/* dynamic-destroy ht{} */
void hash_table_map(struct hash_table* ht, hash_table_func f)
{
    for (int i = 0; i < ht->size; i++) {
        struct hash_entry* p = ht->buckets[i].head;
        while (p) {
            f(p->item);
            p = p->next;
        }
    }
}

/* assume entry is not in table so call hash_table_get before if not sure */
/* dynamic-output ht{} */
void hash_table_add(struct hash_table* ht, struct buffer* value, void* item)
{
    unsigned int val = hash_calc(value, ht->size);

    /* allocate ent */
    struct hash_entry* ent;
    malloc_safe((void**)&ent, sizeof(struct hash_entry));
    hash_entry_init(ent);
    ent->item = item;

    /* allocate ent{} */
    buffer_copy(value, &ent->value);

    /* add to beginning of bucket */
    /* allocate ent ent{} -> ht{} */
    struct hash_entry* next = ht->buckets[val].head;
    ent->next = next;
    if (next) {
        next->prev = ent;
    }
    ht->buckets[val].head = ent;
    if (!ht->buckets[val].tail) {
        ht->buckets[val].tail = ent;
    }
}

/* dynamic-output-none */
void* hash_table_get(struct hash_table* ht, struct buffer* value)
{
    struct hash_entry* ent;

    unsigned int val = hash_calc(value, ht->size);

    ent = ht->buckets[val].head;
    while (ent) {
        if (buffer_compare(&ent->value, value)) {
            return ent->item;
        }
        ent = ent->next;
    }

    return NULL;
}
