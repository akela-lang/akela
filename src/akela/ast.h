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
	AKE_AST_ID,
	AKE_AST_SIGN,
	AKE_AST_NUMBER,
	AKE_AST_STRING,
	AKE_AST_ASSIGN,
	AKE_AST_PLUS,
	AKE_AST_MINUS,
	AKE_AST_MULT,
	AKE_AST_DIVIDE,
	AKE_AST_STMTS,
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
    Ake_ast_type_prototype,
    Ake_ast_type_extern,
    Ake_ast_type_struct_literal,
    Ake_ast_type_struct_literal_field,
    Ake_ast_type_ellipsis,
    Ake_ast_type_impl,
	Ake_ast_type_const,
	Ake_ast_type_var,
	AKE_AST_COUNT		/* keep at end */
} Ake_AstKind;

static char const* Ast_type_name(Ake_AstKind kind)
{
	char const* name[AKE_AST_COUNT];
    name[AKE_AST_NONE] = "none";
    name[AKE_AST_ID] = "id";
    name[AKE_AST_SIGN] = "sign";
    name[AKE_AST_NUMBER] = "number";
    name[AKE_AST_STRING] = "string";
    name[AKE_AST_ASSIGN] = "assign";
    name[AKE_AST_PLUS] = "plus";
    name[AKE_AST_MINUS] = "minus";
    name[AKE_AST_MULT] = "mult";
    name[AKE_AST_DIVIDE] = "divide";
    name[AKE_AST_STMTS] = "stmts";
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
    name[Ake_ast_type_prototype] = "prototype";
    name[Ake_ast_type_extern] = "extern";
    name[Ake_ast_type_struct_literal] = "struct-literal";
    name[Ake_ast_type_struct_literal_field] = "struct-literal-field";
    name[Ake_ast_type_ellipsis] = "ellipsis";
    name[Ake_ast_type_impl] = "impl";
	name[Ake_ast_type_const] = "const";
	name[Ake_ast_type_var] = "var";

	if (kind >= 0 && kind < AKE_AST_COUNT) {
		return name[kind];
	}

	return "Invalid Ake_ast_type";
}

struct Ake_AstList {
	Ake_Ast* head;
	Ake_Ast* tail;
};

typedef struct Ake_AstList Ake_AstList;
typedef struct Ake_Ast {
	Ake_AstKind kind;
	Zinc_string struct_value;
	Zinc_string boolean_value;
	union {
		struct { Zinc_string value; } id;
		struct { Ake_Ast* op; Ake_Ast* right; } sign;
		struct { Zinc_string value; } number;
		struct { Zinc_string value; } string;
		struct { Ake_Ast* left; Ake_Ast* right; } assign;
		struct { Ake_Ast* left; Ake_Ast* right; } plus;
		struct { Ake_Ast* left; Ake_Ast* right; } minus;
		struct { Ake_Ast* left; Ake_Ast* right; } mult;
		struct { Ake_Ast* left; Ake_Ast* right; } divide;
		struct { Ake_AstList list; } stmts;
	} data;
	Ake_Type* type;
    Zinc_location loc;
	Ake_Ast* next;
	Ake_Ast* prev;
	Ake_Ast* head;
	Ake_Ast* tail;
	Ake_Ast* parent;
	bool has_error;
	bool is_set;
	Ake_token_list token_list;
} Ake_Ast;

#ifdef __cplusplus
extern "C" {
#endif

	AKELA_API void Ake_AstCreate(Ake_Ast** n);
	AKELA_API void Ake_AstInit(Ake_Ast* n);
	AKELA_API void Ake_AstSet(Ake_Ast* n, Ake_AstKind kind);
	AKELA_API void Ake_AstValidate(Ake_Ast* n);
	AKELA_API void Ake_AstDestroy(Ake_Ast* n);
	AKELA_API void Ake_AstAdd(Ake_Ast* p, Ake_Ast* c);
	AKELA_API void Ake_AstAdd2(Ake_Ast* p, Ake_Ast* c);
	AKELA_API Ake_Ast* Ake_AstGet(Ake_Ast* p, size_t pos);
	AKELA_API void Ake_AstCopy(Ake_Ast* src, Ake_Ast* dest);
	AKELA_API Ake_Ast* Ake_AstClone(Ake_Ast* n);
	AKELA_API bool Ake_AstMatch(Ake_Ast* a, Ake_Ast* b);
	AKELA_API size_t Ake_AstCountChildren(Ake_Ast* n);

	AKELA_API void Ake_AstListInit(Ake_AstList* list);
	AKELA_API void Ake_AstListCreate(Ake_AstList** list);
	AKELA_API void Ake_AstListDestroy(Ake_AstList* list);
	AKELA_API void Ake_AstListAdd(Ake_AstList* list, Ake_Ast* n);
	AKELA_API Ake_Ast* Ake_AstListGet(Ake_AstList* list, size_t index);

#ifdef __cplusplus
}
#endif

#endif
