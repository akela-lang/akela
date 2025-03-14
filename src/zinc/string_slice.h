#ifndef ZINC_STRING_SLICE_H
#define ZINC_STRING_SLICE_H

#include <stdlib.h>

typedef struct Zinc_string_slice {
    const char* p;
    size_t size;
} Zinc_string_slice;

Zinc_string_slice Zinc_trim_left(Zinc_string_slice in);
Zinc_string_slice Zinc_trim_right(Zinc_string_slice in);
Zinc_string_slice Zinc_trim(Zinc_string_slice in);

#endif