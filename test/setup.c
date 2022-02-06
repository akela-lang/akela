#include <stdlib.h>
#include "alba/scan.h"

struct token_list tl;
struct string s;
struct dag_node* root;

void setup(char* line)
{
	string_init(&s);
	enum result r = array2string(line, &s);
	assert_ok(r, "array2string");
	token_list_init(&tl);
	root = NULL;
}

void teardown()
{
	string_reset(&s);
	token_list_reset(&tl);
	dag_destroy(root);
}
