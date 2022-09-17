#ifndef _PARSE_TYPES_H
#define _PARSE_TYPES_H

#include "alba_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"

/* dynamic-output ps{} root root{} */
ALBA_API bool dseq(struct parse_state* ps, struct ast_node** root, struct location* loc);

/* dynamic-output ps{} root root{} */
ALBA_API bool declaration(struct parse_state* ps, bool add_symbol, struct ast_node** root, struct location* loc);

ALBA_API bool type(struct parse_state* ps, struct token* id, struct ast_node** root, struct location* loc);

ALBA_API struct ast_node* function2type(struct symbol_table* st, struct ast_node* n);

ALBA_API void check_return_type(struct parse_state* ps, struct ast_node* fd, struct ast_node* stmts_node, struct location* loc, bool* valid);

ALBA_API void get_function_children(struct ast_node* tu, struct ast_node** input, struct ast_node** output);

ALBA_API struct ast_node* get_function_type(struct symbol* sym);

ALBA_API struct ast_node* get_function_input_type(struct ast_node* tu, int index);

ALBA_API bool check_input_type(struct parse_state* ps, struct ast_node* tu, int index, struct ast_node* a, struct location* loc_expr);

ALBA_API struct ast_node* make_constructor(struct type_def* td);

#endif
