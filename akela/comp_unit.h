	#ifndef _COMP_UNIT_H
#define _COMP_UNIT_H

#include "akela_api.h"
#include "ast.h"
#include "symbol_table.h"
#include <stdbool.h>
#include "zinc/input_unicode.h"
#include "zinc/buffer_list.h"

struct comp_unit {
	bool valid;
	Ast_node* root;
	struct error_list el;
    struct buffer_list extern_list;
	struct symbol_table st;
    void* input_obj;
	struct buffer path;
	struct comp_unit* next;
	struct comp_unit* prev;
};

AKELA_API void comp_unit_init(struct comp_unit* cu);
AKELA_API void comp_unit_create(struct comp_unit** cu);
AKELA_API void comp_unit_destroy(struct comp_unit* cu);
AKELA_API bool comp_unit_compile(struct comp_unit* cu, void* input_obj, InputUnicodeVTable* input_vtable);

#endif
