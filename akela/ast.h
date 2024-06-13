#ifndef _AST_H
#define _AST_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "zinc/vector.h"

typedef enum Array_element_option {
    Array_element_default,
    Array_element_const,
} Array_element_option;

typedef struct Type_dimension {
    size_t size;
    Array_element_option option;
} Type_dimension;

typedef struct Type_options {
    bool is_mut;
    bool is_array;
    bool is_slice;
    bool is_ref;
    bool original_is_mut;
    Vector dim;             /* vector of Type_dimension */
} Type_options;

typedef enum Ast_type {
	ast_type_none,
	ast_type_id,
	ast_type_sign,
	ast_type_number,
	ast_type_string,
	ast_type_assign,
	ast_type_plus,
	ast_type_minus,
	ast_type_mult,
	ast_type_divide,
	ast_type_stmts,
	ast_type_function,
	ast_type_dseq,
	ast_type_dret,
	ast_type_call,
	ast_type_cseq,
	ast_type_if,
	ast_type_conditional_branch,
	ast_type_default_branch,
	ast_type_equality,
	ast_type_not_equal,
	ast_type_less_than,
	ast_type_less_than_or_equal,
	ast_type_greater_than,
	ast_type_greater_than_or_equal,
	ast_type_not,
	ast_type_and,
	ast_type_or,
	ast_type_while,
	ast_type_for_range,
	ast_type_for_iteration,
	ast_type_declaration,
	ast_type_array_literal,
	ast_type_array_subscript,
	ast_type_let,
	ast_type_boolean,
	ast_type_array,
	ast_type_parenthesis,
	ast_type_type,
	ast_type_power,
	ast_type_type_pool,
	ast_type_dot,
	ast_type_module,
	ast_type_struct,
	ast_type_return,
    ast_type_eseq,
    ast_type_let_lseq,
    ast_type_let_rseq,
    ast_type_error,
    ast_type_prototype,
    ast_type_extern,
	ast_type_count		/* keep at end */
} Ast_type;

typedef struct Ast_node {
	Ast_type type;
	struct buffer value;
	struct Ast_node* tu;
	struct type_def* td;
    struct Type_options to;
    struct location loc;
    struct symbol* sym;
	struct Ast_node* next;
	struct Ast_node* prev;
	struct Ast_node* head;
	struct Ast_node* tail;
} Ast_node;

#ifdef __cplusplus
extern "C" {
#endif

AKELA_API enum result Ast_set_names(char** names);
AKELA_API void Ast_node_create(Ast_node** n);
AKELA_API void Ast_node_destroy(Ast_node* n);
AKELA_API void Ast_node_init(Ast_node* n);
AKELA_API void Ast_node_add(Ast_node* p, Ast_node* c);
AKELA_API void Ast_node_push(Ast_node* parent, Ast_node* child);
AKELA_API Ast_node* Ast_node_get(Ast_node* p, size_t pos);
AKELA_API void Ast_node_print(Ast_node* root, bool debug);
AKELA_API Ast_node* Ast_node_copy(Ast_node* n);
AKELA_API bool Ast_node_match(Ast_node* a, Ast_node* b);
AKELA_API size_t Ast_node_count_children(Ast_node* n);
AKELA_API void ast_node_location_update_token(Ast_node* n, struct token* t);
AKELA_API void ast_node_location_update(Ast_node* n, Ast_node* n2);
AKELA_API void Type_options_init(Type_options* to);
AKELA_API void Type_options_destroy(Type_options* to);
AKELA_API void Type_options_reduce_dimension(Type_options* to);

#ifdef __cplusplus
}
#endif

#endif
