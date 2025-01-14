#ifndef AKELA_AST_H
#define AKELA_AST_H

#include "api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "zinc/vector.h"
#include "type_use.h"
#include "zinc/list.h"

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
	Ake_ast_type_let,
	Ake_ast_type_boolean,
	Ake_ast_type_parenthesis,
	Ake_ast_type_type,
	Ake_ast_type_power,
	Ake_ast_type_type_pool,
	Ake_ast_type_dot,
	Ake_ast_type_module,
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
	Ake_ast_type_count		/* keep at end */
} Ake_ast_type;

#ifdef AKELA_AST_C
enum result Ast_set_names(char** names)
{
    for (int i = 0; i < Ake_ast_type_count; i++) {
        names[i] = NULL;
    }

    names[Ake_ast_type_none] = "none";
    names[Ake_ast_type_id] = "id";
    names[Ake_ast_type_sign] = "sign";
    names[Ake_ast_type_number] = "number";
    names[Ake_ast_type_string] = "string";
    names[Ake_ast_type_assign] = "assign";
    names[Ake_ast_type_plus] = "plus";
    names[Ake_ast_type_minus] = "minus";
    names[Ake_ast_type_mult] = "mult";
    names[Ake_ast_type_divide] = "divide";
    names[Ake_ast_type_stmts] = "parse_stmts";
    names[Ake_ast_type_function] = "function";
    names[Ake_ast_type_dseq] = "dseq";
    names[Ake_ast_type_dret] = "dret";
    names[Ake_ast_type_call] = "call";
    names[Ake_ast_type_cseq] = "cseq";
    names[Ake_ast_type_if] = "if";
    names[Ake_ast_type_conditional_branch] = "conditional-branch";
    names[Ake_ast_type_default_branch] = "default-branch";
    names[Ake_ast_type_equality] = "equality";
    names[Ake_ast_type_not_equal] = "not-equal";
    names[Ake_ast_type_less_than] = "less-than";
    names[Ake_ast_type_less_than_or_equal] = "less-than-or-equal";
    names[Ake_ast_type_greater_than] = "greater-than";
    names[Ake_ast_type_greater_than_or_equal] = "greater-than-or-equal";
    names[Ake_ast_type_not] = "not";
    names[Ake_ast_type_and] = "and";
    names[Ake_ast_type_or] = "or";
    names[Ake_ast_type_while] = "while";
    names[Ake_ast_type_for_range] = "for-range";
    names[Ake_ast_type_for_iteration] = "for-iteration";
    names[Ake_ast_type_declaration] = "declaration";
    names[Ake_ast_type_array_literal] = "array-literal";
    names[Ake_ast_type_array_subscript] = "array-subscript";
    names[Ake_ast_type_let] = "let";
    names[Ake_ast_type_boolean] = "boolean";
    names[Ake_ast_type_parenthesis] = "parenthesis";
    names[Ake_ast_type_type] = "type";
    names[Ake_ast_type_power] = "power";
    names[Ake_ast_type_type_pool] = "type-pool";
    names[Ake_ast_type_dot] = "dot";
    names[Ake_ast_type_module] = "module";
    names[Ake_ast_type_struct] = "struct";
    names[Ake_ast_type_return] = "return";
    names[Ake_ast_type_eseq] = "eseq";
    names[Ake_ast_type_let_lseq] = "let_lseq";
    names[Ake_ast_type_let_rseq] = "let_rseq";
    names[Ake_ast_type_error] = "error";
    names[Ake_ast_type_prototype] = "prototype";
    names[Ake_ast_type_extern] = "extern";
    names[Ake_ast_type_struct_literal] = "struct-literal";
    names[Ake_ast_type_struct_literal_field] = "struct-literal-field";
    names[Ake_ast_type_ellipsis] = "ellipsis";
    names[Ake_ast_type_impl] = "impl";
    names[Ake_ast_type_self] = "self";

    for (int i = 0; i < Ake_ast_type_count; i++) {
        if (names[i] == NULL) {
            return set_error("missing dag name: %d", i);
        }
    }

    return result_ok;
}
#endif

typedef struct Ake_ast {
	Ake_ast_type type;
	struct buffer value;
	struct Type_use* tu;
    bool is_mut;
    struct location loc;
    struct Ake_symbol* sym;
	struct Ake_ast* next;
	struct Ake_ast* prev;
	struct Ake_ast* head;
	struct Ake_ast* tail;
} Ake_ast;

#ifdef __cplusplus
extern "C" {
#endif

AKELA_API enum result Ast_set_names(char** names);
AKELA_API void Ake_ast_create(Ake_ast** n);
AKELA_API void Ake_ast_destroy(Ake_ast* n);
AKELA_API void Ake_ast_init(Ake_ast* n);
AKELA_API void Ake_ast_add(Ake_ast* p, Ake_ast* c);
AKELA_API void Ast_node_push(Ake_ast* parent, Ake_ast* child);
AKELA_API Ake_ast* Ast_node_get(Ake_ast* p, size_t pos);
AKELA_API void Ake_ast_print(Ake_ast* n);
AKELA_API void Ake_ast_print_pointers(Ake_ast* root, struct list* l);
AKELA_API void Ake_ast_copy(Ake_ast* src, Ake_ast* dest);
AKELA_API Ake_ast* Ake_ast_clone(Ake_ast* n);
AKELA_API bool Ake_ast_match(Ake_ast* a, Ake_ast* b);
AKELA_API size_t Ake_ast_count_children(Ake_ast* n);
AKELA_API void Ast_node_validate(Ake_ast* n, struct list* l);

#ifdef __cplusplus
}
#endif

#endif
