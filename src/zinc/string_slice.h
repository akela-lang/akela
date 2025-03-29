#ifndef ZINC_STRING_SLICE_H
#define ZINC_STRING_SLICE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Zinc_string_slice {
    const char* p;
    size_t size;
} Zinc_string_slice;

Zinc_string_slice Zinc_trim_left(Zinc_string_slice in);
Zinc_string_slice Zinc_trim_right(Zinc_string_slice in);
Zinc_string_slice Zinc_trim(Zinc_string_slice in);
Zinc_string_slice Zinc_string_slice_from_str(char* s);
bool Zinc_string_slice_compare(Zinc_string_slice* a, Zinc_string_slice* b);

#endif