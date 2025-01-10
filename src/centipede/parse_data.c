#include "parse_data.h"
#include "zinc/memory.h"

void Cent_parse_data_init(
    Cent_parse_data *pd,
    struct error_list* errors,
    Cent_lex_data* ld,
    String_slice file_name)
{
    pd->ld = ld;
    pd->lookahead = NULL;
    pd->top = NULL;
    pd->file_name.p = NULL;
    pd->file_name.size = 0;
    pd->cu = NULL;
    pd->ct = NULL;
    pd->errors = errors;
    pd->file_name = file_name;
}

void Cent_parse_data_create(
    Cent_parse_data **pd,
    struct error_list* errors,
    Cent_lex_data* ld,
    String_slice file_name)
{
    malloc_safe((void**)pd, sizeof(Cent_parse_data));
    Cent_parse_data_init(*pd, errors, ld, file_name);
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