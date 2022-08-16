#ifndef _PARSE_TYPES_H
#define _PARSE_TYPES_H

#include "alba_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"

/* dynamic-output ps{} root root{} */
ALBA_API bool dseq(struct parse_state* ps, struct ast_node** root);

/* dynamic-output ps{} root root{} */
ALBA_API bool declaration(struct parse_state* ps, struct ast_node** root);

ALBA_API bool type(struct parse_state* ps, struct token* id, struct ast_node** root);

ALBA_API struct ast_node* af2etype(struct ast_node* n);

ALBA_API bool type_match(struct symbol_table* st, struct ast_node* a, struct ast_node* b);

ALBA_API bool binary_arithmetic_check(struct parse_state* ps, struct ast_node* left, struct ast_node* a, struct ast_node* b, struct location* loc_a, struct location* loc_op, struct location* loc_b, char* op_name, struct ast_node** ret);

#endif
