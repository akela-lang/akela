#include "compile_data.h"
#include "zinc/memory.h"
#include "token.h"

void compile_data_init(
    Compile_data* cd,
    void* input_obj,
    InputUnicodeVTable* input_vtable,
    struct error_list* el)
{
    cd->input_obj = input_obj;
    cd->input_vtable = input_vtable;
    cd->lookahead = NULL;
    cd->el = el;
    cd->group_number = 0;
}

void compile_data_reeate(
    Compile_data** cd,
    void* input_obj,
    InputUnicodeVTable* input_vtable,
    struct error_list* el)
{
    malloc_safe((void**)cd, sizeof(Compile_data));
    compile_data_init(*cd, input_obj, input_vtable, el);
}

void compile_data_destroy(Compile_data* cd)
{
    free(cd->lookahead);
}

void Cob_re_init(Cob_re* re, struct error_list* el, Ast_node* root)
{
    re->el = el;
    re->root = root;
}

void Cob_re_reeate(Cob_re** re, struct error_list* el, Ast_node* root)
{
    malloc_safe((void**)re, sizeof(Cob_re));
    Cob_re_init(*re, el, root);
}

void Cob_re_destroy(Cob_re* re)
{
    error_list_destroy(re->el);
    free(re->el);
    Ast_node_destroy(re->root);
    free(re->root);
}