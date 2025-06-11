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
#include "type_use.h"
#include "zinc/list.h"
#include "environment.h"

typedef enum Ake_ast_type {
	Ake_ast_type_none,
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
    Ake_ast_type_let_lseq,
    Ake_ast_type_let_rseq,
    Ake_ast_type_error,
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
} Ake_ast_type;

static char const* Ast_type_name(Ake_ast_type type)
{
	char const* name[Ake_ast_type_count];
    name[Ake_ast_type_none] = "none";
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
    name[Ake_ast_type_let_lseq] = "let_lseq";
    name[Ake_ast_type_let_rseq] = "let_rseq";
    name[Ake_ast_type_error] = "error";
    name[Ake_ast_type_prototype] = "prototype";
    name[Ake_ast_type_extern] = "extern";
    name[Ake_ast_type_struct_literal] = "struct-literal";
    name[Ake_ast_type_struct_literal_field] = "struct-literal-field";
    name[Ake_ast_type_ellipsis] = "ellipsis";
    name[Ake_ast_type_impl] = "impl";
    name[Ake_ast_type_self] = "self";
	name[Ake_ast_type_const] = "const";
	name[Ake_ast_type_var] = "var";

	if (type >= 0 && type < Ake_ast_type_count) {
		return name[type];
	}

	return "Invalid Ake_ast_type";
}

typedef struct Ake_ast {
	Ake_ast_type type;
	Zinc_string value;
	struct Ake_type_use* tu;
    bool is_mut;
    struct Zinc_location loc;
    struct Ake_symbol* sym;
	Ake_Environment* env;
	struct Ake_ast* next;
	struct Ake_ast* prev;
	struct Ake_ast* head;
	struct Ake_ast* tail;
} Ake_ast;

#ifdef __cplusplus
extern "C" {
#endif

//AKELA_API enum result Ast_set_names(char const** names);
void Ake_ast_create(Ake_ast** n);
AKELA_API void Ake_ast_destroy(Ake_ast* n);
AKELA_API void Ake_ast_init(Ake_ast* n);
AKELA_API void Ake_ast_add(Ake_ast* p, Ake_ast* c);
AKELA_API void Ast_node_push(Ake_ast* parent, Ake_ast* child);
AKELA_API Ake_ast* Ast_node_get(Ake_ast* p, size_t pos);
AKELA_API void Ake_ast_print(Ake_ast* n);
AKELA_API void Ake_ast_print_pointers(Ake_ast* root, struct Zinc_list* l);
AKELA_API void Ake_ast_copy(Ake_ast* src, Ake_ast* dest);
AKELA_API Ake_ast* Ake_ast_clone(Ake_ast* n);
AKELA_API bool Ake_ast_match(Ake_ast* a, Ake_ast* b);
AKELA_API size_t Ake_ast_count_children(Ake_ast* n);
AKELA_API void Ast_node_validate(Ake_ast* n, struct Zinc_list* l);

#ifdef __cplusplus
}
#endif

#endif
