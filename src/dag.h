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
	dag_type_number,
	dag_type_assign,
	dag_type_plus,
	dag_type_minus,
	dag_type_multiply,
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

#endif
