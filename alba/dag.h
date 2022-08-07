#ifndef _DAG_H
#define _DAG_H

#include "zinc/buffer.h"
#include "zinc/result.h"

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
	dag_type_var,
	dag_type_boolean,
	dag_type_array,
	dag_type_array_type_name,
	dag_type_type_name,
	dag_type_count		/* keep at end */
};

# ifdef _DAG_C

/* dynamic-output-none */
enum result dag_set_names(char** names)
{
	for (int i = 0; i < dag_type_count; i++) {
		names[i] = NULL;
	}

	names[dag_type_none] = "none";
	names[dag_type_id] = "word";
	names[dag_type_sign] = "sign";
	names[dag_type_number] = "number";
	names[dag_type_assign] = "assign";
	names[dag_type_plus] = "plus";
	names[dag_type_minus] = "minus";
	names[dag_type_mult] = "mult";
	names[dag_type_divide] = "divide";
	names[dag_type_stmts] = "stmts";
	names[dag_type_function] = "function";
	names[dag_type_dseq] = "dseq";
	names[dag_type_dret] = "dret";
	names[dag_type_call] = "call";
	names[dag_type_cseq] = "cseq";
	names[dag_type_if] = "if";
	names[dag_type_conditional_branch] = "conditional-branch";
	names[dag_type_default_branch] = "default-branch";
	names[dag_type_equality] = "equality";
	names[dag_type_not_equal] = "not equal";
	names[dag_type_less_than_or_equal] = "less than or equal";
	names[dag_type_greater_than_or_equal] = "greater than or equal";
	names[dag_type_not] = "not";
	names[dag_type_and] = "and";
	names[dag_type_or] = "or";
	names[dag_type_while] = "while";
	names[dag_type_for_range] = "for-range";
	names[dag_type_for_iteration] = "for-iteration";
	names[dag_type_declaration] = "declaration";
	names[dag_type_array_literal] = "array-literal";
	names[dag_type_array_subscript] = "array-subscript";
	names[dag_type_anonymous_function] = "anonymous-function";
	names[dag_type_var] = "var";
	names[dag_type_boolean] = "boolean";
	names[dag_type_array] = "array";
	names[dag_type_array_type_name] = "array-type-name";
	names[dag_type_type_name] = "type-name";

	for (int i = 0; i < dag_type_count; i++) {
		if (names[i] == NULL) {
			return set_error("missing dag name: %d", i);
		}
	}

	return result_ok;
}

# else

/* dynamic-output-none */
enum result dag_set_names(char** names);

# endif

struct dag_node {
	enum dag_type type;
	struct buffer value;
	struct dag_node* next;
	struct dag_node* prev;
	struct dag_node* head;
	struct dag_node* tail;
};

/* dynamic-output n */
void dag_create_node(struct dag_node** n);

/* dynamic-destroy n n{} */
void dag_destroy(struct dag_node* n);

/* dynamic-output-none */
void dag_init_node(struct dag_node* n);

/* dynamic-output-none */
void dag_add_child(struct dag_node* p, struct dag_node* c);

/* dynamic-output-none */
void dag_push(struct dag_node* parent, struct dag_node* child);

/* dynamic-output-none */
struct dag_node* dag_get_child(struct dag_node* p, size_t pos);

/* dynamic-output-none */
int is_binary_operator(struct dag_node* n);

/* dynamic-output-none */
void dag_print(struct dag_node* root, char** names);

#endif
