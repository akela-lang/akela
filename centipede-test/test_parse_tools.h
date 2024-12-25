#ifndef CENTIPEDE_TEST_PARSE_TOOLS_H
#define CENTIPEDE_TEST_PARSE_TOOLS_H

#include "centipede/parse_data.h"

void test_parse_setup(Cent_parse_data* pd, char* s);
void test_parse_teardown(Cent_parse_data* pd, Cent_parse_result* pr);

#endif