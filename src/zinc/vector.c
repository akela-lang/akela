#include "vector.h"
#include "memory.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

void Zinc_vector_init(Zinc_vector* v, size_t value_size)
{
    v->buffer = NULL;
    v->value_size = value_size;
    v->count = 0;
    v->container_size = 0;
}

void Zinc_vector_create(Zinc_vector** v, size_t value_size)
{
    Zinc_malloc_safe((void**)v, sizeof(Zinc_vector));
    Zinc_vector_init(*v, value_size);
}

/**
 * Add space to Vector.
 * @param v Vector
 * @param count the count of elements
 */
void Zinc_vector_expand(Zinc_vector* v, size_t count)
{
    assert(v->value_size > 0);

    size_t copy_size = v->value_size * count;
    size_t current_size = v->value_size * v->count;
    if (current_size + copy_size > v->container_size) {
        size_t old_container_size = v->container_size;
        if (copy_size < ZINC_SERIES_CHUNK_SIZE) {
            v->container_size = current_size + ZINC_SERIES_CHUNK_SIZE;
        } else {
            v->container_size = current_size + copy_size;
        }
        if (old_container_size == 0) {
            Zinc_malloc_safe(&v->buffer, v->container_size);
        } else {
            Zinc_realloc_safe(&v->buffer, v->container_size);
        }
    }
}

/**
 * Add a number of values to the end of the Vector.
 * @param v the Vector
 * @param buffer the buffer holding the new values
 * @param count the number of new elements
 */
void Zinc_vector_add(Zinc_vector* v, const void* buffer, size_t count)
{
    Zinc_vector_expand(v, count);
    memcpy(ZINC_VECTOR_PTR(v, v->count), buffer, v->value_size * count);
    v->count += count;
}

/**
 * Add a null byte. Only use if value_size is 1 byte.
 * @param v Vector
 */
void Zinc_vector_add_null(Zinc_vector* v)
{
    assert(v->value_size == sizeof(u_int8_t));
    u_int8_t value = 0x0;
    Zinc_vector_add(v, &value, 1);
    v->count--;
}

void Zinc_vector_destroy(Zinc_vector* v)
{
    free(v->buffer);
}

bool Zinc_vector_match(Zinc_vector* a, Zinc_vector* b)
{
    return Zinc_memory_match(
            a->buffer, a->count * a->value_size,
            b->buffer, b->count * b->value_size);
}

bool Zinc_vector_match_str(Zinc_vector* a, const char b[])
{
    return Zinc_memory_match(
            a->buffer, a->count * a->value_size,
            (u_int8_t *) b, strlen(b));
}

void Zinc_vector_copy(Zinc_vector* src, Zinc_vector* dest)
{
    assert(src->value_size == dest->value_size);
    Zinc_vector_add(dest, src->buffer, src->count);
}

void Zinc_vector_print_double(Zinc_vector* v)
{
    for (size_t i = 0; i < v->count; i++) {
        printf("%15.15lf\n", ZINC_VECTOR_DOUBLE(v, i));
    }
}