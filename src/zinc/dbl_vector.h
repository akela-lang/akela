#ifndef ZINC_DBL_VECTOR_H
#define ZINC_DBL_VECTOR_H

#include <stddef.h>

typedef struct Zinc_dbl_vector {
    size_t start;
    size_t end;
    void* buffer;
    size_t count;
    size_t container_size;
} Zinc_dbl_vector;

#endif