#ifndef WORM_TEST_TOOLS_H
#define WORM_TEST_TOOLS_H

#include "worm/lex_data.h"

void test_lex_setup(Worm_lex_data** ld, char* s);
void test_lex_teardown(Worm_lex_data* ld);

#endif
