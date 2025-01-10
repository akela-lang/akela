#include <string.h>
#include <string.h>
#include <akela/comp_unit.h>

#include "zinc/unit_test.h"
#include "centipede/parse.h"
#include "centipede/parse_data.h"
#include "zinc/input_unicode_string.h"
#include "zinc/error.h"
#include "centipede/module_string.h"
#include "centipede/comp_table.h"

void test_parse_add_comp_unit(Cent_parse_data* pd, char* name, char* s)
{
    size_t len = strlen(s);

    Vector* v = NULL;
    VectorCreate(&v, sizeof(char));
    VectorAdd(v, s, len);

    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, v);

    struct error_list* errors = NULL;
    error_list_create(&errors);

    Cent_module_string_add_module_str_str(pd->module_obj, name, s);
}

void test_parse_setup(Cent_parse_data* pd, char* s)
{
    Cent_parse_data_init(pd);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create(&ct);
    pd->comp_table = ct;

    Cent_module_string* ms = NULL;
    Cent_module_string_create(&ms);
    pd->module_obj = ms;
    pd->module_vtable = ms->vtable;
    ct->module_finder_obj = ms;
    ct->module_finder_vtable = ms->vtable;

    String_slice name_slice;
    name_slice.p = "**string**";
    name_slice.size = strlen(name_slice.p);
    struct buffer name;
    buffer_init(&name);
    buffer_add(&name, name_slice.p, name_slice.size);
    buffer_finish(&name);
    test_parse_add_comp_unit(pd, name.buf, s);
    Cent_comp_unit* cu = Cent_module_find_interface(
        pd->module_obj,
        pd->module_vtable,
        &name);
    buffer_destroy(&name);

    Cent_lex_data* ld = NULL;
    Cent_lex_data_create(&ld, cu->errors, cu->input, cu->input_vtable);

    pd->errors = cu->errors;
    pd->ld = ld;
    pd->file_name = name_slice;
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

    Cent_comp_unit_destroy(cu);
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

    Cent_module_string_destroy(pd->module_obj);
    free(pd->module_obj);

    free(pr->root);
    Cent_environment_destroy(pr->base);
    free(pr->base);
}