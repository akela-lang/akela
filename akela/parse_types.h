#ifndef _PARSE_TYPES_H
#define _PARSE_TYPES_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/buffer_list.h"

#ifdef __cplusplus
extern "C" {
#endif

AKELA_API struct ast_node* parse_prototype(struct parse_state* ps, bool* has_id, struct location* loc);
AKELA_API struct ast_node* parse_dseq(struct parse_state* ps, struct location* loc);
AKELA_API void declare_params(struct parse_state* ps, struct ast_node* proto);
AKELA_API struct ast_node* parse_declaration(struct parse_state* ps, bool add_symbol, struct location* loc);
AKELA_API struct ast_node* parse_type(struct parse_state* ps, struct location* loc);
AKELA_API void declare_type(struct parse_state* ps, struct ast_node* n, struct ast_node* id_node);
AKELA_API struct ast_node* function2type(struct symbol_table* st, struct ast_node* n);
AKELA_API bool check_return_type(struct parse_state* ps, struct ast_node* fd, struct ast_node* stmts_node, struct location* loc);
AKELA_API void get_function_children(struct ast_node* tu, struct ast_node** input, struct ast_node** output);
AKELA_API struct ast_node* get_function_type(struct symbol* sym);
AKELA_API struct ast_node* get_function_input_type(struct ast_node* tu, int index);
AKELA_API bool check_input_type(struct parse_state* ps, struct ast_node* tu, int index, struct ast_node* a, struct location* loc_expr);
AKELA_API struct ast_node* make_constructor(struct type_def* td);
AKELA_API void Override_rhs(struct ast_node* tu, struct ast_node* rhs);
AKELA_API bool check_lvalue(struct parse_state* ps, struct ast_node* n, struct location* loc);

#ifdef __cplusplus
}
#endif

#endif
