#ifndef _DAG_H
#define _DAG_H

#include "alba_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "source.h"

enum dag_type {
	dag_type_none,
	dag_type_id,
	dag_type_sign,
	dag_type_number,
	dag_type_string,
	dag_type_assign,
	dag_type_plus,
	dag_type_minus,
	dag_type_mult,
	dag_type_divide,
	dag_type_stmts,
	dag_type_function,
	dag_type_dseq,
	dag_type_dret,
	dag_type_call,
	dag_type_cseq,
	dag_type_if,
	dag_type_conditional_branch,
	dag_type_default_branch,
	dag_type_equality,
	dag_type_not_equal,
	dag_type_less_than,
	dag_type_less_than_or_equal,
	dag_type_greater_than,
	dag_type_greater_than_or_equal,
	dag_type_not,
	dag_type_and,
	dag_type_or,
	dag_type_while,
	dag_type_for_range,
	dag_type_for_iteration,
	dag_type_declaration,
	dag_type_array_literal,
	dag_type_array_subscript,
	dag_type_anonymous_function,
	dag_type_type_function,
	dag_type_type_dseq,
	dag_type_type_dret,
	dag_type_var,
	dag_type_boolean,
	dag_type_array,
	dag_type_array_type_name,
	dag_type_type_name,
	dag_type_parenthesis,
	dag_type_count		/* keep at end */
};

struct dag_node {
	enum dag_type type;
	struct buffer value;
	struct token_list tl;
	struct location loc;
	struct dag_node* etype;
	struct dag_node* next;
	struct dag_node* prev;
	struct dag_node* head;
	struct dag_node* tail;
};

/* dynamic-output-none */
ALBA_API enum result dag_set_names(char** names);

/* dynamic-output n */
ALBA_API void dag_create_node(struct dag_node** n);

/* dynamic-destroy n n{} */
ALBA_API void dag_destroy(struct dag_node* n);

/* dynamic-output-none */
ALBA_API void dag_init_node(struct dag_node* n);

/* dynamic-output-none */
ALBA_API void dag_add_child(struct dag_node* p, struct dag_node* c);

/* dynamic-output-none */
ALBA_API void dag_push(struct dag_node* parent, struct dag_node* child);

/* dynamic-output-none */
ALBA_API struct dag_node* dag_get_child(struct dag_node* p, size_t pos);

/* dynamic-output-none */
ALBA_API int is_binary_operator(struct dag_node* n);

/* dynamic-output-none */
ALBA_API void dag_print(struct dag_node* root, char** names);

ALBA_API struct dag_node* dag_copy(struct dag_node* n);

ALBA_API bool dag_match(struct dag_node* a, struct dag_node* b);

#endif
