#ifndef _TEST_PARSE_H
#define _TEST_PARSE_H

#include "zinc/result.h"
#include "alba/scan.h"
#include "alba/token.h"
#include "alba/dag.h"

/* dynamic-output ps{} root root{} */
enum result parse_setup(char* line, struct parse_state* ps, struct dag_node** root);

/* dynamic-destroy ps{} */
void parse_teardown(struct parse_state* ps);

/* dynamic-output-none */
struct dag_node* check_stmts(struct dag_node* root, char* message);

#endif
