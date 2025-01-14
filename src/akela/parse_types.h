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

AKELA_API Ake_ast* Ake_parse_prototype(
        struct Ake_parse_state* ps,
        bool is_function,
        bool is_extern,
        bool is_method,
        bool require_param_name,
        bool* has_id);
AKELA_API Ake_ast* Ake_parse_dseq(
        struct Ake_parse_state* ps,
        bool require_param_name,
        bool is_extern,
        bool is_method);
AKELA_API void Ake_declare_params(struct Ake_parse_state* ps, Ake_ast* proto, Ake_ast* struct_type);
AKELA_API Ake_ast* Ake_parse_declaration(
    struct Ake_parse_state* ps,
    bool add_symbol,
    bool is_method,
    bool require_param_name);
AKELA_API Ake_ast* Ake_parse_type(struct Ake_parse_state* ps);
AKELA_API void Ake_declare_type(struct Ake_parse_state* ps, Ake_ast* n, Ake_ast* id_node);
AKELA_API bool Ake_check_return_type(struct Ake_parse_state* ps, Ake_ast* proto, Ake_ast* stmts_node, struct location* loc);
AKELA_API void Ake_get_function_children(Ake_type_use* func, Ake_type_use** inputs, Ake_type_use** outputs);
AKELA_API Ake_type_use* Ake_proto2type_use(struct Ake_symbol_table* st, Ake_ast* proto, Ake_ast* struct_type);
AKELA_API Ake_type_use* Ake_get_function_input_type(Ake_type_use* func, int index);
AKELA_API bool Ake_check_input_type(
        struct Ake_parse_state* ps,
        Ake_type_use* func,
        int index,
        Ake_ast* a);
AKELA_API void Ake_Override_rhs(Ake_type_use* tu, Ake_ast* rhs);
AKELA_API bool Ake_check_lvalue(struct Ake_parse_state* ps, Ake_ast* n, struct location* loc);

#ifdef __cplusplus
}
#endif

#endif
