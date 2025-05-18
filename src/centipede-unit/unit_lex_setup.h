#ifndef CENTIPEDE_TEST_LEX_SETUP_H
#define CENTIPEDE_TEST_LEX_SETUP_H

#include "centipede/lex_data.h"

void CentUnit_lex_setup(Cent_lex_data* ld, char* text);
void CentUnit_lex_teardown(Cent_lex_data* ld);

#endif