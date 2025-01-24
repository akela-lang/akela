#include <stdlib.h>
#include "hash_map_string.h"
#include "string.h"
#include "memory.h"
#include <string.h>

unsigned int Zinc_hash_calc_string(struct Zinc_string* value, unsigned int size)
{
    unsigned int val = 0;

    for (int i = 0; i < value->size; i++) {
        val += (unsigned int)value->buf[i];
        val %= size;
    }

    return val;
}

unsigned int Zinc_hash_calc_str(char* str, unsigned int size)
{
    unsigned int val = 0;

    size_t len = strlen(str);
    for (int i = 0; i < len; i++) {
        val += (unsigned int)str[i];
        val %= size;
    }

    return val;
}

void Zinc_hash_map_string_entry_init(struct Zinc_hash_map_string_entry* ent)
{
    ent->item = NULL;
    Zinc_string_init(&ent->value);
    ent->next = NULL;
    ent->prev = NULL;
}

void Zinc_hash_map_string_entry_destroy(struct Zinc_hash_map_string_entry* ent)
{
    Zinc_string_destroy(&ent->value);
}

void Zinc_hash_map_string_list_init(struct Zinc_hash_map_string_list* hl)
{
    hl->head = NULL;
    hl->tail = NULL;
}

void Zinc_hash_map_string_list_destroy(struct Zinc_hash_map_string_list* hl)
{
    struct Zinc_hash_map_string_entry* p = hl->head;
    while (p) {
        struct Zinc_hash_map_string_entry* temp = p;
        p = p->next;
        Zinc_hash_map_string_entry_destroy(temp);
        free(temp);
    }
}

void Zinc_hash_map_string_init(struct Zinc_hash_table* ht, unsigned int size)
{
    ht->size = size;

    Zinc_malloc_safe((void**)&ht->buckets, sizeof(struct Zinc_hash_map_string_list) * size);

    for (int i = 0; i < size; i++) {
        Zinc_hash_map_string_list_init(&ht->buckets[i]);
    }
}

void Zinc_hash_map_string_create(struct Zinc_hash_table** ht, unsigned int size)
{
    Zinc_malloc_safe((void**)ht, sizeof(struct Zinc_hash_table));
    Zinc_hash_map_string_init(*ht, size);
}

void Zinc_hash_map_string_destroy(struct Zinc_hash_table* ht)
{
    if (ht) {
        for (int i = 0; i < ht->size; i++) {
            Zinc_hash_map_string_list_destroy(&ht->buckets[i]);
        }
        free(ht->buckets);
    }
}

void Zinc_hash_map_string_map(struct Zinc_hash_table* ht, Zinc_hash_map_string_func f)
{
    for (int i = 0; i < ht->size; i++) {
        struct Zinc_hash_map_string_entry* p = ht->buckets[i].head;
        while (p) {
            f(p->item);
            p = p->next;
        }
    }
}

void Zinc_hash_map_string_map_name(struct Zinc_hash_table* ht, Zinc_hash_map_string_func_name f)
{
    for (int i = 0; i < ht->size; i++) {
        struct Zinc_hash_map_string_entry* p = ht->buckets[i].head;
        while (p) {
            f(&p->value, p->item);
            p = p->next;
        }
    }
}

/* assume entry is not in table so call hash_table_get before if not sure */
void Zinc_hash_map_string_add(struct Zinc_hash_table* ht, struct Zinc_string* value, void* item)
{
    unsigned int val = Zinc_hash_calc_string(value, ht->size);

    struct Zinc_hash_map_string_entry* ent;
    Zinc_malloc_safe((void**)&ent, sizeof(struct Zinc_hash_map_string_entry));
    Zinc_hash_map_string_entry_init(ent);
    ent->item = item;

    Zinc_string_copy(value, &ent->value);

    /* add to beginning of bucket */
    struct Zinc_hash_map_string_entry* next = ht->buckets[val].head;
    ent->next = next;
    if (next) {
        next->prev = ent;
    }
    ht->buckets[val].head = ent;
    if (!ht->buckets[val].tail) {
        ht->buckets[val].tail = ent;
    }
}

/* assume entry is not in table so call hash_table_get before if not sure */
void Zinc_hash_map_string_add_str(struct Zinc_hash_table* ht, char* str, void* item)
{
    unsigned int val = Zinc_hash_calc_str(str, ht->size);

    struct Zinc_hash_map_string_entry* ent;
    Zinc_malloc_safe((void**)&ent, sizeof(struct Zinc_hash_map_string_entry));
    Zinc_hash_map_string_entry_init(ent);
    ent->item = item;

    Zinc_string_add_str(&ent->value, str);

    /* add to beginning of bucket */
    struct Zinc_hash_map_string_entry* next = ht->buckets[val].head;
    ent->next = next;
    if (next) {
        next->prev = ent;
    }
    ht->buckets[val].head = ent;
    if (!ht->buckets[val].tail) {
        ht->buckets[val].tail = ent;
    }
}

void* Zinc_hash_map_string_get(struct Zinc_hash_table* ht, struct Zinc_string* value)
{
    struct Zinc_hash_map_string_entry* ent;

    unsigned int val = Zinc_hash_calc_string(value, ht->size);

    ent = ht->buckets[val].head;
    while (ent) {
        if (Zinc_string_compare(&ent->value, value)) {
            return ent->item;
        }
        ent = ent->next;
    }

    return NULL;
}

void* Zinc_hash_map_string_get_str(struct Zinc_hash_table* ht, char* str)
{
    struct Zinc_hash_map_string_entry* ent;

    unsigned int val = Zinc_hash_calc_str(str, ht->size);

    ent = ht->buckets[val].head;
    while (ent) {
        if (Zinc_string_compare_str(&ent->value, str)) {
            return ent->item;
        }
        ent = ent->next;
    }

    return NULL;
}