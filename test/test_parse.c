#include "assert.h"
#include "alba/buffer.h"
#include "alba/input.h"
#include "alba/uconv.h"
#include "alba/parse_tools.h"
#include "alba/parse.h"

void parse_setup(struct allocator* al, char* line, struct token_state* ts, struct dag_node** root)
{
	enum result r;

	allocator_init(al);

	struct buffer* bf;
	r = allocator_malloc(al, &bf, sizeof(struct buffer));
	assert_ok(r, "allocator malloc string");
	buffer_init(bf);
	r = array2buffer(al, line, bf);
	assert_ok(r, "ok");
	buffer_add_char(al, bf, '\n');

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

	token_state_init(is, ts);

	r = parse(al, ts, root);
	assert_ok(r, "parse");
}

void parse_teardown(struct allocator* al, struct token_state* ts)
{
	conv_close(ts->is->conv);
	allocator_destroy(al);
}

