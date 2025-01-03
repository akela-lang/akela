#include "cobble/compile.h"
#include "cobble/compile_data.h"
#include "cobble/compile_tools.h"
#include <string.h>
#include "zinc/input_unicode_string.h"

Cob_re Cvr_gcov_line_re()
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

    Cob_compile_data* cd = NULL;
    Cob_compile_data_create(&cd, input, input->input_vtable, el);

    Cob_re re = Cob_compile(cd);

    VectorDestroy(text);
    free(text);
    free(input);
    Cob_compile_data_destroy(cd);
    free(cd);

    return re;
}

Cob_re Cvr_gcov_ext_re()
{
    Vector* text = NULL;
    char* s = "\\.gcov$";
    size_t len = strlen(s);

    VectorCreate(&text, 1);
    VectorAdd(text, s, len);

    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, text);

    struct error_list* el = NULL;
    error_list_create(&el);

    Cob_compile_data* cd = NULL;
    Cob_compile_data_create(&cd, input, input->input_vtable, el);

    Cob_re re = Cob_compile(cd);

    VectorDestroy(text);
    free(text);
    free(input);
    Cob_compile_data_destroy(cd);
    free(cd);

    return re;
}

Cob_re Cvr_test_dir_re()
{
    Vector* text = NULL;
    char* s = "\\-test$";
    size_t len = strlen(s);

    VectorCreate(&text, 1);
    VectorAdd(text, s, len);

    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, text);

    struct error_list* el = NULL;
    error_list_create(&el);

    Cob_compile_data* cd = NULL;
    Cob_compile_data_create(&cd, input, input->input_vtable, el);

    Cob_re re = Cob_compile(cd);

    VectorDestroy(text);
    free(text);
    free(input);
    Cob_compile_data_destroy(cd);
    free(cd);

    return re;
}

void Cvr_re_cleanup(Cob_re* cr)
{
    error_list_destroy(cr->el);
    free(cr->el);
    Cob_ast_destroy(cr->root);
    free(cr->root);
}