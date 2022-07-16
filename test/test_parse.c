#include "zinc/assert.h"
#include "zinc/buffer.h"
#include "zinc/memory.h"
#include "alba/input.h"
#include "alba/uconv.h"
#include "alba/parse_tools.h"
#include "alba/parse.h"
#include "alba/scan.h"
#include "alba/source.h"

enum result parse_setup(char* line, struct parse_state* ps, struct dag_node** root)
{
	enum result r;

	struct buffer* bf;
	malloc_safe(&bf, sizeof(struct buffer));
	buffer_init(bf);
	r = array2buffer(line, bf);
	assert_ok(r, "ok");

	struct string_data* sd;
	malloc_safe(&sd, sizeof(struct string_data));
	string_data_init(bf, sd);

	UConverter* conv;
	r = conv_open(&conv);
	assert_ok(r, "conv_open");

	struct word_table* wt;
	malloc_safe(&wt, sizeof(struct word_table));
	r = word_table_init(wt, WORD_TABLE_SIZE);
	assert_ok(r, "word table init");

	struct lookahead_char* lc;
	malloc_safe(&lc, sizeof(struct lookahead_char));
	lookahead_char_init(lc, string_getchar, sd, conv);

	struct compile_error_list* el;
	malloc_safe(&el, sizeof(struct compile_error_list));
	compile_error_list_init(el);

	struct scan_state* sns;
	malloc_safe(&sns, sizeof(struct scan_state));
	scan_state_init(sns, lc, wt, el);

	parse_state_init(ps, sns, el);

	r = parse(ps, root);
	return r;
}

void parse_teardown(struct parse_state* ps)
{
	conv_close(ps->sns->lc->conv);
}

struct dag_node* check_stmts(struct dag_node* root, char* message)
{
	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, message);

	return dag_get_child(root, 0);
}
