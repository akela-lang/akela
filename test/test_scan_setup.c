#include "zinc/buffer.h"
#include "alba/input.h"
#include "alba/uconv.h"
#include "alba/symbol_table.h"
#include "alba/scan.h"
#include "zinc/memory.h"
#include "zinc/unit_test.h"

/* dynamic-output sns{lc{d{bf} sns{lc{d{bf{}}} sns{lc{conv}} sns{wt{}} sns{el{}} */
void scan_setup(char* line, struct scan_state* sns, struct lookahead_char* lc, struct error_list* el)
{
	struct buffer* bf = NULL;

	/* allocate bf */
	malloc_safe((void**)&bf, sizeof(struct buffer));
	buffer_init(bf);

	/* allocate bf{} */
	array2buffer(line, bf);

	struct string_data* sd = NULL;

	/* allocate sd */
	malloc_safe((void**)&sd, sizeof(struct string_data));

	/* transfer bf -> sd */
	string_data_init(bf, sd);

	/* open conv */
	UConverter* conv;
	enum result r = conv_open(&conv);
	assert_ok(r, "conv_open");

	/* transfer sd conv -> lc{} */
	lookahead_char_init(lc, (input_getchar)string_getchar, sd, conv);

	compile_error_list_init(el);

	struct symbol_table* st = NULL;
	malloc_safe((void**)&st, sizeof(struct symbol_table));
	symbol_table_init(st);

	/* transfer lc{} wt{} el{} -> sns */
	scan_state_init(sns, lc, el, st);
}

/* destroy sns{lc{d{bf{}}}} sns{lc{d{bf}}} sns{lc{d}} sns{wt{}} sns{el{}} sns{lc{conv}} */
void scan_teardown(struct scan_state* sns)
{
	struct string_data* sd = (struct string_data*)sns->lc->d;
	struct buffer* bf = sd->bf;
	struct error_list* el = sns->el;

	/* destroy bf bf{} sd wt{} el{} conv */
	buffer_destroy(bf);
	free(bf);
	free(sd);
	compile_error_list_destroy(el);
	conv_close(sns->lc->conv);
	symbol_table_destroy(sns->st);
	free(sns->st);
}
