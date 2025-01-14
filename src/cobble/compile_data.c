#include "compile_data.h"
#include "zinc/memory.h"
#include "token.h"

void Cob_compile_data_init(
    Cob_compile_data* cd,
    void* input_obj,
    InputUnicodeVTable* input_vtable,
    struct Zinc_error_list* el)
{
    cd->input_obj = input_obj;
    cd->input_vtable = input_vtable;
    cd->lookahead = NULL;
    cd->el = el;
    cd->group_number = 0;
}

void Cob_compile_data_create(
    Cob_compile_data** cd,
    void* input_obj,
    InputUnicodeVTable* input_vtable,
    struct Zinc_error_list* el)
{
    malloc_safe((void**)cd, sizeof(Cob_compile_data));
    Cob_compile_data_init(*cd, input_obj, input_vtable, el);
}

void Cob_compile_data_destroy(Cob_compile_data* cd)
{
    free(cd->lookahead);
}

void Cob_re_init(Cob_re* re, struct Zinc_error_list* el, Cob_ast* root)
{
    re->el = el;
    re->root = root;
}

void Cob_re_reeate(Cob_re** re, struct Zinc_error_list* el, Cob_ast* root)
{
    malloc_safe((void**)re, sizeof(Cob_re));
    Cob_re_init(*re, el, root);
}

void Cob_re_destroy(Cob_re* re)
{
    Zinc_error_list_destroy(re->el);
    free(re->el);
    Cob_ast_destroy(re->root);
    free(re->root);
}