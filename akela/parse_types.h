#ifndef _PARSE_TYPES_H
#define _PARSE_TYPES_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/buffer_list.h"

/* dynamic-output ps{} root root{} */
AKELA_API bool dseq(struct parse_state* ps, struct ast_node** root, struct location* loc);

/* dynamic-output ps{} root root{} */
AKELA_API bool declaration(struct parse_state* ps, bool add_symbol, struct ast_node** root, struct location* loc);

AKELA_API struct ast_node* parse_type(struct parse_state* ps, struct token_list* id_list, struct location* loc);

AKELA_API struct ast_node* function2type(struct symbol_table* st, struct ast_node* n);

AKELA_API void check_return_type(struct parse_state* ps, struct ast_node* fd, struct ast_node* stmts_node, struct location* loc, bool* valid);

AKELA_API void get_function_children(struct ast_node* tu, struct ast_node** input, struct ast_node** output);

AKELA_API struct ast_node* get_function_type(struct symbol* sym);

AKELA_API struct ast_node* get_function_input_type(struct ast_node* tu, int index);

AKELA_API bool check_input_type(struct parse_state* ps, struct ast_node* tu, int index, struct ast_node* a, struct location* loc_expr);

AKELA_API struct ast_node* make_constructor(struct type_def* td);

#endif
