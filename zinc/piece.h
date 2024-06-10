#ifndef PIECE_H
#define PIECE_H

#include "buffer.h"
#include <stdlib.h>

ZINC_API void get_piece(struct buffer* bf, char delim, size_t num, struct buffer* out);

#endif