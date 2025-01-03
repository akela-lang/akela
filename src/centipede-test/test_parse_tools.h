#ifndef CENTIPEDE_TEST_PARSE_TOOLS_H
#define CENTIPEDE_TEST_PARSE_TOOLS_H

#include "centipede/parse_data.h"

Cent_comp_unit* test_parse_add_comp_unit(Cent_parse_data* pd, char* name, char* s);
void test_parse_setup(Cent_parse_data* pd, char* s);
void test_parse_teardown(Cent_parse_data* pd, Cent_parse_result* pr);

#endif