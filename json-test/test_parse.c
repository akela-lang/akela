#include <string.h>

#include "zinc/unit_test.h"
#include "json/parse.h"
#include "zinc/input_unicode_string.h"
#include "json/dom.h"

void test_parse_string()
{
    test_name(__func__);

    char s[] = "\"hello\"";
    Vector* v = NULL;
    VectorCreate(&v, sizeof(char));
    VectorAdd(v, s, strlen(s));

    InputUnicodeString* input;
    InputUnicodeStringCreate(&input, v);

    struct error_list* el = NULL;
    error_list_create(&el);

    Json_lex_data* ld = NULL;
    Json_lex_data_create(&ld, el, input, input->input_vtable);

    Json_parse_data* pd = NULL;
    Json_parse_data_create(&pd, el, ld);

    Json_dom* dom = Json_parse(pd);
    bool valid = !dom->has_error && !el->head;
    expect_true(valid, "valid");

    VectorDestroy(v);
    free(v);
    free(input);
    error_list_destroy(el);
    free(el);
    free(ld);
    free(pd);
}

void test_parse()
{
    test_parse_string();
}