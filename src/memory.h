#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdlib.h>
#include "result.h"

enum result_enum malloc_safe(void** buf, size_t size);
enum result_enum realloc_safe(void** buf, size_t size);

#endif
