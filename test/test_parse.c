#include "assert.h"
#include "alba/buffer.h"
#include "alba/input.h"
#include "alba/uconv.h"
#include "alba/parse_tools.h"
#include "alba/parse.h"
#include "alba/scan.h"

enum result parse_setup(struct allocator* al, char* line, struct parse_state* ps, struct dag_node** root)
{
	enum result r;

	allocator_init(al);

	struct buffer* bf;
	r = allocator_malloc(al, &bf, sizeof(struct buffer));
	assert_ok(r, "allocator malloc string");
	buffer_init(bf);
	r = array2buffer(al, line, bf);
	assert_ok(r, "ok");

	struct string_data* sd;
	r = allocator_malloc(al, &sd, sizeof(struct string_data));
	assert_ok(r, "allocator_malloc string_data");
	string_data_init(bf, sd);

	UConverter* conv;
	r = conv_open(&conv);
	assert_ok(r, "conv_open");

	struct input_state* is;
	r = allocator_malloc(al, &is, sizeof(struct input_state));
	assert_ok(r, "malloc input state");
	input_state_init(string_getchar, sd, conv, is);

	struct word_table* wt;
	r = allocator_malloc(al, &wt, sizeof(struct word_table));
	assert_ok(r, "malloc word table");
	r = word_table_init(al, wt, WORD_TABLE_SIZE);
	assert_ok(r, "word table init");

	struct scan_state* sns;
	r = allocator_malloc(al, &sns, sizeof(struct scan_state));
	assert_ok(r, "malloc scan state");
	scan_state_init(sns, is, wt);

	parse_state_init(ps, sns);

	r = parse(al, ps, root);
	return r;
}

void parse_teardown(struct allocator* al, struct parse_state* ps)
{
	conv_close(ps->sns->is->conv);
	allocator_destroy(al);
}

struct dag_node* check_stmts(struct dag_node* root, char* message)
{
	assert_ptr(root, "ptr root");
	expect_int_equal(root->type, dag_type_stmts, message);

	return dag_get_child(root, 0);
}
