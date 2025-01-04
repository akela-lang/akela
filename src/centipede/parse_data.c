#include "parse_data.h"
#include "zinc/memory.h"

void Cent_parse_data_init(Cent_parse_data *pd)
{
    pd->errors = NULL;
    pd->ld = NULL;
    pd->lookahead = NULL;
    pd->top = NULL;
    pd->file_name.p = NULL;
    pd->file_name.size = 0;
    pd->comp_table = NULL;
}

void Cent_parse_data_create(Cent_parse_data **pd)
{
    malloc_safe((void**)pd, sizeof(Cent_parse_data));
    Cent_parse_data_init(*pd);
}

void Cent_parse_result_init(Cent_parse_result *pr)
{
    pr->errors = NULL;
    pr->base = NULL;
    pr->root = NULL;
}

void Cent_parse_result_create(Cent_parse_result **pr)
{
    malloc_safe((void**)pr, sizeof(Cent_parse_result));
    Cent_parse_result_init(*pr);
}