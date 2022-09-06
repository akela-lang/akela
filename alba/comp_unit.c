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
	cu->root = NULL;
	compile_error_list_init(&cu->el);
	buffer_init(&cu->filename);
	buffer_init(&cu->path);
	cu->next = NULL;
	cu->prev = NULL;
}

void comp_unit_destroy(struct comp_unit* cu)
{
	ast_node_destroy(cu->root);
	compile_error_list_destroy(&cu->el);
	buffer_destroy(&cu->filename);
	buffer_destroy(&cu->path);
}

void comp_unit_list_init(struct comp_unit_list* list)
{
	symbol_table_init(&list->st);
	list->head = NULL;
	list->tail = NULL;
}

void comp_unit_list_destroy(struct comp_unit_list* list)
{
	symbol_table_destroy(&list->st);
	struct comp_unit* p = list->head;
	while (p) {
		struct comp_unit* temp = p;
		p = p->next;
		comp_unit_destroy(temp);
		free(temp);
	}
}

void comp_unit_list_add(struct comp_unit_list* list, struct comp_unit* cu)
{
	if (list->head) {
		list->head->prev = cu;
	}
	cu->next = list->head;
	list->head = cu;
	if (!list->tail) {
		list->tail = cu;
	}
}

enum result comp_unit_setup(struct comp_unit* cu, input_getchar ig, input_data* id, struct symbol_table* st, struct parse_state** ps)
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
	scan_state_init(sns, lc, &cu->el, st);

	malloc_safe(ps, sizeof(struct parse_state));
	parse_state_init(*ps, sns, &cu->el, st);

	return r;
}

bool comp_unit_compile(struct comp_unit* cu, input_getchar ig, input_data* id, struct symbol_table* st)
{
	struct parse_state* ps = NULL;
	enum result r = comp_unit_setup(cu, ig, id, st, &ps);
	bool valid = parse(ps, &cu->root);
	comp_unit_teardown(cu, ps);
	return valid;
}

void comp_unit_teardown(struct comp_unit* cu, struct parse_state* ps)
{
	/* destroy conv{} */
	conv_close(ps->sns->lc->conv);

	free(ps->sns->lc);

	free(ps->sns);

	/* destroy ps{} */
	token_list_destroy(&ps->lookahead);
	free(ps);
}
