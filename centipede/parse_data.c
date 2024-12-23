#include "parse_data.h"
#include "zinc/memory.h"

void Cent_parse_data_init(Cent_parse_data *pd)
{
    pd->errors = NULL;
    pd->ld = NULL;
    pd->lookahead = NULL;
}

void Cent_parse_data_create(Cent_parse_data **pd)
{
    malloc_safe((void**)pd, sizeof(Cent_parse_data));
    Cent_parse_data_init(*pd);
}