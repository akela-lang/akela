#include <stdlib.h>
#include <stdio.h>
#include "memory.h"
#include "result.h"
#include <stdbool.h>
#include <stdint.h>

void Zinc_malloc_safe(void** buf, size_t size)
{
    *buf = malloc(size);
    if (*buf == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
}

void Zinc_realloc_safe(void** buf, size_t size)
{
    void* new_buf;
    new_buf = realloc(*buf, size);
    if (new_buf == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    *buf = new_buf;
}

void free_safe(void* p)
{
    free(p);
}

/**
 * Check if byte arrays memory_match.
 * @param a first array
 * @param a_size first array size
 * @param b second array
 * @param b_size second array size
 * @return true if they memory_match, otherwise false
 */
bool Zinc_memory_match(const uint8_t a[], size_t a_size, const uint8_t b[], size_t b_size)
{
    if (a_size != b_size) {
        return false;
    }

    for (size_t i = 0; i < a_size; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}