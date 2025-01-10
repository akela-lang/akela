#ifndef CENTIPEDE_TEST_PARSE_TOOLS_H
#define CENTIPEDE_TEST_PARSE_TOOLS_H

#include "centipede/parse_data.h"
#include "centipede/module_string.h"

void test_parse_add_comp_unit(Cent_module_string* ms, char* name, char* s);
void test_parse_setup(Cent_parse_data* pd, char* s);
void test_parse_teardown(Cent_parse_data* pd, Cent_parse_result* pr);

#endif