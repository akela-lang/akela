#include "comp_unit.h"
#include "zinc/buffer.h"
#include "uconv.h"
#include "zinc/result.h"
#include "zinc/memory.h"
#include "lookahead_char.h"
#include "scan.h"
#include "parse_tools.h"
#include "parse.h"

void comp_unit_init(struct comp_unit* cu)
{
	cu->valid = true;
	cu->root = NULL;
	compile_error_list_init(&cu->el);
	buffer_init(&cu->path);
	symbol_table_init(&cu->st);
	cu->next = NULL;
	cu->prev = NULL;
}

void comp_unit_destroy(struct comp_unit* cu)
{
	ast_node_destroy(cu->root);
	compile_error_list_destroy(&cu->el);
	buffer_destroy(&cu->path);
	symbol_table_destroy(&cu->st);
}

enum result comp_unit_setup(struct comp_unit* cu, input_getchar ig, input_data id, struct parse_state** ps)
{
	enum result r = result_ok;
	*ps = NULL;

	/* allocate conv{} */
	UConverter* conv;
	r = conv_open(&conv);

	/* allocate exp{el{}} */
	if (r == result_error) {
		struct location loc;
		location_init(&loc);
		set_source_error(&cu->el, &loc, "conv_open: %s", error_message);
		return r;
	}

	/* destroy root root{} */
	ast_node_destroy(cu->root);
	cu->root = NULL;

	struct lookahead_char* lc = NULL;
	malloc_safe(&lc, sizeof(struct lookahead_char));
	lookahead_char_init(lc, ig, id, conv);

	struct scan_state* sns = NULL;
	malloc_safe(&sns, sizeof(struct scan_state));
	scan_state_init(sns, lc, &cu->el, &cu->st);

	malloc_safe(ps, sizeof(struct parse_state));
	parse_state_init(*ps, sns, &cu->el, &cu->st);

	return r;
}

bool comp_unit_compile(struct comp_unit* cu, input_getchar ig, input_data id)
{
	struct parse_state* ps = NULL;
	enum result r = comp_unit_setup(cu, ig, id, &ps);
	cu->valid = parse(ps, &cu->root);
	comp_unit_teardown(cu, ps);
	return cu->valid;
}

void comp_unit_teardown(struct comp_unit* cu, struct parse_state* ps)
{
	/* destroy conv{} */
	conv_close(ps->sns->lc->conv);

	free(ps->sns->lc);

	free(ps->sns);

	/* destroy ps{} */
	parse_state_destroy(ps);
	free(ps);
}
