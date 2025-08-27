#ifndef AKELA_PARSE_TYPES_H
#define AKELA_PARSE_TYPES_H

#include "api.h"
#include <stdbool.h>
#include "zinc/zstring.h"
#include "zinc/string_list.h"
#include "ast.h"
#include "parse_tools.h"

#ifdef __cplusplus
extern "C" {
#endif

AKELA_API Ake_Ast* Ake_parse_prototype(
        Ake_parse_state* ps,
        bool is_function,
        bool is_extern,
        bool is_method,
        bool require_param_name,
        bool* has_id);
AKELA_API Ake_Ast* Ake_parse_dseq(
        Ake_parse_state* ps,
        bool require_param_name,
        bool is_extern,
        bool is_method);
AKELA_API void Ake_declare_params(Ake_symbol_table* st, Ake_Ast* proto);
AKELA_API Ake_Ast* Ake_parse_declaration(
    struct Ake_parse_state* ps,
    bool add_symbol,
    bool is_method,
    bool require_param_name,
    bool is_const);
AKELA_API Ake_Ast* Ake_parse_type(Ake_parse_state* ps);
AKELA_API void Ake_declare_type(Ake_symbol_table* st, Ake_Ast* n, Ake_Ast* id_node, bool is_const);
AKELA_API bool Ake_check_return_type(Ake_parse_state* ps, Ake_Ast* proto, Ake_Ast* stmts_node, Zinc_location* loc);
AKELA_API Ake_Type* Ake_proto2type_use(Ake_parse_state* ps, Ake_Ast* proto);
AKELA_API Ake_TypeParam* Ake_get_function_input_type(Ake_Type* func, int index);
AKELA_API bool Ake_check_input_type(
        Ake_parse_state* ps,
        Ake_Type* func,
        int index,
        Ake_Ast* a);
AKELA_API void Ake_Override_rhs(Ake_Type* type, Ake_Ast* rhs);
AKELA_API bool Ake_check_lvalue(Ake_parse_state* ps, Ake_Ast* n, Zinc_location* loc);
AKELA_API Ake_Type* Ake_StructToType(Ake_Ast* n);

#ifdef __cplusplus
}
#endif

#endif
