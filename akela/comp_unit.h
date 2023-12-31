	#ifndef _COMP_UNIT_H
#define _COMP_UNIT_H

#include "akela_api.h"
#include "ast.h"
#include "source.h"
#include "symbol_table.h"
#include <stdbool.h>

struct comp_unit {
	bool valid;
	struct ast_node* root;
	struct error_list el;
	struct symbol_table st;
	struct buffer path;
	struct comp_unit* next;
	struct comp_unit* prev;
};

AKELA_API void comp_unit_init(struct comp_unit* cu);
AKELA_API void comp_unit_destroy(struct comp_unit* cu);
AKELA_API bool comp_unit_compile(struct comp_unit* cu, input_getchar ig, input_data id);

#endif
