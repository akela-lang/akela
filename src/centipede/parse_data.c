#include "parse_data.h"

#include "comp_unit.h"
#include "zinc/memory.h"

void Cent_parse_data_create(
    Cent_parse_data **pd,
    Zinc_error_list* errors,
    Cent_lex_data* ld,
    Cent_environment* base)
{
    Zinc_malloc_safe((void**)pd, sizeof(Cent_parse_data));
    Cent_parse_data_init(*pd, errors, ld, base);
}

void Cent_parse_data_init(
    Cent_parse_data *pd,
    Zinc_error_list* errors,
    Cent_lex_data* ld,
    Cent_environment* base
)
{
    pd->ld = ld;
    pd->lookahead = NULL;
    pd->top = NULL;
    pd->cu = NULL;
    pd->ct = NULL;
    pd->errors = errors;
    pd->base = base;
    Zinc_priority_queue_init(&pd->pq);
}

void Cent_parse_data_destroy(Cent_parse_data* pd)
{
    Zinc_priority_queue_destroy(&pd->pq);
}

void Cent_parse_result_init(Cent_parse_result *pr, Zinc_error_list* errors)
{
    pr->errors = errors;
    pr->base = NULL;
    pr->root = NULL;
}

void Cent_parse_result_create(Cent_parse_result **pr, Zinc_error_list* errors)
{
    Zinc_malloc_safe((void**)pr, sizeof(Cent_parse_result));
    Cent_parse_result_init(*pr, errors);
}