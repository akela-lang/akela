#ifndef _TEST_PARSE_H
#define _TEST_PARSE_H

#include "alba/result.h"
#include "alba/allocator.h"
#include "alba/scan.h"
#include "alba/token.h"
#include "alba/dag.h"

enum result parse_setup(struct allocator* al, char* line, struct token_state* ts, struct dag_node** root);
void parse_teardown(struct allocator* al, struct token_state* ts);
struct dag_node* check_stmts(struct dag_node* root, char* message);

#endif
