#ifndef ZINC_MEMORY_H
#define ZINC_MEMORY_H

#include "api.h"
#include <stdlib.h>
#include "result.h"
#include <stdbool.h>

ZINC_API void Zinc_malloc_safe(void** buf, size_t size);
ZINC_API void Zinc_realloc_safe(void** buf, size_t size);
ZINC_API bool Zinc_memory_match(const u_int8_t a[], size_t a_size, const u_int8_t b[], size_t b_size);

#endif
