#include <string.h>
#include "cobble/compile_data.h"
#include "zinc/memory.h"
#include "cobble/Ast_node.h"
#include "zinc/input_unicode_string.h"

void setup_compile(Compile_data** cd, const char* s)
{
    Vector* text = NULL;
    VectorCreate(&text, sizeof(char));
    VectorAdd(text, s, strlen(s));
    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, text);

    struct error_list* el = NULL;
    malloc_safe((void**)&el, sizeof(struct error_list));
    error_list_init(el);

    malloc_safe((void**)cd, sizeof(struct Compile_data));
    compile_data_init(*cd, input, input->input_vtable, el);
}

void teardown_compile(Compile_data* cd, Cob_compile_result* result)
{
    InputUnicodeString* input = cd->input_obj;
    VectorDestroy(input->text);
    free(input->text);
    free(input);
    free(cd->lookahead);
    free(cd);
    Cob_compile_result_destroy(result);
}
