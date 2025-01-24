#include <string.h>
#include <worm/lex_data.h>
#include <zinc/error.h>
#include <zinc/input_unicode_string.h>
#include <zinc/vector.h>

void test_setup(Worm_lex_data** ld, char* s)
{
    Zinc_error_list* errors = NULL;
    Zinc_error_list_create(&errors);

    Zinc_vector* text = NULL;
    Zinc_vector_create(&text, sizeof(char));
    Zinc_vector_add(text, s, strlen(s));

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, text);

    Worm_lex_data_create(ld, input, input->input_vtable, errors);
}

void test_teardown(Worm_lex_data* ld)
{
    Zinc_error_list* errors = ld->errors;
    Zinc_error_list_destroy(errors);
    free(errors);

    Zinc_input_unicode_string* input = ld->input;
    Zinc_vector* text = input->text;

    Zinc_vector_destroy(text);
    free(text);
    free(input);
}