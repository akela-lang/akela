#include <stdbool.h>
#include "zinc/unit_test.h"
#include "zinc/buffer.h"
#include "zinc/memory.h"
#include "alba/input.h"
#include "alba/uconv.h"
#include "alba/parse_tools.h"
#include "alba/parse.h"
#include "alba/scan.h"
#include "alba/source.h"

/* dynamic-output ps{} root root{} */
bool parse_setup(char* line, struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	enum result r;

	/* allocate bf */
	struct buffer* bf;
	malloc_safe(&bf, sizeof(struct buffer));
	buffer_init(bf);

	/* allocate bf{} */
	array2buffer(line, bf);

	/* allocate sd */
	struct string_data* sd;
	malloc_safe(&sd, sizeof(struct string_data));

	/* transfer bf -> sd{} */
	string_data_init(bf, sd);

	/* allocate el */
	struct compile_error_list* el;
	malloc_safe(&el, sizeof(struct compile_error_list));
	compile_error_list_init(el);

	/* open conv */
	UConverter* conv;
	r = conv_open(&conv);
	assert_ok(r, "conv_open");
	if (r == result_error) {
		set_source_error(el, NULL, error_message);
		valid = false;
	}

	/* allocate wt */
	struct word_table* wt;
	malloc_safe(&wt, sizeof(struct word_table));

	/* allocate wt {} */
	word_table_init(wt, WORD_TABLE_SIZE);

	/* allocate lc */
	struct lookahead_char* lc;
	malloc_safe(&lc, sizeof(struct lookahead_char));

	/* transfer sd conv -> lc */
	lookahead_char_init(lc, string_getchar, sd, conv);

	/* allocate sns */
	struct scan_state* sns;
	malloc_safe(&sns, sizeof(struct scan_state));

	/* transfer lc wt el -> sns{} */
	scan_state_init(sns, lc, wt, el);

	/* transfer sns el -> ps{}  */
	parse_state_init(ps, sns, el);

	/* allocate ps{} root root{} */
	valid = valid && parse(ps, root);
	return valid;
}

/* dynamic-destroy ps{} */
void parse_teardown(struct parse_state* ps)
{
	struct string_data* sd = ps->sns->lc->d;
	struct buffer* bf = sd->bf;

	/* destroy ps{lc{d}} */
	buffer_destroy(bf);
	free(bf);
	free(sd);

	/* destroy ps{sns{el el{}}}*/
	struct compile_error_list* el = ps->sns->el;
	compile_error_list_destroy(el);
	free(el);

	/* destroy ps{sns{wt wt{}}} */
	struct word_table* wt = ps->sns->wt;
	word_table_destroy(wt);
	free(wt);

	/* destroy ps{sns{lc{conv}}} */
	conv_close(ps->sns->lc->conv);

	/* destroy ps{sns{lc}} */
	struct lookahead_char* lc = ps->sns->lc;
	free(lc);

	/* destroy ps{sns} */
	free(ps->sns);

	/* destroy ps{lookahead{}}*/
	token_list_destroy(&ps->lookahead);
}

/* dynamic-output-none */
struct dag_node* check_stmts(struct dag_node* root, char* message)
{
	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, message);

	return dag_get_child(root, 0);
}
