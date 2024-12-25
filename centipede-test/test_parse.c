#include <string.h>
#include <string.h>

#include "zinc/unit_test.h"
#include "centipede/parse.h"
#include "centipede/parse_data.h"
#include "zinc/input_unicode_string.h"
#include "zinc/error.h"

void test_parse_element()
{
    test_name(__func__);

    Cent_parse_data pd;

    char* s = "element Test_suite\n"
        "    properties\n"
        "        name: String `required`\n"
        "        solo: Bool      # only run this suite\n"
        "        mute: Bool      # exclude this suite from running\n"
        "    end\n"
        "    children\n"
        "        Test\n"
        "    end\n"
        "end\n";
    size_t len = strlen(s);

    Vector* v = NULL;
    VectorCreate(&v, sizeof(char));
    VectorAdd(v, s, len);

    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, v);

    struct error_list* errors = NULL;
    error_list_create(&errors);

    Cent_lex_data* ld = NULL;
    Cent_lex_data_create(&ld, errors, input, input->input_vtable);

    Cent_parse_data_init(&pd);
    pd.errors = errors;
    pd.top = NULL;
    pd.ld = ld;

    Cent_parse_result pr = Cent_parse(&pd);

    assert_ptr(pr.root, "ptr pr.root");
    expect_int_equal(pr.root->type, Cent_ast_type_stmts, "type pr.root");
}

void test_parse()
{
    test_parse_element();
}