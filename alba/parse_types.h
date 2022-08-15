#ifndef _PARSE_TYPES_H
#define _PARSE_TYPES_H

#include <stdbool.h>
#include "zinc/buffer.h"

/* dynamic-output ps{} root root{} */
bool dseq(struct parse_state* ps, struct dag_node** root);

/* dynamic-output ps{} root root{} */
bool declaration(struct parse_state* ps, struct dag_node** root);

bool type(struct parse_state* ps, struct token* id, struct dag_node** root);

struct dag_node* af2etype(struct dag_node* n);

bool type_match(struct symbol_table* st, struct dag_node* a, struct dag_node* b);

bool binary_arithmetic_check(struct parse_state* ps, struct dag_node* left, struct dag_node* a, struct dag_node* b, struct location* loc_a, struct location* loc_op, struct location* loc_b, char* op_name, struct dag_node** ret);

#endif
