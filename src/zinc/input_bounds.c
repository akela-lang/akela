#include "input_bounds.h"

void Zinc_input_bounds_init(Zinc_input_bounds* bounds)
{
    Zinc_location_init(&bounds->loc);
    bounds->end = 0;
}