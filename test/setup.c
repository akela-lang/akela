#include <stdlib.h>
#include "alba/token.h"
#include "alba/token_defer.h"
#include "alba/scan.h"
#include "alba/allocator.h"
#include "alba/dag.h"

struct token_list tl;
struct string s;
struct dag_node* root;
struct allocator al;

void setup(char* line)
{
	allocator_init(&al);
	string_init(&s);
	enum result r = array2string(line, &s);
	assert_ok(r, "array2string");
	token_list_init(&tl);
	root = NULL;
}

void teardown()
{
	string_reset(&s);
	token_list_reset_defer(&tl);
	dag_destroy(root);
	allocator_destroy(&al);
}
