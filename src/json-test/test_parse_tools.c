#include "json/parse_tools.h"
#include <string.h>
#include "zinc/input_unicode_string.h"

void test_parse_setup(Json_parse_data* pd, char* text)
{
    Vector* v = NULL;
    VectorCreate(&v, sizeof(char));
    VectorAdd(v, text, strlen(text));

    Zinc_input_unicode_string* input;
    Zinc_input_unicode_string_create(&input, v);

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    Json_lex_data* ld = NULL;
    Json_lex_data_create(&ld, el, input, input->input_vtable);

    Json_parse_data_init(pd, el, ld);
}

void test_parse_destroy(Json_parse_data* pd)
{
    if (pd->lookahead) {
        Json_token_destroy(pd->lookahead);
        free(pd->lookahead);
    }
    Json_lex_data* ld = pd->ld;
    struct Zinc_error_list* el = pd->el;
    Zinc_input_unicode_string* input = ld->input_obj;
    Vector* v = input->text;
    VectorDestroy(v);
    free(v);
    free(input);
    Zinc_error_list_destroy(el);
    free(el);
    free(ld);
}