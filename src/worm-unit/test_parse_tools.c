#include "worm/parse_data.h"
#include <string.h>
#include "zinc/input_unicode_string.h"

void test_parse_setup(Worm_parse_data** pd, char const* s)
{
    Zinc_error_list* errors = NULL;
    Zinc_error_list_create(&errors);

    Zinc_vector* text = NULL;
    Zinc_vector_create(&text, sizeof(char));
    Zinc_vector_add(text, s, strlen(s));

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, text);

    Worm_lex_data* ld = NULL;
    Worm_lex_data_create(&ld, input, input->input_vtable, errors);

    Worm_parse_data_create(pd, ld, errors);
}

void test_parse_teardown(Worm_parse_data* pd)
{
    Zinc_error_list* errors = pd->errors;
    Worm_lex_data* ld = pd->ld;
    Zinc_input_unicode_string* input = ld->input;
    Zinc_vector* text = input->text;

    Zinc_error_list_destroy(errors);
    free(errors);

    Zinc_vector_destroy(text);
    free(text);

    free(input);

    Worm_lex_data_destroy(ld);
    free(ld);

    free(pd);
}

