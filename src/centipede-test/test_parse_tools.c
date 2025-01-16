#include <string.h>
#include <akela/comp_unit.h>
#include "zinc/unit_test.h"
#include "centipede/parse_data.h"
#include "zinc/input_unicode_string.h"
#include "centipede/module_string.h"
#include "centipede/comp_table.h"
#include <assert.h>
#include <centipede/base.h>

void test_parse_add_comp_unit(Cent_module_string* ms, char* name, char* s)
{
    Cent_module_string_add_module_str_str(ms, name, s);
}

void test_parse_setup(Cent_comp_table** ct, char* s)
{
    Cent_module_string* ms = NULL;
    Cent_module_string_create(&ms);

    Cent_environment* base = Cent_base_create();

    Cent_comp_table_create(ct, ms, ms->vtable, base);

    Zinc_string_slice file_name;
    file_name.p = "**string**";
    file_name.size = strlen(file_name.p);
    struct Zinc_string name;
    Zinc_string_init(&name);
    Zinc_string_add(&name, file_name.p, file_name.size);
    Zinc_string_finish(&name);

    test_parse_add_comp_unit(ms, name.buf, s);
    Cent_input_data data = Cent_module_find_interface(
        ms,
        ms->vtable,
        &name);
    assert(data.input);
    assert(data.input_vtable);

    Cent_comp_unit* cu = NULL;
    Cent_comp_unit_create(
        &cu,
        data.input,
        data.input_vtable,
        file_name,
        ms,
        ms->vtable,
        base);
    cu->ct = *ct;
    cu->pd.ct = *ct;
    (*ct)->primary = cu;
    Cent_comp_table_add(*ct, &name, cu);
    Zinc_string_destroy(&name);
}

void test_parse_teardown_input(Cent_comp_unit* cu)
{
    Zinc_input_unicode_string* input = cu->input;
    Zinc_vector* v = input->text;
    Zinc_vector_destroy(v);
    free(v);
    free(input);
}

void test_parse_teardown_module(Cent_comp_table* ct)
{
    Cent_module_string* ms = ct->module_finder_obj;
    Cent_module_string_destroy(ms);
    free(ms);
}

void test_parse_teardown(Cent_comp_table* ct)
{
    Cent_value_destroy_setup();

    Cent_comp_table_map(ct, test_parse_teardown_input);
    Cent_comp_table_destroy(ct);
    test_parse_teardown_module(ct);
    free(ct);

    Cent_value_destroy_teardown();
}