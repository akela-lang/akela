#ifndef _TEST_PARSE_H
#define _TEST_PARSE_H

#include "zinc/result.h"
#include "akela/lex.h"
#include "akela/token.h"
#include "akela/ast.h"
#include "akela/parse_tools.h"
#include "akela/comp_unit.h"
typedef struct Parse_test_result {
    struct comp_unit cu;
    bool debug;
} Parse_test_result;

bool parse_setup(char* line, struct comp_unit* cu);
void parse_teardown(struct comp_unit* cu);

#endif
