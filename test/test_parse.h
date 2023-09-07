#ifndef _TEST_PARSE_H
#define _TEST_PARSE_H

#include "zinc/result.h"
#include "akela/scan.h"
#include "akela/token.h"
#include "akela/ast.h"
#include "akela/parse_tools.h"
#include "akela/comp_unit.h"

bool parse_setup(char* line, struct comp_unit* cu);
void parse_teardown2(struct comp_unit* cu);

#endif
