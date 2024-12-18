#include "data.h"
#include "zinc/memory.h"

void Coverage_file_init(Coverage_file *cf)
{
    buffer_init(&cf->path);
    cf->number_of_lines = 0;
    cf->number_covered = 0;
    cf->number_not_covered = 0;
    cf->coverage_percentage = 0.0;
    cf->next = NULL;
    cf->prev = NULL;
}

void Coverage_file_create(Coverage_file **cf)
{
    malloc_safe((void**)cf, sizeof(Coverage_file));
    Coverage_file_init(*cf);
}

void Coverage_file_destroy(Coverage_file *cf)
{
    buffer_destroy(&cf->path);
}