#include "buffer_list.h"
#include <stdlib.h>
#include "memory.h"

void buffer_node_init(struct buffer_node* bn)
{
    buffer_init(&bn->value);
    bn->next = NULL;
    bn->prev = NULL;
}

void buffer_node_destroy(struct buffer_node* bn)
{
    buffer_destroy(&bn->value);
}

void buffer_list_init(struct buffer_list *bl)
{
    bl->head = NULL;
    bl->tail = NULL;
}

void buffer_list_destroy(struct buffer_list* bl)
{
    struct buffer_node* bn = bl->head;
    while (bn) {
        struct buffer_node* temp = bn;
        bn = bn->next;
        buffer_node_destroy(temp);
        free(temp);
    }
}

void buffer_list_add(struct buffer_list* bl, struct buffer_node* bn)
{
    if (!bl->head && !bl->tail) {
        /* empty */
        bl->head = bn;
        bl->tail = bn;
    } else {
        /* not empty */
        struct buffer_node* prev = bl->tail;
        prev->next = bn;
        bn->prev = prev;
        bl->tail = bn;
    }
}

void buffer_list_add_str(struct buffer_list* bl, const char* str)
{
    struct buffer_node* bn = NULL;
    malloc_safe((void**)&bn, sizeof(struct buffer_node));
    buffer_node_init(bn);
    buffer_copy_str(&bn->value, str);
    buffer_list_add(bl, bn);
}

void buffer_list_add_bf(struct buffer_list* bl, struct buffer* bf)
{
    struct buffer_node* bn = NULL;
    malloc_safe((void**)&bn, sizeof(struct buffer_node));
    buffer_node_init(bn);
    buffer_copy(bf, &bn->value);
    buffer_list_add(bl, bn);
}

size_t buffer_list_count(struct buffer_list* bl)
{
    size_t count = 0;

    struct buffer_node* bn = bl->head;
    while (bn) {
        count++;
        bn = bn->next;
    }
    return count;
}

void buffer_split(struct buffer* bf, struct buffer_list* bl)
{
    struct buffer current;
    buffer_init(&current);

    size_t pos = 0;
    while (pos < bf->size) {
        if (bf->buf[pos] == ' ') {
            if (current.size > 0) {
                buffer_list_add_bf(bl, &current);
                buffer_clear(&current);
            }
        } else {
            buffer_add_char(&current, bf->buf[pos]);
        }
        pos++;
    }

    if (current.size > 0) {
        buffer_list_add_bf(bl, &current);
        buffer_clear(&current);
    }

    buffer_destroy(&current);
}

struct buffer* buffer_list_get(struct buffer_list* bl, size_t num)
{
    struct buffer_node* bn = bl->head;
    size_t i = 0;
    while (bn) {
        if (i == num) {
            return &bn->value;
        }
        bn = bn->next;
        i++;
    }
    return NULL;
}