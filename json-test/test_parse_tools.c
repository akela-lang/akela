#include "json/parse_tools.h"
#include <string.h>
#include "zinc/input_unicode_string.h"

void test_parse_setup(Json_parse_data* pd, char* text)
{
    Vector* v = NULL;
    VectorCreate(&v, sizeof(char));
    VectorAdd(v, text, strlen(text));

    InputUnicodeString* input;
    InputUnicodeStringCreate(&input, v);

    struct error_list* el = NULL;
    error_list_create(&el);

    Json_lex_data* ld = NULL;
    Json_lex_data_create(&ld, el, input, input->input_vtable);

    Json_parse_data_init(pd, el, ld);
}

void test_parse_destroy(Json_parse_data* pd)
{
    Json_lex_data* ld = pd->ld;
    struct error_list* el = pd->el;
    InputUnicodeString* input = ld->input_obj;
    Vector* v = input->text;
    VectorDestroy(v);
    free(v);
    free(input);
    error_list_destroy(el);
    free(el);
    free(ld);
}