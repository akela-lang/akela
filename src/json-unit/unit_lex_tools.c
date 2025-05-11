#include "json/lex.h"
#include "json/lex_tools.h"
#include "zinc/unit_test.h"
#include "zinc/input_unicode_string.h"
#include <string.h>

void Zinc_unit_lex_setup(Json_lex_data* jld, char* text)
{
    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    Zinc_vector* v = NULL;
    Zinc_vector_create(&v, sizeof(char));
    Zinc_vector_add(v, text, strlen(text));

    Zinc_input_unicode_string* input_obj = NULL;
    Zinc_input_unicode_string_create(&input_obj, v);
    Zinc_input_unicode_vtable* input_vtable = &Zinc_input_unicode_string_vtable;

    enum Zinc_result r = Json_lex_data_init(jld, el, input_obj, input_vtable);
    if (r == Zinc_result_error) {
        struct Zinc_location loc;
        Zinc_location_init(&loc);
        Zinc_error_list_set(el, &loc, Zinc_error_message);
    }
}

void Zinc_unit_lex_teardown(Json_lex_data* jld)
{
    Zinc_error_list_destroy(jld->el);
    free(jld->el);
    Zinc_input_unicode_string* input_obj = jld->input_obj;
    Zinc_vector* v = input_obj->text;
    Zinc_vector_destroy(v);
    free(v);
    free(input_obj);
}
