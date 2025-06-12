#include "zinc/error.h"

void Json_location_combine(struct Zinc_location* loc, struct Zinc_location* new_loc)
{
    if (loc->line == 0) {
        *loc = *new_loc;
    } else {
        loc->end = new_loc->end;
    }
}