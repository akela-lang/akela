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
void Parse_test_result_init(Parse_test_result* result);
void parse_teardown(struct comp_unit* cu);
bool parse_setup2(char* line, Parse_test_result *result);
void parse_teardown2(Parse_test_result* result);

#endif
