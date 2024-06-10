#ifndef _MEMORY_H
#define _MEMORY_H

#include "api.h"
#include <stdlib.h>
#include "result.h"
#include <stdbool.h>

/* dynamic-output buf */
ZINC_API void malloc_safe(void** buf, size_t size);

/* dynamic-output buf */
ZINC_API void realloc_safe(void** buf, size_t size);

ZINC_API bool memory_match(const u_int8_t a[], size_t a_size, const u_int8_t b[], size_t b_size);

#endif
