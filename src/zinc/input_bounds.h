#ifndef ZINC_INPUT_BOUNDS_H
#define ZINC_INPUT_BOUNDS_H

#include "error.h"

typedef struct Zinc_input_bounds Zinc_input_bounds;
struct Zinc_input_bounds {
    Zinc_location loc;
    size_t end;
};

void Zinc_input_bounds_init(Zinc_input_bounds* bounds);

#endif