#include <string.h>
#include <zinc/input_unicode_string.h>

#include "zinc/unit_test.h"
#include "centipede/lex.h"
#include "centipede/lex_data.h"

void test_lex_element()
{
    test_name(__func__);
    Cent_token* t = NULL;

    char* s = "element";
    size_t len = strlen(s);

    Vector* v = NULL;
    VectorCreate(&v, 1);
    VectorAdd(v, s, len);

    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, v);

    struct error_list* errors = NULL;
    error_list_create(&errors);

    Cent_lex_data* ld = NULL;
    Cent_lex_data_create(&ld, errors, input, input->input_vtable);

    t = lex(ld);
    assert_ptr(t, "ptr 0");
    expect_int_equal(t->type, Cent_token_id, "type 0");
    expect_str(&t->value, "element", "value 0");

    t = lex(ld);
    assert_ptr(t, "ptr 1");
    expect_int_equal(t->type, Cent_token_eof, "type 1");

    VectorDestroy(v);
    free(v);
    free(input);
    error_list_destroy(errors);
    free(errors);
    free(ld);
}

void test_lex()
{
    test_lex_element();
}