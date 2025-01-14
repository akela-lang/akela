#ifndef _PARSE_TYPES_H
#define _PARSE_TYPES_H

#include "api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/buffer_list.h"
#include "ast.h"

#ifdef __cplusplus
extern "C" {
#endif

AKELA_API Ake_ast* parse_prototype(
        struct parse_state* ps,
        bool is_function,
        bool is_extern,
        bool is_method,
        bool require_param_name,
        bool* has_id);
AKELA_API Ake_ast* parse_dseq(
        struct parse_state* ps,
        bool require_param_name,
        bool is_extern,
        bool is_method);
AKELA_API void declare_params(struct parse_state* ps, Ake_ast* proto, Ake_ast* struct_type);
AKELA_API Ake_ast* parse_declaration(
    struct parse_state* ps,
    bool add_symbol,
    bool is_method,
    bool require_param_name);
AKELA_API Ake_ast* parse_type(struct parse_state* ps);
AKELA_API void declare_type(struct parse_state* ps, Ake_ast* n, Ake_ast* id_node);
AKELA_API bool check_return_type(struct parse_state* ps, Ake_ast* proto, Ake_ast* stmts_node, struct location* loc);
AKELA_API void get_function_children(Type_use* func, Type_use** inputs, Type_use** outputs);
AKELA_API Type_use* proto2type_use(struct symbol_table* st, Ake_ast* proto, Ake_ast* struct_type);
AKELA_API Type_use* get_function_input_type(Type_use* func, int index);
AKELA_API bool check_input_type(
        struct parse_state* ps,
        Type_use* func,
        int index,
        Ake_ast* a);
AKELA_API void Override_rhs(Type_use* tu, Ake_ast* rhs);
AKELA_API bool check_lvalue(struct parse_state* ps, Ake_ast* n, struct location* loc);

#ifdef __cplusplus
}
#endif

#endif
