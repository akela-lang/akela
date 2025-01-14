#include "buffer_list.h"
#include <stdlib.h>
#include "memory.h"

void buffer_node_init(struct buffer_node* bn)
{
    Zinc_string_init(&bn->value);
    bn->next = NULL;
    bn->prev = NULL;
}

void buffer_node_destroy(struct buffer_node* bn)
{
    Zinc_string_destroy(&bn->value);
}

void buffer_list_init(struct buffer_list *bl)
{
    bl->head = NULL;
    bl->tail = NULL;
}

void buffer_list_create(struct buffer_list** bl)
{
    malloc_safe((void**)bl, sizeof(struct buffer_list));
    buffer_list_init(*bl);
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
    Zinc_string_add_str(&bn->value, str);
    buffer_list_add(bl, bn);
}

void buffer_list_add_bf(struct buffer_list* bl, struct Zinc_string* bf)
{
    struct buffer_node* bn = NULL;
    malloc_safe((void**)&bn, sizeof(struct buffer_node));
    buffer_node_init(bn);
    Zinc_string_copy(bf, &bn->value);
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

void buffer_split(struct Zinc_string* bf, struct buffer_list* bl)
{
    struct Zinc_string current;
    Zinc_string_init(&current);

    size_t pos = 0;
    while (pos < bf->size) {
        if (bf->buf[pos] == ' ') {
            if (current.size > 0) {
                buffer_list_add_bf(bl, &current);
                Zinc_string_clear(&current);
            }
        } else {
            Zinc_string_add_char(&current, bf->buf[pos]);
        }
        pos++;
    }

    if (current.size > 0) {
        buffer_list_add_bf(bl, &current);
        Zinc_string_clear(&current);
    }

    Zinc_string_destroy(&current);
}

struct Zinc_string* buffer_list_get(struct buffer_list* bl, size_t num)
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

bool buffer_list_contains(struct buffer_list* bl, struct Zinc_string* value)
{
    struct buffer_node* bn = bl->head;
    while (bn) {
        if (Zinc_string_compare(&bn->value, value)) {
            return true;
        }
        bn = bn->next;
    }

    return false;
}

bool buffer_list_contains_str(struct buffer_list* bl, const char* str)
{
    struct buffer_node* bn = bl->head;
    while (bn) {
        if (Zinc_string_compare_str(&bn->value, str)) {
            return true;
        }
        bn = bn->next;
    }

    return false;
}