#include <stdlib.h>
#include "zstring.h"

void Zinc_get_piece(Zinc_string* bf, char delim, size_t num, Zinc_string* out)
{
    size_t count = 0;
    for (int i = 0; i < bf->size; i++) {
        if (bf->buf[i] == delim) {
            count++;
            i++;
        }
        if (count == num && i < bf->size) {
            Zinc_string_add_char(out, bf->buf[i]);
        }
    }
}