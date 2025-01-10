#include "parse_data.h"
#include "zinc/memory.h"

void Cent_parse_data_init(Cent_parse_data *pd, struct error_list* errors, Cent_lex_data* ld)
{
    pd->ld = ld;
    pd->lookahead = NULL;
    pd->top = NULL;
    pd->file_name.p = NULL;
    pd->file_name.size = 0;
    pd->comp_table = NULL;
    pd->module_obj = NULL;
    pd->module_vtable = NULL;
    pd->errors = errors;
}

void Cent_parse_data_create(Cent_parse_data **pd, struct error_list* errors, Cent_lex_data* ld)
{
    malloc_safe((void**)pd, sizeof(Cent_parse_data));
    Cent_parse_data_init(*pd, errors, ld);
}

void Cent_parse_result_init(Cent_parse_result *pr, struct error_list* errors)
{
    pr->errors = errors;
    pr->base = NULL;
    pr->root = NULL;
}

void Cent_parse_result_create(Cent_parse_result **pr, struct error_list* errors)
{
    malloc_safe((void**)pr, sizeof(Cent_parse_result));
    Cent_parse_result_init(*pr, errors);
}