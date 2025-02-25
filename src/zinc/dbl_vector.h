#ifndef ZINC_DBL_VECTOR_H
#define ZINC_DBL_VECTOR_H

#include <stddef.h>

typedef struct Zinc_dbl_vector {
    size_t start;
    size_t end;
    void* buffer;
    size_t count;
    size_t item_size;
    size_t capacity;
} Zinc_dbl_vector;

#define ZINC_DBL_VECTOR_START_SIZE 64

void Zinc_dbl_vector_init(Zinc_dbl_vector* dv, size_t item_size);
void Zinc_dbl_vector_create(Zinc_dbl_vector** dv, size_t item_size);
void Zinc_dbl_vector_destroy(Zinc_dbl_vector* dv);
void Zinc_dbl_vector_expand_right(Zinc_dbl_vector* dv, size_t size);
void Zinc_dbl_vector_expand_left(Zinc_dbl_vector* dv, size_t size);
void Zinc_dbl_vector_append(Zinc_dbl_vector* dv, void* item, size_t num);
void Zinc_dbl_vector_prepend(Zinc_dbl_vector* dv, void* item, size_t num);
void* Zinc_dbl_vector_slice(Zinc_dbl_vector* dv, size_t* size);

#endif