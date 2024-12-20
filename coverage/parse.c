#include "cobble/compile.h"
#include "cobble/compile_data.h"
#include "cobble/compile_tools.h"
#include <string.h>
#include "zinc/input_unicode_string.h"

Cob_re Cov_gov_line_re()
{
    Vector* text = NULL;
    char* s = "\\s*(\\-|#####|\\d+)\\*?:\\s*(\\d+):(Source:)?(.*)";
    size_t len = strlen(s);

    VectorCreate(&text, 1);
    VectorAdd(text, s, len);

    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, text);

    struct error_list* el = NULL;
    error_list_create(&el);

    Compile_data* cd = NULL;
    compile_data_create(&cd, input, input->input_vtable, el);

    Cob_re cr = compile(cd);

    VectorDestroy(text);
    free(text);
    free(input);
    compile_data_destroy(cd);
    free(cd);

    return cr;
}

void Cov_re_cleanup(Cob_re* cr)
{
    error_list_destroy(cr->el);
    Ast_node_destroy(cr->root);
    free(cr->root);
}