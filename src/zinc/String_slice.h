#ifndef ZINC_STRING_SLICE_H
#define ZINC_STRING_SLICE_H

#include <stdlib.h>

typedef struct Zinc_string_slice {
    const char* p;
    size_t size;
} Zinc_string_slice;

#endif