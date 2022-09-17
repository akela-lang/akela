#ifndef _TEST_PARSE_H
#define _TEST_PARSE_H

#include "zinc/result.h"
#include "alba/scan.h"
#include "alba/token.h"
#include "alba/ast.h"
#include "alba/parse_tools.h"

/* dynamic-output ps{} root root{} */
enum result parse_setup(char* line, struct parse_state* ps, struct ast_node** root);

/* dynamic-destroy ps{} */
void parse_teardown(struct parse_state* ps);

bool parse_setup2(char* line, struct comp_unit* cu);
void parse_teardown2(struct comp_unit* cu);

#endif
