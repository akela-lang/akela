#ifndef CENTIPEDE_TEST_PARSE_TOOLS_H
#define CENTIPEDE_TEST_PARSE_TOOLS_H

#include "centipede/parse_data.h"
#include "centipede/module_finder_string.h"
#include "centipede/comp_table.h"

void test_parse_add_comp_unit(Cent_module_finder_string* ms, char* name, char* s);
void test_parse_setup(Cent_comp_table** ct, char* s);
void test_parse_teardown(Cent_comp_table* ct);

#endif