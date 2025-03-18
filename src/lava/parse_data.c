#include "parse_data.h"
#include "zinc/memory.h"

void Lava_parse_data_init(Lava_parse_data* pd, Lava_lex_data* ld, Zinc_error_list* errors)
{
    pd->ld = ld;
    pd->errors = errors;
    pd->lookahead = NULL;
}

void Lava_parse_data_create(Lava_parse_data** pd, Lava_lex_data* ld, Zinc_error_list* errors)
{
    Zinc_malloc_safe((void**)pd, sizeof(Lava_parse_data));
    Lava_parse_data_init(*pd, ld, errors);
}

void Lava_result_init(Lava_result* lr, Zinc_error_list* errors, Lava_dom* root)
{
    lr->errors = errors;
    lr->root = root;
}

void Lava_result_create(Lava_result** lr, Zinc_error_list* errors, Lava_dom* root)
{
    Zinc_malloc_safe((void**)lr, sizeof(Lava_result));
    Lava_result_init(*lr, errors, root);
}

void Lava_result_destroy(Lava_result* lr)
{
    Zinc_error_list_destroy(lr->errors);
    free(lr->errors);
    Lava_dom_destroy(lr->root);
    free(lr->root);
}