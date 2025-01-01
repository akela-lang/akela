#include <string.h>
#include <string.h>

#include "zinc/unit_test.h"
#include "centipede/parse.h"
#include "centipede/parse_data.h"
#include "zinc/input_unicode_string.h"
#include "zinc/error.h"

void test_parse_setup(Cent_parse_data* pd, char* s)
{
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

    Cent_parse_data_init(pd);
    pd->errors = errors;
    pd->ld = ld;
    pd->file_name.p = "**string**";
    pd->file_name.size = strlen(pd->file_name.p);
}

void test_parse_teardown(Cent_parse_data* pd, Cent_parse_result* pr)
{
    InputUnicodeString* input = pd->ld->input;
    Vector* v = input->text;
    VectorDestroy(v);
    free(v);
    free(input);
    Cent_lex_data* ld = pd->ld;
    Cent_lex_data_destroy(ld);
    free(ld);

    bool need_teardown = Cent_value_destroy_setup();
    Cent_ast_destroy(pr->root);
    if (need_teardown) {
        Cent_value_destroy_teardown();
    }

    free(pr->root);
    Cent_environment_destroy(pr->base);
    free(pr->base);
    struct error_list* errors = pd->errors;
    error_list_destroy(errors);
    free(errors);
}