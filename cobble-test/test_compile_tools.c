//
// Created by miguel on 6/6/23.
//

#include <string.h>

#include "cobble/compile_data.h"
#include "zinc/memory.h"
#include "cobble/Ast_node.h"
#include "zinc/input_unicode_string.h"

void setup_compile(struct compile_data** cd, const char* s)
{
    Vector* text = NULL;
    VectorCreate(&text, sizeof(char));
    VectorAdd(text, s, strlen(s));
    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, text);

    struct error_list* el = NULL;
    malloc_safe((void**)&el, sizeof(struct error_list));
    error_list_init(el);

    malloc_safe((void**)cd, sizeof(struct compile_data));
    compile_data_init(*cd, input, input->input_vtable, el);
}

void teardown_compile(struct compile_data* cd, Ast_node* root)
{
    InputUnicodeString* input = cd->input_obj;
    VectorDestroy(input->text);
    error_list_destroy(cd->el);
    free(cd->el);
    free(cd->lookahead);
    Ast_node_destroy(root);
}
