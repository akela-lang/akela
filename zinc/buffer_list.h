#ifndef BUFFER_LIST_H
#define BUFFER_LIST_H

#include "api.h"
#include "buffer.h"
#include <stdbool.h>

struct buffer_node {
    struct buffer value;
    struct buffer_node* next;
    struct buffer_node* prev;
};

struct buffer_list {
    struct buffer_node* head;
    struct buffer_node* tail;
};

#ifdef __cplusplus
extern "C" {
#endif

ZINC_API void buffer_node_init(struct buffer_node* bn);
ZINC_API void buffer_node_destroy(struct buffer_node* bn);
ZINC_API void buffer_list_init(struct buffer_list *bl);
ZINC_API void buffer_list_create(struct buffer_list** bl);
ZINC_API void buffer_list_destroy(struct buffer_list* bl);
ZINC_API void buffer_list_add(struct buffer_list* bl, struct buffer_node* bn);
ZINC_API void buffer_list_add_str(struct buffer_list* bl, const char* str);
ZINC_API void buffer_list_add_bf(struct buffer_list* bl, struct buffer* bf);
ZINC_API size_t buffer_list_count(struct buffer_list* bl);
ZINC_API void buffer_split(struct buffer* bf, struct buffer_list* bl);
ZINC_API struct buffer* buffer_list_get(struct buffer_list* bl, size_t num);
ZINC_API bool buffer_list_contains(struct buffer_list* bl, struct buffer* value);
ZINC_API bool buffer_list_contains_str(struct buffer_list* bl, const char* str);

#ifdef __cplusplus
}
#endif

#endif