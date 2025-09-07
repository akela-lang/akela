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
	AKE_AST_FUNCTION,
	AKE_AST_DSEQ,
	AKE_AST_DRET,
	AKE_AST_CALL,
	AKE_AST_IF,
	AKE_AST_COND_BRANCH,
	AKE_AST_DEFAULT_BRANCH,
	AKE_AST_EQUALITY,
	AKE_AST_NOT_EQUAL,
	AKE_AST_LESS_THAN,
	AKE_AST_LESS_THAN_OR_EQUAL,
	AKE_AST_GREATER_THAN,
	AKE_AST_GREATER_THAN_OR_EQUAL,
	AKE_AST_NOT,
	AKE_AST_AND,
	AKE_AST_OR,
	AKE_AST_WHILE,
	AKE_AST_FOR_RANGE,
	AKE_AST_FOR_ITERATION,
	AKE_AST_DECLARATION,
	AKE_AST_ARRAY_LITERAL,
	AKE_AST_ARRAY_SUBSCRIPT,
	AKE_AST_BOOLEAN,
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
    name[AKE_AST_FUNCTION] = "function";
    name[AKE_AST_DSEQ] = "dseq";
    name[AKE_AST_DRET] = "dret";
    name[AKE_AST_CALL] = "call";
    name[AKE_AST_IF] = "if";
    name[AKE_AST_COND_BRANCH] = "conditional-branch";
    name[AKE_AST_DEFAULT_BRANCH] = "default-branch";
    name[AKE_AST_EQUALITY] = "equality";
    name[AKE_AST_NOT_EQUAL] = "not-equal";
    name[AKE_AST_LESS_THAN] = "less-than";
    name[AKE_AST_LESS_THAN_OR_EQUAL] = "less-than-or-equal";
    name[AKE_AST_GREATER_THAN] = "greater-than";
    name[AKE_AST_GREATER_THAN_OR_EQUAL] = "greater-than-or-equal";
    name[AKE_AST_NOT] = "not";
    name[AKE_AST_AND] = "and";
    name[AKE_AST_OR] = "or";
    name[AKE_AST_WHILE] = "while";
    name[AKE_AST_FOR_RANGE] = "for-range";
    name[AKE_AST_FOR_ITERATION] = "for-iteration";
    name[AKE_AST_DECLARATION] = "declaration";
    name[AKE_AST_ARRAY_LITERAL] = "array-literal";
    name[AKE_AST_ARRAY_SUBSCRIPT] = "array-subscript";
    name[AKE_AST_BOOLEAN] = "boolean";
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
	Ake_Ast* parent;
	Zinc_location loc;
	bool has_error;
};

typedef struct Ake_AstList Ake_AstList;
typedef struct Ake_Ast {
	Ake_AstKind kind;
	Zinc_string struct_value;
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
		struct { Ake_Ast* proto; Ake_Ast* body; } function;
		struct { Ake_AstList list; } dseq;
		struct { Ake_Ast* node; } dret;
		struct { Ake_Ast* func; Ake_AstList args; } call;
		struct { Ake_AstList branches; } _if_;
		struct { Ake_Ast* cond; Ake_Ast* body; } cond_branch;
		struct { Ake_Ast* body; } default_branch;
		struct { Ake_Ast* left; Ake_Ast* right; } equality;
		struct { Ake_Ast* left; Ake_Ast* right; } not_equal;
		struct { Ake_Ast* left; Ake_Ast* right; } less_than;
		struct { Ake_Ast* left; Ake_Ast* right; } less_than_or_equal;
		struct { Ake_Ast* left; Ake_Ast* right; } greater_than;
		struct { Ake_Ast* left; Ake_Ast* right; } greater_than_or_equal;
		struct { Ake_Ast* right; } _not_;
		struct { Ake_Ast* left; Ake_Ast* right; } _and_;
		struct { Ake_Ast* left; Ake_Ast* right; } _or_;
		struct { Ake_Ast* cond; Ake_Ast* body; } _while_;
		struct { Ake_Ast* dec; Ake_Ast* start; Ake_Ast* end; Ake_Ast* body; } for_range;
		struct { Ake_Ast* dec; Ake_Ast* iterator; Ake_Ast* body; } for_iteration;
		struct { Ake_Ast* id; Ake_Ast* type; } declaration;
		struct { Ake_AstList list; } array_literal;
		struct { Ake_Ast* array; Ake_Ast* index; } array_subscript;
		struct { Zinc_string value; } boolean;
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

	AKELA_API void Ake_AstListInit(Ake_AstList* list, Ake_Ast* parent);
	AKELA_API void Ake_AstListCreate(Ake_AstList** list, Ake_Ast* parent);
	AKELA_API void Ake_AstListDestroy(Ake_AstList* list);
	AKELA_API void Ake_AstListAdd(Ake_AstList* list, Ake_Ast* n);
	AKELA_API Ake_Ast* Ake_AstListGet(Ake_AstList* list, size_t index);
	AKELA_API size_t Ake_AstListCount(Ake_AstList* list);

#ifdef __cplusplus
}
#endif

#endif
