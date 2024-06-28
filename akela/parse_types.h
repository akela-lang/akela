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

AKELA_API Ast_node* parse_prototype(
        struct parse_state* ps,
        bool is_function,
        bool is_extern,
        bool is_method,
        bool require_param_name,
        bool* has_id);
AKELA_API Ast_node* parse_dseq(
        struct parse_state* ps,
        bool require_param_name,
        bool is_extern,
        bool is_method);
AKELA_API void declare_params(struct parse_state* ps, Ast_node* proto, Ast_node* struct_type);
AKELA_API Ast_node* parse_declaration(
    struct parse_state* ps,
    bool add_symbol,
    bool is_method,
    bool require_param_name);
AKELA_API Ast_node* parse_type(struct parse_state* ps);
AKELA_API void declare_type(struct parse_state* ps, Ast_node* n, Ast_node* id_node);
AKELA_API bool check_return_type(struct parse_state* ps, Ast_node* proto, Ast_node* stmts_node, struct location* loc);
AKELA_API void get_function_children(Ast_node* proto, Ast_node** dseq, Ast_node** dret);
AKELA_API Type_use* proto2type(struct symbol_table* st, Ast_node* proto);
AKELA_API Type_use* proto2type_use(struct symbol_table* st, Ast_node* proto);
AKELA_API Type_use* get_function_input_type(Ast_node* tu, int index);
AKELA_API bool check_input_type(struct parse_state* ps, Ast_node* proto, int index, Ast_node* a, struct location* loc_expr);
AKELA_API void Override_rhs(Type_use* tu, Ast_node* rhs);
AKELA_API bool check_lvalue(struct parse_state* ps, Ast_node* n, struct location* loc);

#ifdef __cplusplus
}
#endif

#endif
