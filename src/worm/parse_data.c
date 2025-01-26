#include "parse_data.h"
#include "zinc/memory.h"

void Worm_parse_data_init(Worm_parse_data* pd, Worm_lex_data* ld, Zinc_error_list* errors)
{
    pd->ld = ld;
    pd->errors = errors;
    pd->lookahead = NULL;
}

void Worm_parse_data_create(Worm_parse_data** pd, Worm_lex_data* ld, Zinc_error_list* errors)
{
    Zinc_malloc_safe((void**)pd, sizeof(Worm_parse_data));
    Worm_parse_data_init(*pd, ld, errors);
}
