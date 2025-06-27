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

AKELA_API Ake_ast* Ake_parse_prototype(
        Ake_parse_state* ps,
        bool is_function,
        bool is_extern,
        bool is_method,
        bool require_param_name,
        bool* has_id);
AKELA_API Ake_ast* Ake_parse_dseq(
        Ake_parse_state* ps,
        bool require_param_name,
        bool is_extern,
        bool is_method);
AKELA_API void Ake_declare_params(Ake_parse_state* ps, Ake_ast* proto, Ake_TypeDef* struct_type);
AKELA_API Ake_ast* Ake_parse_declaration(
    struct Ake_parse_state* ps,
    bool add_symbol,
    bool is_method,
    bool require_param_name);
AKELA_API Ake_ast* Ake_parse_type(Ake_parse_state* ps);
AKELA_API void Ake_declare_type(Ake_parse_state* ps, Ake_ast* n, Ake_ast* id_node);
AKELA_API bool Ake_check_return_type(Ake_parse_state* ps, Ake_ast* proto, Ake_ast* stmts_node, Zinc_location* loc);
AKELA_API Ake_TypeDef* Ake_proto2type_use(Ake_parse_state* ps, Ake_ast* proto, Ake_TypeDef* struct_type);
AKELA_API Ake_TypeParam* Ake_get_function_input_type(Ake_TypeDef* func, int index);
AKELA_API bool Ake_check_input_type(
        Ake_parse_state* ps,
        Ake_TypeDef* func,
        int index,
        Ake_ast* a);
AKELA_API void Ake_Override_rhs(Ake_TypeDef* tu, Ake_ast* rhs);
AKELA_API bool Ake_check_lvalue(Ake_parse_state* ps, Ake_ast* n, Zinc_location* loc);
AKELA_API Ake_TypeDef* Ake_StructToType(Ake_ast* n);

#ifdef __cplusplus
}
#endif

#endif
