#include <stdlib.h>
#include "result.h"

enum result malloc_safe(void** buf, size_t size)
{
    *buf = malloc(size);
    if (*buf == NULL) {
        return set_error("Out of memory");
    }
    return result_ok;
}

enum result realloc_safe(void** buf, size_t size)
{
    void* new_buf;
    new_buf = realloc(*buf, size);
    if (new_buf == NULL) {
        return set_error("Out of memory");
    }
    *buf = new_buf;
    return result_ok;
}
