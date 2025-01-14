#ifndef BUFFER_LIST_H
#define BUFFER_LIST_H

#include "api.h"
#include "zstring.h"
#include <stdbool.h>

typedef struct Zinc_buffer_node {
    Zinc_string value;
    struct Zinc_buffer_node* next;
    struct Zinc_buffer_node* prev;
} Zinc_buffer_node;

typedef struct Zinc_string_list {
    Zinc_buffer_node* head;
    Zinc_buffer_node* tail;
} Zinc_string_list;

#ifdef __cplusplus
extern "C" {
#endif

ZINC_API void Zinc_string_node_init(Zinc_buffer_node* bn);
ZINC_API void Zinc_string_node_destroy(Zinc_buffer_node* bn);
ZINC_API void Zinc_string_list_init(Zinc_string_list *bl);
ZINC_API void Zinc_string_list_create(Zinc_string_list** bl);
ZINC_API void Zinc_string_list_destroy(Zinc_string_list* bl);
ZINC_API void Zinc_string_list_add(Zinc_string_list* bl, Zinc_buffer_node* bn);
ZINC_API void Zinc_string_list_add_str(Zinc_string_list* bl, const char* str);
ZINC_API void Zinc_string_list_add_bf(Zinc_string_list* bl, Zinc_string* bf);
ZINC_API size_t Zinc_string_list_count(Zinc_string_list* bl);
ZINC_API void Zinc_string_split(Zinc_string* bf, Zinc_string_list* bl);
ZINC_API Zinc_string* Zinc_string_list_get(Zinc_string_list* bl, size_t num);
ZINC_API bool Zinc_string_list_contains(Zinc_string_list* bl, Zinc_string* value);
ZINC_API bool Zinc_string_list_contains_str(Zinc_string_list* bl, const char* str);

#ifdef __cplusplus
}
#endif

#endif