#include <string.h>
#include <zinc/error_unit_test.h>
#include <zinc/input_unicode_string.h>

#include "zinc/unit_test.h"
#include "centipede/lex.h"
#include "centipede/lex_data.h"

void test_lex_setup(Cent_lex_data* ld, char* text)
{
    size_t len = strlen(text);

    Vector* v = NULL;
    VectorCreate(&v, 1);
    VectorAdd(v, text, len);

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, v);

    struct Zinc_error_list* errors = NULL;
    Zinc_error_list_create(&errors);

    Cent_lex_data_init(ld, errors, input, input->input_vtable);
}

void test_lex_teardown(Cent_lex_data* ld)
{
    Zinc_input_unicode_string* input = ld->input;
    Vector* v = input->text;
    VectorDestroy(v);
    free(v);

    free(input);

    Zinc_error_list_destroy(ld->errors);
    free(ld->errors);
    Cent_lex_data_destroy(ld);
}