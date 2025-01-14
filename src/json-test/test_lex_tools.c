#include "json/lex.h"
#include "json/lex_tools.h"
#include "zinc/unit_test.h"
#include "zinc/input_unicode_string.h"
#include <string.h>

void test_lex_setup(Json_lex_data* jld, char* text)
{
    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    Vector* v = NULL;
    VectorCreate(&v, sizeof(char));
    VectorAdd(v, text, strlen(text));

    InputUnicodeString* input_obj = NULL;
    InputUnicodeStringCreate(&input_obj, v);
    InputUnicodeVTable* input_vtable = &InputUnicodeStringVTable;

    enum result r = Json_lex_data_init(jld, el, input_obj, input_vtable);
    if (r == result_error) {
        struct Zinc_location loc;
        Zinc_location_init(&loc);
        Zinc_error_list_set(el, &loc, error_message);
    }
}

void test_lex_teardown(Json_lex_data* jld)
{
    Zinc_error_list_destroy(jld->el);
    free(jld->el);
    InputUnicodeString* input_obj = jld->input_obj;
    Vector* v = input_obj->text;
    VectorDestroy(v);
    free(v);
    free(input_obj);
}
