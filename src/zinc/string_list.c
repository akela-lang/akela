#include "string_list.h"
#include <stdlib.h>
#include "memory.h"

void Zinc_string_node_init(Zinc_buffer_node* bn)
{
    Zinc_string_init(&bn->value);
    bn->next = NULL;
    bn->prev = NULL;
}

void Zinc_string_node_destroy(Zinc_buffer_node* bn)
{
    Zinc_string_destroy(&bn->value);
}

void Zinc_string_list_init(Zinc_string_list *bl)
{
    bl->head = NULL;
    bl->tail = NULL;
}

void Zinc_string_list_create(Zinc_string_list** bl)
{
    malloc_safe((void**)bl, sizeof(Zinc_string_list));
    Zinc_string_list_init(*bl);
}

void Zinc_string_list_destroy(Zinc_string_list* bl)
{
    Zinc_buffer_node* bn = bl->head;
    while (bn) {
        Zinc_buffer_node* temp = bn;
        bn = bn->next;
        Zinc_string_node_destroy(temp);
        free(temp);
    }
}

void Zinc_string_list_add(Zinc_string_list* bl, Zinc_buffer_node* bn)
{
    if (!bl->head && !bl->tail) {
        /* empty */
        bl->head = bn;
        bl->tail = bn;
    } else {
        /* not empty */
        Zinc_buffer_node* prev = bl->tail;
        prev->next = bn;
        bn->prev = prev;
        bl->tail = bn;
    }
}

void Zinc_string_list_add_str(Zinc_string_list* bl, const char* str)
{
    Zinc_buffer_node* bn = NULL;
    malloc_safe((void**)&bn, sizeof(Zinc_buffer_node));
    Zinc_string_node_init(bn);
    Zinc_string_add_str(&bn->value, str);
    Zinc_string_list_add(bl, bn);
}

void Zinc_string_list_add_bf(Zinc_string_list* bl, Zinc_string* bf)
{
    Zinc_buffer_node* bn = NULL;
    malloc_safe((void**)&bn, sizeof(Zinc_buffer_node));
    Zinc_string_node_init(bn);
    Zinc_string_copy(bf, &bn->value);
    Zinc_string_list_add(bl, bn);
}

size_t Zinc_string_list_count(Zinc_string_list* bl)
{
    size_t count = 0;

    Zinc_buffer_node* bn = bl->head;
    while (bn) {
        count++;
        bn = bn->next;
    }
    return count;
}

void Zinc_string_split(Zinc_string* bf, Zinc_string_list* bl)
{
    Zinc_string current;
    Zinc_string_init(&current);

    size_t pos = 0;
    while (pos < bf->size) {
        if (bf->buf[pos] == ' ') {
            if (current.size > 0) {
                Zinc_string_list_add_bf(bl, &current);
                Zinc_string_clear(&current);
            }
        } else {
            Zinc_string_add_char(&current, bf->buf[pos]);
        }
        pos++;
    }

    if (current.size > 0) {
        Zinc_string_list_add_bf(bl, &current);
        Zinc_string_clear(&current);
    }

    Zinc_string_destroy(&current);
}

Zinc_string* Zinc_string_list_get(Zinc_string_list* bl, size_t num)
{
    Zinc_buffer_node* bn = bl->head;
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

bool Zinc_string_list_contains(Zinc_string_list* bl, Zinc_string* value)
{
    Zinc_buffer_node* bn = bl->head;
    while (bn) {
        if (Zinc_string_compare(&bn->value, value)) {
            return true;
        }
        bn = bn->next;
    }

    return false;
}

bool Zinc_string_list_contains_str(Zinc_string_list* bl, const char* str)
{
    Zinc_buffer_node* bn = bl->head;
    while (bn) {
        if (Zinc_string_compare_str(&bn->value, str)) {
            return true;
        }
        bn = bn->next;
    }

    return false;
}