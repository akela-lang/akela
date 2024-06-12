#ifndef _PARSE_TYPES_H
#define _PARSE_TYPES_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/buffer_list.h"
#include "ast.h"

#ifdef __cplusplus
extern "C" {
#endif

AKELA_API Ast_node* parse_prototype(struct parse_state* ps, bool* has_id, struct location* loc);
AKELA_API Ast_node* parse_dseq(struct parse_state* ps, struct location* loc);
AKELA_API void declare_params(struct parse_state* ps, Ast_node* proto);
AKELA_API Ast_node* parse_declaration(struct parse_state* ps, bool add_symbol, struct location* loc);
AKELA_API Ast_node* parse_type(struct parse_state* ps, struct location* loc);
AKELA_API void declare_type(struct parse_state* ps, Ast_node* n, Ast_node* id_node);
AKELA_API Ast_node* function2type(struct symbol_table* st, Ast_node* n);
AKELA_API bool check_return_type(struct parse_state* ps, Ast_node* fd, Ast_node* stmts_node, struct location* loc);
AKELA_API void get_function_children(Ast_node* tu, Ast_node** input, Ast_node** output);
AKELA_API Ast_node* get_function_type(struct symbol* sym);
AKELA_API Ast_node* get_function_input_type(Ast_node* tu, int index);
AKELA_API bool check_input_type(struct parse_state* ps, Ast_node* tu, int index, Ast_node* a, struct location* loc_expr);
AKELA_API Ast_node* make_constructor(struct type_def* td);
AKELA_API void Override_rhs(Ast_node* tu, Ast_node* rhs);
AKELA_API bool check_lvalue(struct parse_state* ps, Ast_node* n, struct location* loc);

#ifdef __cplusplus
}
#endif

#endif
