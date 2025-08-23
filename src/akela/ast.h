#ifndef AKELA_AST_H
#define AKELA_AST_H

#include "forward.h"
#include "api.h"
#include "forward_declare.h"
#include <stdbool.h>
#include "zinc/zstring.h"
#include "zinc/result.h"
#include "token.h"
#include "zinc/vector.h"
#include "type.h"
#include "zinc/list.h"
#include "environment.h"

typedef enum Ake_AstKind {
	AKE_AST_NONE,
	Ake_ast_type_id,
	Ake_ast_type_sign,
	Ake_ast_type_number,
	Ake_ast_type_string,
	Ake_ast_type_assign,
	Ake_ast_type_plus,
	Ake_ast_type_minus,
	Ake_ast_type_mult,
	Ake_ast_type_divide,
	Ake_ast_type_stmts,
	Ake_ast_type_function,
	Ake_ast_type_dseq,
	Ake_ast_type_dret,
	Ake_ast_type_call,
	Ake_ast_type_cseq,
	Ake_ast_type_if,
	Ake_ast_type_conditional_branch,
	Ake_ast_type_default_branch,
	Ake_ast_type_equality,
	Ake_ast_type_not_equal,
	Ake_ast_type_less_than,
	Ake_ast_type_less_than_or_equal,
	Ake_ast_type_greater_than,
	Ake_ast_type_greater_than_or_equal,
	Ake_ast_type_not,
	Ake_ast_type_and,
	Ake_ast_type_or,
	Ake_ast_type_while,
	Ake_ast_type_for_range,
	Ake_ast_type_for_iteration,
	Ake_ast_type_declaration,
	Ake_ast_type_array_literal,
	Ake_ast_type_array_subscript,
	Ake_ast_type_boolean,
	Ake_ast_type_parenthesis,
	Ake_ast_type_type,
	Ake_ast_type_power,
	Ake_ast_type_type_pool,
	Ake_ast_type_dot,
	Ake_ast_type_struct,
	Ake_ast_type_return,
    Ake_ast_type_eseq,
    Ake_ast_type_prototype,
    Ake_ast_type_extern,
    Ake_ast_type_struct_literal,
    Ake_ast_type_struct_literal_field,
    Ake_ast_type_ellipsis,
    Ake_ast_type_impl,
    Ake_ast_type_self,
	Ake_ast_type_const,
	Ake_ast_type_var,
	Ake_ast_type_count		/* keep at end */
} Ake_AstKind;

static char const* Ast_type_name(Ake_AstKind kind)
{
	char const* name[Ake_ast_type_count];
    name[AKE_AST_NONE] = "none";
    name[Ake_ast_type_id] = "id";
    name[Ake_ast_type_sign] = "sign";
    name[Ake_ast_type_number] = "number";
    name[Ake_ast_type_string] = "string";
    name[Ake_ast_type_assign] = "assign";
    name[Ake_ast_type_plus] = "plus";
    name[Ake_ast_type_minus] = "minus";
    name[Ake_ast_type_mult] = "mult";
    name[Ake_ast_type_divide] = "divide";
    name[Ake_ast_type_stmts] = "stmts";
    name[Ake_ast_type_function] = "function";
    name[Ake_ast_type_dseq] = "dseq";
    name[Ake_ast_type_dret] = "dret";
    name[Ake_ast_type_call] = "call";
    name[Ake_ast_type_cseq] = "cseq";
    name[Ake_ast_type_if] = "if";
    name[Ake_ast_type_conditional_branch] = "conditional-branch";
    name[Ake_ast_type_default_branch] = "default-branch";
    name[Ake_ast_type_equality] = "equality";
    name[Ake_ast_type_not_equal] = "not-equal";
    name[Ake_ast_type_less_than] = "less-than";
    name[Ake_ast_type_less_than_or_equal] = "less-than-or-equal";
    name[Ake_ast_type_greater_than] = "greater-than";
    name[Ake_ast_type_greater_than_or_equal] = "greater-than-or-equal";
    name[Ake_ast_type_not] = "not";
    name[Ake_ast_type_and] = "and";
    name[Ake_ast_type_or] = "or";
    name[Ake_ast_type_while] = "while";
    name[Ake_ast_type_for_range] = "for-range";
    name[Ake_ast_type_for_iteration] = "for-iteration";
    name[Ake_ast_type_declaration] = "declaration";
    name[Ake_ast_type_array_literal] = "array-literal";
    name[Ake_ast_type_array_subscript] = "array-subscript";
    name[Ake_ast_type_boolean] = "boolean";
    name[Ake_ast_type_parenthesis] = "parenthesis";
    name[Ake_ast_type_type] = "type";
    name[Ake_ast_type_power] = "power";
    name[Ake_ast_type_type_pool] = "type-pool";
    name[Ake_ast_type_dot] = "dot";
    name[Ake_ast_type_struct] = "struct";
    name[Ake_ast_type_return] = "return";
    name[Ake_ast_type_eseq] = "eseq";
    name[Ake_ast_type_prototype] = "prototype";
    name[Ake_ast_type_extern] = "extern";
    name[Ake_ast_type_struct_literal] = "struct-literal";
    name[Ake_ast_type_struct_literal_field] = "struct-literal-field";
    name[Ake_ast_type_ellipsis] = "ellipsis";
    name[Ake_ast_type_impl] = "impl";
    name[Ake_ast_type_self] = "self";
	name[Ake_ast_type_const] = "const";
	name[Ake_ast_type_var] = "var";

	if (kind >= 0 && kind < Ake_ast_type_count) {
		return name[kind];
	}

	return "Invalid Ake_ast_type";
}

typedef struct Ake_Ast {
	Ake_AstKind kind;
	Zinc_string id_value;
	Zinc_string struct_value;
	Zinc_string number_value;
	Zinc_string string_value;
	Zinc_string boolean_value;
	Ake_Type* type;
    Zinc_location loc;
	Ake_Environment* env;
	Ake_Ast* next;
	Ake_Ast* prev;
	Ake_Ast* head;
	Ake_Ast* tail;
	Ake_Ast* parent;
	bool has_error;
} Ake_Ast;

#ifdef __cplusplus
extern "C" {
#endif

void Ake_AstCreate(Ake_Ast** n);
AKELA_API void Ake_AstDestroy(Ake_Ast* n);
AKELA_API void Ake_AstInit(Ake_Ast* n);
AKELA_API void Ake_AstAdd(Ake_Ast* p, Ake_Ast* c);
AKELA_API Ake_Ast* Ake_AstGet(Ake_Ast* p, size_t pos);
AKELA_API void Ake_AstCopy(Ake_Ast* src, Ake_Ast* dest);
AKELA_API Ake_Ast* Ake_AstClone(Ake_Ast* n);
AKELA_API bool Ake_AstMatch(Ake_Ast* a, Ake_Ast* b);
AKELA_API size_t Ake_AstCountChildren(Ake_Ast* n);

#ifdef __cplusplus
}
#endif

#endif
