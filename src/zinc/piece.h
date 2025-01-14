#ifndef PIECE_H
#define PIECE_H

#include "zstring.h"
#include <stdlib.h>

ZINC_API void get_piece(struct Zinc_string* bf, char delim, size_t num, struct Zinc_string* out);

#endif