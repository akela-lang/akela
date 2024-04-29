#ifndef _AST_H
#define _AST_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"

enum ast_type {
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
	ast_type_anonymous_function,
	ast_type_var,
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
    ast_type_var_lseq,
    ast_type_var_rseq,
    ast_type_error,
	ast_type_count		/* keep at end */
};

struct ast_node {
	enum ast_type type;
	struct buffer value;
	struct ast_node* tu;
	struct type_def* td;
    struct location loc;
    struct symbol* sym;
	struct ast_node* next;
	struct ast_node* prev;
	struct ast_node* head;
	struct ast_node* tail;
};

#ifdef __cplusplus
extern "C" {
#endif

AKELA_API enum result ast_set_names(char** names);
AKELA_API void ast_node_create(struct ast_node** n);
AKELA_API void ast_node_destroy(struct ast_node* n);
AKELA_API void ast_node_init(struct ast_node* n);
AKELA_API void ast_node_add(struct ast_node* p, struct ast_node* c);
AKELA_API void ast_node_push(struct ast_node* parent, struct ast_node* child);
AKELA_API struct ast_node* ast_node_get(struct ast_node* p, size_t pos);
AKELA_API void ast_node_print(struct ast_node* root, char** names, bool debug);
AKELA_API struct ast_node* ast_node_copy(struct ast_node* n);
AKELA_API bool ast_node_match(struct ast_node* a, struct ast_node* b);
AKELA_API size_t ast_node_count_children(struct ast_node* n);
AKELA_API void ast_node_location_update_token(struct ast_node* n, struct token* t);
AKELA_API void ast_node_location_update(struct ast_node* n, struct ast_node* n2);

#ifdef __cplusplus
}
#endif

#endif
