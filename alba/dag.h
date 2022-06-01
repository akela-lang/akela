#ifndef _DAG_H
#define _DAG_H

#include "ustring.h"

enum dag_type {
	dag_type_none,
	dag_type_statement,
	dag_type_assignment,
	dag_type_expression,
	dag_type_word,
	dag_type_value,
	dag_type_expression2,
	dag_type_sign,
	dag_type_number,
	dag_type_assign,
	dag_type_plus,
	dag_type_minus,
	dag_type_mult,
	dag_type_divide,
	dag_type_left_parenthesis,
	dag_type_right_parenthesis
};

struct dag_node {
	enum dag_type type;
	struct string value;
	struct dag_node* next;
	struct dag_node* prev;
	struct dag_node* head;
	struct dag_node* tail;
};

enum result dag_create_node(struct dag_node** n);
void dag_init_node(struct dag_node* n);
void dag_add_child(struct dag_node* p, struct dag_node* c);
void dag_push(struct dag_node* parent, struct dag_node* child);
void dag_destroy(struct dag_node* r);
struct dag_node* dag_get_child(struct dag_node* p, size_t pos);

#endif
