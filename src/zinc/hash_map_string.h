#ifndef ZINC_HASH_MAP_STRING_H
#define ZINC_HASH_MAP_STRING_H

#include "api.h"
#include "zstring.h"

typedef struct Zinc_hash_map_string_entry {
    void* item;
    Zinc_string value;
    struct Zinc_hash_map_string_entry* next;
    struct Zinc_hash_map_string_entry* prev;
} Zinc_hash_map_string_entry;

typedef struct Zinc_hash_map_string_list {
    Zinc_hash_map_string_entry* head;
    Zinc_hash_map_string_entry* tail;
} Zinc_hash_map_string_list;

typedef struct Zinc_hash_table {
    unsigned int size;
     Zinc_hash_map_string_list* buckets;
} Zinc_hash_map_string;

typedef void (*Zinc_hash_map_string_func)(void*);
typedef void (*Zinc_hash_map_string_func_name)(Zinc_string*, void*);

ZINC_API unsigned int Zinc_hash_calc_string(Zinc_string* value, unsigned int size);
ZINC_API unsigned int Zinc_hash_calc_str(char* str, unsigned int size);
ZINC_API void Zinc_hash_map_string_entry_init(Zinc_hash_map_string_entry* ent);
ZINC_API void Zinc_hash_map_string_entry_destroy(Zinc_hash_map_string_entry* ent);
ZINC_API void Zinc_hash_map_string_list_init(Zinc_hash_map_string_list* hl);
ZINC_API void Zinc_hash_map_string_list_destroy(Zinc_hash_map_string_list* hl);
ZINC_API void Zinc_hash_map_string_map(Zinc_hash_map_string* ht, Zinc_hash_map_string_func f);
ZINC_API void Zinc_hash_map_string_map_name(Zinc_hash_map_string* ht, Zinc_hash_map_string_func_name f);
ZINC_API void Zinc_hash_map_string_init(Zinc_hash_map_string* ht, unsigned int size);
ZINC_API void Zinc_hash_map_string_create(Zinc_hash_map_string** ht, unsigned int size);
ZINC_API void Zinc_hash_map_string_destroy(Zinc_hash_map_string* ht);
ZINC_API void Zinc_hash_map_string_add(Zinc_hash_map_string* ht, Zinc_string* value, void* item);
ZINC_API void Zinc_hash_map_string_add_str(Zinc_hash_map_string* ht, char* str, void* item);
ZINC_API void* Zinc_hash_map_string_get(Zinc_hash_map_string* ht, Zinc_string* value);
ZINC_API void* Zinc_hash_map_string_get_str(Zinc_hash_map_string* ht, char* str);

#endif
