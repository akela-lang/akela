#include <string.h>
#include <akela/comp_unit.h>
#include "zinc/unit_test.h"
#include "centipede/parse_data.h"
#include "zinc/input_unicode_string.h"
#include "centipede/module_finder_string.h"
#include "centipede/comp_table.h"
#include <assert.h>
#include <centipede/base.h>

void test_parse_add_comp_unit(Cent_comp_table* ct, char* name, char* s)
{
    assert(ct->module_finder_vtable == &Cent_module_finder_string_vtable);
    Cent_module_finder_string_add_module_str_str(ct->module_finder_obj, name, s);
}

void test_parse_setup(Cent_comp_table** ct, char* s)
{
    Cent_module_finder_string* ms = NULL;
    Cent_module_finder_string_create(&ms);

    Cent_comp_table_create(ct, ms, ms->vtable);

    Zinc_string_slice file_name;
    file_name.p = "**string**";
    file_name.size = strlen(file_name.p);
    Zinc_string name;
    Zinc_string_init(&name);
    Zinc_string_add(&name, file_name.p, file_name.size);
    Zinc_string_finish(&name);

    test_parse_add_comp_unit(*ct, name.buf, s);
    Cent_input_data data = Cent_module_finder_find(
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
        (*ct)->base);
    cu->ct = *ct;
    cu->pd.ct = *ct;
    (*ct)->primary = cu;
    Cent_comp_table_add(*ct, &name, cu);
    Zinc_string_destroy(&name);
}

void test_parse_teardown(Cent_comp_table* ct)
{
    Cent_comp_table_destroy(ct);
    free(ct);
}