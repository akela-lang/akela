#include "string_slice.h"
#include <stdlib.h>
#include <stdbool.h>

Zinc_string_slice Zinc_trim_left(Zinc_string_slice in)
{
    Zinc_string_slice out = {
        .p = in.p,
        .size = in.size,
    };
    for (size_t i = 0; i < in.size; i++) {
        if (in.p[i] != ' ') {
            break;
        }
        out.p++;
        out.size--;
    }

    return out;
}

Zinc_string_slice Zinc_trim_right(Zinc_string_slice in)
{
    Zinc_string_slice out = {
        .p = in.p,
        .size = in.size,
    };

    while (true) {
        if (out.size == 0) {
            break;
        }
        if (out.p[out.size - 1] != ' ') {
            break;
        }
        out.size--;
    }

    return out;
}

Zinc_string_slice Zinc_trim(Zinc_string_slice in)
{
    return Zinc_trim_right(Zinc_trim_left(in));
}