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