#include "comp_unit.h"
#include "zinc/buffer.h"
#include "uconv.h"
#include "zinc/result.h"
#include "zinc/memory.h"
#include "lookahead_char.h"
#include "scan.h"
#include "parse_tools.h"
#include "parse.h"
#include <assert.h>

void comp_unit_init(struct comp_unit* cu)
{
	cu->valid = true;
	cu->root = NULL;
	error_list_init(&cu->el);
	buffer_init(&cu->path);
	symbol_table_init(&cu->st);
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

bool comp_unit_setup(struct comp_unit* cu, input_getchar ig, input_data id, struct parse_state** ps)
{
	enum result r = result_ok;
	bool valid = true;
	*ps = NULL;

	/* allocate conv{} */
	UConverter* conv;
	r = conv_open(&conv);

	/* allocate exp{el{}} */
	if (r == result_error) {
		struct location loc;
		location_init(&loc);
		valid = set_source_error(&cu->el, &loc, "conv_open: %s", error_message);
		return valid;
	}

	/* destroy root root{} */
	ast_node_destroy(cu->root);
	cu->root = NULL;

	struct lookahead_char* lc = NULL;
	malloc_safe((void**) & lc, sizeof(struct lookahead_char));
	lookahead_char_init(lc, ig, id, conv);

	struct scan_state* sns = NULL;
	malloc_safe((void**)&sns, sizeof(struct scan_state));
	scan_state_init(sns, lc, &cu->el, &cu->st);

	malloc_safe((void**)ps, sizeof(struct parse_state));
	parse_state_init(*ps, sns, &cu->el, &cu->st);

	return valid;
}

void comp_unit_teardown(struct comp_unit* cu, struct parse_state* ps)
{
	assert(ps);
	assert(ps->sns);
	/* destroy conv{} */
	conv_close(ps->sns->lc->conv);

	free(ps->sns->lc);

	free(ps->sns);

	/* destroy ps{} */
	parse_state_destroy(ps);
	free(ps);
}

bool comp_unit_compile(struct comp_unit* cu, input_getchar ig, input_data id)
{
	bool valid = true;
	struct parse_state* ps = NULL;
	valid = comp_unit_setup(cu, ig, id, &ps) && valid;
	if (valid) {
		valid = parse(ps, &cu->root) && valid;
	}
	comp_unit_teardown(cu, ps);
	cu->valid = valid;
	return valid;
}
