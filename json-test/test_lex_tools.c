#include "json/lex.h"
#include "json/lex_tools.h"
#include "zinc/unit_test.h"
#include "zinc/input_unicode_string.h"
#include <string.h>

void test_lex_setup(Json_lex_data* jld, char* text)
{
    struct error_list* el = NULL;
    error_list_create(&el);
    jld->el = el;

    Vector* v = NULL;
    VectorCreate(&v, sizeof(char));
    VectorAdd(v, text, strlen(text));

    InputUnicodeString* input_obj = NULL;
    InputUnicodeStringCreate(&input_obj, v);
    jld->input_obj = input_obj;
    jld->input_vtable = &InputUnicodeStringVTable;
}

void test_lex_teardown(Json_lex_data* jld)
{
    error_list_destroy(jld->el);
    free(jld->el);
    InputUnicodeString* input_obj = jld->input_obj;
    Vector* v = input_obj->text;
    VectorDestroy(v);
    free(v);
    free(input_obj);
}
