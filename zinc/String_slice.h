#ifndef STRING_SLICE_H
#define STRING_SLICE_H

#include <stdlib.h>

typedef struct String_slice {
    const char* p;
    size_t size;
} String_slice;

#endif