#ifndef _DAG_H
#define _DAG_H

#include "buffer.h"
#include "result.h"
#include "allocator.h"

enum dag_type {
	dag_type_none,
	dag_type_word,
	dag_type_sign,
	dag_type_number,
	dag_type_assign,
	dag_type_plus,
	dag_type_minus,
	dag_type_mult,
	dag_type_divide,
	dag_type_stmts,
	dag_type_function,
	dag_type_count		/* keep at end */
};

#ifdef _DAG_C

enum result dag_set_names(char** names)
{
	for (int i = 0; i < dag_type_count; i++) {
		names[i] = NULL;
	}

	names[dag_type_none] = "none";
	names[dag_type_word] = "word";
	names[dag_type_sign] = "sign";
	names[dag_type_number] = "number";
	names[dag_type_assign] = "assign";
	names[dag_type_plus] = "plus";
	names[dag_type_minus] = "minus";
	names[dag_type_mult] = "mult";
	names[dag_type_divide] = "divide";
	names[dag_type_stmts] = "stmts";

	for (int i = 0; i < dag_type_count; i++) {
		if (names[i] == NULL) {
			return set_error("missing dag name: %d", i);
		}
	}

	return result_ok;
}

#else

void dag_set_names(char** names);

#endif

struct dag_node {
	enum dag_type type;
	struct buffer value;
	struct dag_node* next;
	struct dag_node* prev;
	struct dag_node* head;
	struct dag_node* tail;
};

enum result dag_create_node(struct allocator *al, struct dag_node** n);
void dag_init_node(struct dag_node* n);
void dag_add_child(struct dag_node* p, struct dag_node* c);
void dag_push(struct dag_node* parent, struct dag_node* child);
struct dag_node* dag_get_child(struct dag_node* p, size_t pos);
int is_binary_operator(struct dag_node* n);
void dag_print(struct allocator* al, struct dag_node* root, char** names);

#endif
