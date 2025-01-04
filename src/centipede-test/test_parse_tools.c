#include <string.h>
#include <string.h>

#include "zinc/unit_test.h"
#include "centipede/parse.h"
#include "centipede/parse_data.h"
#include "zinc/input_unicode_string.h"
#include "zinc/error.h"

Cent_comp_unit* test_parse_add_comp_unit(Cent_parse_data* pd, char* name, char* s)
{
    size_t len = strlen(s);

    Vector* v = NULL;
    VectorCreate(&v, sizeof(char));
    VectorAdd(v, s, len);

    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, v);

    struct error_list* errors = NULL;
    error_list_create(&errors);

    Cent_comp_unit* cu = NULL;
    Cent_comp_unit_create(&cu);
    cu->input = input;
    cu->input_vtable = input->input_vtable;
    cu->errors = errors;

    Cent_comp_table_add_str(pd->comp_table, name, cu);
    return cu;
}

void test_parse_setup(Cent_parse_data* pd, char* s)
{
    Cent_parse_data_init(pd);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create(&ct);
    pd->comp_table = ct;

    Cent_comp_unit* cu = test_parse_add_comp_unit(pd, "**string**", s);
    ct->primary = cu;

    Cent_lex_data* ld = NULL;
    Cent_lex_data_create(&ld, cu->errors, cu->input, cu->input_vtable);

    pd->errors = cu->errors;
    pd->ld = ld;
    pd->file_name.p = "**string**";
    pd->file_name.size = strlen(pd->file_name.p);
}

void test_parse_teardown_comp_unit(Cent_comp_unit* cu)
{
    InputUnicodeString* input = cu->input;
    Vector* v = input->text;
    VectorDestroy(v);
    free(v);
    free(input);

    struct error_list* errors = cu->errors;
    error_list_destroy(errors);
    free(errors);

    free(cu);
}

void test_parse_teardown(Cent_parse_data* pd, Cent_parse_result* pr)
{
    Cent_comp_table_map(pd->comp_table, test_parse_teardown_comp_unit);
    Cent_comp_table_destroy(pd->comp_table);
    free(pd->comp_table);

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
}