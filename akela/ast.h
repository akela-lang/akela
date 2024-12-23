#ifndef AKELA_AST_H
#define AKELA_AST_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "zinc/vector.h"
#include "type_use.h"
#include "zinc/list.h"

typedef enum Cob_ast_type {
	Cob_ast_type_none,
	Ast_type_id,
	Ast_type_sign,
	Cob_ast_type_number,
	Ast_type_string,
	Ast_type_assign,
	Ast_type_plus,
	Ast_type_minus,
	Ast_type_mult,
	Ast_type_divide,
	Ast_type_stmts,
	Ast_type_function,
	Ast_type_dseq,
	Ast_type_dret,
	Ast_type_call,
	Ast_type_cseq,
	Ast_type_if,
	Ast_type_conditional_branch,
	Ast_type_default_branch,
	Ast_type_equality,
	Ast_type_not_equal,
	Ast_type_less_than,
	Ast_type_less_than_or_equal,
	Ast_type_greater_than,
	Ast_type_greater_than_or_equal,
	Ast_type_not,
	Ast_type_and,
	Ast_type_or,
	Ast_type_while,
	Ast_type_for_range,
	Ast_type_for_iteration,
	Ast_type_declaration,
	Ast_type_array_literal,
	Ast_type_array_subscript,
	Ast_type_let,
	Ast_type_boolean,
	Ast_type_parenthesis,
	Ast_type_type,
	Ast_type_power,
	Ast_type_type_pool,
	Ast_type_dot,
	Ast_type_module,
	Ast_type_struct,
	Ast_type_return,
    Ast_type_eseq,
    Ast_type_let_lseq,
    Ast_type_let_rseq,
    Cob_ast_type_error,
    Ast_type_prototype,
    Ast_type_extern,
    Ast_type_struct_literal,
    Ast_type_struct_literal_field,
    Ast_type_ellipsis,
    Ast_type_impl,
    Ast_type_self,
	Ast_type_count		/* keep at end */
} Cob_ast_type;

#ifdef AKELA_AST_C
enum result Ast_set_names(char** names)
{
    for (int i = 0; i < Ast_type_count; i++) {
        names[i] = NULL;
    }

    names[Cob_ast_type_none] = "none";
    names[Ast_type_id] = "id";
    names[Ast_type_sign] = "sign";
    names[Cob_ast_type_number] = "number";
    names[Ast_type_string] = "string";
    names[Ast_type_assign] = "assign";
    names[Ast_type_plus] = "plus";
    names[Ast_type_minus] = "minus";
    names[Ast_type_mult] = "mult";
    names[Ast_type_divide] = "divide";
    names[Ast_type_stmts] = "parse_stmts";
    names[Ast_type_function] = "function";
    names[Ast_type_dseq] = "dseq";
    names[Ast_type_dret] = "dret";
    names[Ast_type_call] = "call";
    names[Ast_type_cseq] = "cseq";
    names[Ast_type_if] = "if";
    names[Ast_type_conditional_branch] = "conditional-branch";
    names[Ast_type_default_branch] = "default-branch";
    names[Ast_type_equality] = "equality";
    names[Ast_type_not_equal] = "not-equal";
    names[Ast_type_less_than] = "less-than";
    names[Ast_type_less_than_or_equal] = "less-than-or-equal";
    names[Ast_type_greater_than] = "greater-than";
    names[Ast_type_greater_than_or_equal] = "greater-than-or-equal";
    names[Ast_type_not] = "not";
    names[Ast_type_and] = "and";
    names[Ast_type_or] = "or";
    names[Ast_type_while] = "while";
    names[Ast_type_for_range] = "for-range";
    names[Ast_type_for_iteration] = "for-iteration";
    names[Ast_type_declaration] = "declaration";
    names[Ast_type_array_literal] = "array-literal";
    names[Ast_type_array_subscript] = "array-subscript";
    names[Ast_type_let] = "let";
    names[Ast_type_boolean] = "boolean";
    names[Ast_type_parenthesis] = "parenthesis";
    names[Ast_type_type] = "type";
    names[Ast_type_power] = "power";
    names[Ast_type_type_pool] = "type-pool";
    names[Ast_type_dot] = "dot";
    names[Ast_type_module] = "module";
    names[Ast_type_struct] = "struct";
    names[Ast_type_return] = "return";
    names[Ast_type_eseq] = "eseq";
    names[Ast_type_let_lseq] = "let_lseq";
    names[Ast_type_let_rseq] = "let_rseq";
    names[Cob_ast_type_error] = "error";
    names[Ast_type_prototype] = "prototype";
    names[Ast_type_extern] = "extern";
    names[Ast_type_struct_literal] = "struct-literal";
    names[Ast_type_struct_literal_field] = "struct-literal-field";
    names[Ast_type_ellipsis] = "ellipsis";
    names[Ast_type_impl] = "impl";
    names[Ast_type_self] = "self";

    for (int i = 0; i < Ast_type_count; i++) {
        if (names[i] == NULL) {
            return set_error("missing dag name: %d", i);
        }
    }

    return result_ok;
}
#endif

typedef struct Cob_ast {
	Cob_ast_type type;
	struct buffer value;
	struct Type_use* tu;
    bool is_mut;
    struct location loc;
    struct symbol* sym;
	struct Cob_ast* next;
	struct Cob_ast* prev;
	struct Cob_ast* head;
	struct Cob_ast* tail;
} Cob_ast;

#ifdef __cplusplus
extern "C" {
#endif

AKELA_API enum result Ast_set_names(char** names);
AKELA_API void Cob_ast_create(Cob_ast** n);
AKELA_API void Cob_ast_destroy(Cob_ast* n);
AKELA_API void Cob_ast_init(Cob_ast* n);
AKELA_API void Cob_ast_add(Cob_ast* p, Cob_ast* c);
AKELA_API void Ast_node_push(Cob_ast* parent, Cob_ast* child);
AKELA_API Cob_ast* Ast_node_get(Cob_ast* p, size_t pos);
AKELA_API void Ast_node_print(Cob_ast* n);
AKELA_API void Ast_node_print_pointers(Cob_ast* root, struct list* l);
AKELA_API void Ast_node_copy(Cob_ast* src, Cob_ast* dest);
AKELA_API Cob_ast* Ast_node_clone(Cob_ast* n);
AKELA_API bool Ast_node_match(Cob_ast* a, Cob_ast* b);
AKELA_API size_t Ast_node_count_children(Cob_ast* n);
AKELA_API void Ast_node_validate(Cob_ast* n, struct list* l);

#ifdef __cplusplus
}
#endif

#endif
