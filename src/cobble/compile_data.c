#include "compile_data.h"
#include "zinc/memory.h"
#include "token.h"

void Cob_compile_data_init(Cob_compile_data* cd, Zinc_string* text, Zinc_error_list* errors)
{
    cd->text = text;
    cd->lookahead = NULL;
    cd->errors = errors;
    cd->group_number = 0;
    cd->pos = 0;
    Zinc_location_init(&cd->loc);
    cd->loc.line = 1;
    cd->loc.col = 1;
}

void Cob_compile_data_create(Cob_compile_data** cd, Zinc_string* text, struct Zinc_error_list* errors)
{
    Zinc_malloc_safe((void**)cd, sizeof(Cob_compile_data));
    Cob_compile_data_init(*cd, text, errors);
}

void Cob_compile_data_destroy(Cob_compile_data* cd)
{
    free(cd->lookahead);
}

void Cob_re_init(Cob_re* re, Zinc_error_list* errors, Cob_ast* root)
{
    re->errors = errors;
    re->root = root;
}

void Cob_re_create(Cob_re** re, Zinc_error_list* errors, Cob_ast* root)
{
    Zinc_malloc_safe((void**)re, sizeof(Cob_re));
    Cob_re_init(*re, errors, root);
}

void Cob_re_destroy(Cob_re* re)
{
    Zinc_error_list_destroy(re->errors);
    free(re->errors);
    Cob_ast_destroy(re->root);
    free(re->root);
}