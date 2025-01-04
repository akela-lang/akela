#include <stdlib.h>
#include "buffer.h"

void get_piece(struct buffer* bf, char delim, size_t num, struct buffer* out)
{
    size_t count = 0;
    for (int i = 0; i < bf->size; i++) {
        if (bf->buf[i] == delim) {
            count++;
            i++;
        }
        if (count == num && i < bf->size) {
            buffer_add_char(out, bf->buf[i]);
        }
    }
}