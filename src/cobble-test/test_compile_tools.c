#include <string.h>
#include "cobble/compile_data.h"
#include "zinc/memory.h"
#include "cobble/ast.h"
#include "zinc/input_unicode_string.h"

void setup_compile(Cob_compile_data** cd, const char* s)
{
    Vector* text = NULL;
    VectorCreate(&text, sizeof(char));
    VectorAdd(text, s, strlen(s));
    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, text);

    struct Zinc_error_list* el = NULL;
    malloc_safe((void**)&el, sizeof(struct Zinc_error_list));
    Zinc_error_list_init(el);

    malloc_safe((void**)cd, sizeof(struct Cob_compile_data));
    Cob_compile_data_init(*cd, input, input->input_vtable, el);
}

void teardown_compile(Cob_compile_data* cd, Cob_re* result)
{
    Zinc_input_unicode_string* input = cd->input_obj;
    VectorDestroy(input->text);
    free(input->text);
    free(input);
    free(cd->lookahead);
    free(cd);
    Cob_re_destroy(result);
}
