#ifndef ZINC_VECTOR_H
#define ZINC_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define ZINC_SERIES_CHUNK_SIZE 64

typedef struct {
    void* buffer;
    size_t value_size;
    size_t count;
    size_t container_size;
} Zinc_vector;

#ifdef __cplusplus
extern "C" {
#endif
void Zinc_vector_init(Zinc_vector* v, size_t value_size);
void Zinc_vector_create(Zinc_vector** v, size_t value_size);
void Zinc_vector_expand(Zinc_vector* v, size_t count);
void Zinc_vector_add(Zinc_vector* v, const void* buffer, size_t count);
void Zinc_vector_add_null(Zinc_vector* v);
void Zinc_vector_destroy(Zinc_vector* v);
bool Zinc_vector_match(Zinc_vector* a, Zinc_vector* b);
bool Zinc_vector_match_str(Zinc_vector* a, const char b[]);
void Zinc_vector_copy(Zinc_vector* src, Zinc_vector* dest);
void Zinc_vector_print_double(Zinc_vector* v);

#ifdef __cplusplus
}
#endif

#define ZINC_VECTOR_PTR(s, index) ((u_int8_t*)(s)->buffer + ((s)->value_size * (index)))
#define ZINC_VECTOR_DOUBLE(s, index) (*(double*)((s)->buffer + ((s)->value_size * (index))))
#define ZINC_VECTOR_U_LONG(s, index) (*(unsigned long*)((s)->buffer + ((s)->value_size * (index))))
#define ZINC_VECTOR_INT(s, index) (*(int*)((s)->buffer + ((s)->value_size * (index))))
#define ZINC_VECTOR_INT_U(s, index) (*(unsigned int*)((s)->buffer + ((s)->value_size * (index))))
#define ZINC_VECTOR_LONG(s, index) (*(long*)((s)->buffer + ((s)->value_size * (index))))
#define ZINC_VECTOR_LONG_U(s, index) (*(unsigned long*)((s)->buffer + ((s)->value_size * (index))))
#define ZINC_VECTOR_BOOL(s, index) (*(u_int8_t*)((s)->buffer + ((s)->value_size * (index))))
#define ZINC_VECTOR_CHAR(s, index) (*(char*)((s)->buffer + ((s)->value_size * (index))))
#define ZINC_VECTOR_STRING(s) ((char*)(s)->buffer)
#define ZINC_VECTOR_VECTOR(s, index) (*(Zinc_vector**)((s)->buffer + ((s)->value_size * (index))))

#endif