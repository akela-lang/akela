#include "dbl_vector.h"
#include "memory.h"
#include <string.h>

void Zinc_dbl_vector_init(Zinc_dbl_vector* dv, size_t item_size)
{
    dv->start = 0;
    dv->end = 0;
    dv->buffer = NULL;
    dv->count = 0;
    dv->item_size = item_size;
    dv->capacity = 0;
}

void Zinc_dbl_vector_create(Zinc_dbl_vector** dv, size_t item_size)
{
    Zinc_malloc_safe((void**)dv, sizeof(Zinc_dbl_vector));
    Zinc_dbl_vector_init(*dv, item_size);
}

void Zinc_dbl_vector_destroy(Zinc_dbl_vector* dv)
{
    free(dv->buffer);
}

void Zinc_dbl_vector_expand_right(Zinc_dbl_vector* dv, size_t size)
{
    bool expanded = false;
    if (dv->capacity == 0) {
        dv->capacity = ZINC_DBL_VECTOR_START_SIZE;
        dv->start = dv->end = dv->capacity / 2;
        expanded = true;
    }
    while (dv->capacity - dv->end < size) {
        dv->capacity *= 2;
        expanded = true;
    }
    if (expanded) {
        void* new_buffer = NULL;
        Zinc_malloc_safe(&new_buffer, dv->capacity);
        if (dv->end > dv->start) {
            memcpy((uint8_t*)new_buffer + dv->start, (uint8_t*)dv->buffer + dv->start, dv->end - dv->start);
        }
        free(dv->buffer);
        dv->buffer = new_buffer;
    }
}

void Zinc_dbl_vector_expand_left(Zinc_dbl_vector* dv, size_t size)
{
    if (dv->capacity == 0) {
        dv->capacity = ZINC_DBL_VECTOR_START_SIZE;
        dv->start = dv->end = dv->capacity / 2;
    }
    size_t start = dv->start;
    size_t end = dv->end;
    size_t capacity = dv->capacity;
    while (start < size) {
        capacity *= 2;
        end = capacity - (dv->capacity - dv->end);
        start = capacity - (dv->end - dv->start + size);
    }
    void* new_buffer = NULL;
    Zinc_malloc_safe(&new_buffer, capacity);
    if (dv->end > dv->start) {
        memcpy((uint8_t*)new_buffer + start, (uint8_t*)dv->buffer + dv->start, dv->end - dv->start);
    }
    free(dv->buffer);
    dv->start = start;
    dv->end = start + dv->count * dv->item_size;
    dv->capacity = capacity;
    dv->buffer = new_buffer;
}

void Zinc_dbl_vector_append(Zinc_dbl_vector* dv, void* item, size_t num)
{
    Zinc_dbl_vector_expand_right(dv, dv->item_size * num);
    memcpy((uint8_t*)dv->buffer + dv->end, item, dv->item_size * num);
    dv->count += num;
    dv->end += num * dv->item_size;
}

void Zinc_dbl_vector_prepend(Zinc_dbl_vector* dv, void* item, size_t num)
{
    Zinc_dbl_vector_expand_left(dv, dv->item_size * num);
    dv->start -= num * dv->item_size;
    memcpy((uint8_t*)dv->buffer + dv->start, item, dv->item_size * num);
    dv->count += num;
}

void* Zinc_dbl_vector_slice(Zinc_dbl_vector* dv, size_t* size)
{
    *size = dv->end - dv->start;
    return (uint8_t*)dv->buffer + dv->start;
}