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

#define ZINC_VECTOR_CALC_SIZE_LINEAR_CHUNK_SIZE 64

size_t Zinc_vector_calc_size_linear(Zinc_vector* v, size_t count)
{
    size_t container_size;

    size_t copy_size = v->value_size * count;
    size_t current_size = v->value_size * v->count;
    container_size = v->container_size;
    while (container_size < current_size + copy_size) {
        container_size += ZINC_VECTOR_CALC_SIZE_LINEAR_CHUNK_SIZE;
    }
    return container_size;
}

#define ZINC_VECTOR_CALC_SIZE_EXPO_INIT_SIZE 64

size_t Zinc_vector_calc_size_expo(Zinc_vector* v, size_t count)
{
    size_t container_size;

    size_t copy_size = v->value_size * count;
    size_t current_size = v->value_size * v->count;
    container_size = v->container_size;
    if (container_size == 0) {
        container_size = ZINC_VECTOR_CALC_SIZE_EXPO_INIT_SIZE;
    }
    while (container_size < current_size + copy_size) {
        container_size *= 2;
    }
    return container_size;
}

//#define ZINC_VECTOR_CALC_SIZE_LINEAR
#define ZINC_VECTOR_CALC_SIZE_EXPO

#define ZINC_VECTOR_COPY
//#define ZINC_VECTOR_REALLOC

void Zinc_vector_expand(Zinc_vector* v, size_t count)
{
#ifdef ZINC_VECTOR_CALC_SIZE_LINEAR
    size_t container_size = Zinc_vector_calc_size_linear(v, count);
#elif defined(ZINC_VECTOR_CALC_SIZE_EXPO)
    size_t container_size = Zinc_vector_calc_size_expo(v, count);
#else
    assert(false && "choose Zinc vector calc size function");
#endif

    if (container_size > v->container_size) {
#ifdef ZINC_VECTOR_REALLOC
        if (v->buffer) {
            Zinc_realloc_safe(&v->buffer, container_size);
        } else {
            Zinc_malloc_safe(&v->buffer, container_size);
        }
        v->container_size = container_size;
#elif defined(ZINC_VECTOR_COPY)
        void* new_buffer = NULL;
        Zinc_malloc_safe(&new_buffer, container_size);
        memcpy(new_buffer, v->buffer, v->count * v->value_size);
        free(v->buffer);
        v->buffer = new_buffer;
        v->container_size = container_size;
#endif
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
    memcpy(v->buffer + v->count * v->value_size, buffer, count * v->value_size);
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

void Zinc_vector_shift(Zinc_vector* v, const void* buffer, size_t count)
{
#ifdef ZINC_VECTOR_CALC_SIZE_LINEAR
    size_t container_size = Zinc_vector_calc_size_linear(v, count);
#elif defined(ZINC_VECTOR_CALC_SIZE_EXPO)
    size_t container_size = Zinc_vector_calc_size_expo(v, count);
#else
    assert(false && "choose Zinc vector calc size function");
#endif

    // shift
    void* new_buffer = NULL;
    Zinc_malloc_safe(&new_buffer, container_size);
    memcpy(new_buffer + count * v->value_size, v->buffer, v->count * v->value_size);
    free(v->buffer);
    v->buffer = new_buffer;

    // prepend
    memcpy(v->buffer, buffer, count * v->value_size);
    v->count += count;
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