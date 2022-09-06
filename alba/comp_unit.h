#ifndef _COMP_UNIT_H
#define _COMP_UNIT_H

#include "alba_api.h"
#include "ast.h"
#include "source.h"
#include "symbol_table.h"

struct comp_unit {
	struct ast_node* root;
	struct compile_error_list el;
	struct buffer filename;
	struct buffer path;
	struct comp_unit* next;
	struct comp_unit* prev;
};

struct comp_unit_list {
	struct symbol_table st;
	struct comp_unit* head;
	struct comp_unit* tail;
};

ALBA_API void comp_unit_init(struct comp_unit* cu);
ALBA_API void comp_unit_destroy(struct comp_unit* cu);
ALBA_API void comp_unit_list_init(struct comp_unit_list* list);
ALBA_API void comp_unit_list_destroy(struct comp_unit_list* list);
ALBA_API void comp_unit_list_add(struct comp_unit_list* list, struct comp_unit* cu);
ALBA_API enum result comp_unit_setup(struct comp_unit* cu, input_getchar ig, input_data* id, struct symbol_table* st, struct parse_state** ps);
ALBA_API bool comp_unit_compile(struct comp_unit* cu, input_getchar ig, input_data* id, struct symbol_table* st);
ALBA_API void comp_unit_teardown(struct comp_unit* cu, struct parse_state* ps);

#endif
