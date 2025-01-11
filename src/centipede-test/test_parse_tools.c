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
    size_t len = strlen(s);

    Vector* v = NULL;
    VectorCreate(&v, sizeof(char));
    VectorAdd(v, s, len);

    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, v);

    Cent_module_string_add_module_str_str(ms, name, s);
}

void test_parse_setup(Cent_comp_table** ct, char* s)
{
    Cent_module_string* ms = NULL;
    Cent_module_string_create(&ms);

    Cent_environment* base = Cent_base_create();

    Cent_comp_table_create(ct, ms, ms->vtable, base);

    String_slice file_name;
    file_name.p = "**string**";
    file_name.size = strlen(file_name.p);
    struct buffer name;
    buffer_init(&name);
    buffer_add(&name, file_name.p, file_name.size);
    buffer_finish(&name);

    test_parse_add_comp_unit(ms, name.buf, s);
    Cent_input_data data = Cent_module_find_interface(
        ms,
        ms->vtable,
        &name);
    buffer_destroy(&name);

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
}

void test_parse_teardown_input(Cent_comp_unit* cu)
{
    InputUnicodeString* input = cu->input;
    Vector* v = input->text;
    VectorDestroy(v);
    free(v);
    free(input);
}

void test_parse_teardown(Cent_comp_table* ct)
{
    Cent_comp_table_map(ct, test_parse_teardown_input);
    Cent_comp_table_destroy(ct);
    free(ct);
}