#ifndef _TEST_PARSE_H
#define _TEST_PARSE_H

#include "zinc/result.h"
#include "alba/scan.h"
#include "alba/token.h"
#include "alba/ast.h"
#include "alba/parse_tools.h"

bool parse_setup2(char* line, struct comp_unit* cu);
void parse_teardown2(struct comp_unit* cu);

#endif
