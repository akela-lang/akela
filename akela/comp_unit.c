#include "comp_unit.h"
#include "zinc/buffer.h"
#include "uconv.h"
#include "zinc/result.h"
#include "zinc/memory.h"
#include "lookahead_char.h"
#include "lex.h"
#include "parse_tools.h"
#include "parse.h"
#include <assert.h>
#include "zinc/input_char_string.h"

void comp_unit_init(struct comp_unit* cu)
{
	cu->valid = true;
	cu->root = NULL;
	error_list_init(&cu->el);
	buffer_init(&cu->path);
	symbol_table_init(&cu->st);
    cu->input_obj = NULL;
	cu->next = NULL;
	cu->prev = NULL;
}

void comp_unit_destroy(struct comp_unit* cu)
{
	ast_node_destroy(cu->root);
	error_list_destroy(&cu->el);
	buffer_destroy(&cu->path);
	symbol_table_destroy(&cu->st);
}

void comp_unit_setup(struct comp_unit* cu, void* input_obj, InputCharVTable* input_vtable, struct parse_state** ps)
{
	*ps = NULL;

	ast_node_destroy(cu->root);
	cu->root = NULL;

	struct scan_state* sns = NULL;
	malloc_safe((void**)&sns, sizeof(struct scan_state));
	scan_state_init(sns, input_obj, input_vtable, &cu->el, &cu->st);

	malloc_safe((void**)ps, sizeof(struct parse_state));
	parse_state_init(*ps, sns, &cu->el, &cu->st);
}

void comp_unit_teardown(struct comp_unit* cu, struct parse_state* ps)
{
	assert(ps);
	assert(ps->sns);

	free(ps->sns);

	parse_state_destroy(ps);
	free(ps);
}

bool comp_unit_compile(struct comp_unit* cu, void* input_obj, InputCharVTable* input_vtable)
{
	bool valid = true;
	struct parse_state* ps = NULL;
    cu->input_obj = input_obj;
	comp_unit_setup(cu, input_obj, input_vtable, &ps);
    cu->root = parse(ps);
    if (cu->root->type == ast_type_error) {
        valid = false;
    }

	comp_unit_teardown(cu, ps);
	cu->valid = valid;
	return valid;
}
