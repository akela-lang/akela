#include "zinc/buffer.h"
#include "akela/input.h"
#include "akela/uconv.h"
#include "akela/symbol_table.h"
#include "akela/scan.h"
#include "zinc/memory.h"
#include "zinc/unit_test.h"
#include "zinc/input_char_string.h"
#include <string.h>

void scan_setup(char* line, struct scan_state* sns, struct lookahead_char* lc, struct error_list* el)
{
    Vector* text = NULL;
    VectorCreate(&text, sizeof(char));
    VectorAdd(text, line, strlen(line));

    InputCharString* input_string = NULL;
    InputCharStringCreate(&input_string, text);

	error_list_init(el);

	struct symbol_table* st = NULL;
	malloc_safe((void**)&st, sizeof(struct symbol_table));
	symbol_table_init(st);

	scan_state_init(sns, input_string, input_string->input_vtable, el, st);
}

void scan_teardown(struct scan_state* sns)
{
    InputCharString* input_string = sns->input_obj;
    VectorDestroy(input_string->text);

	struct error_list* el = sns->el;

	error_list_destroy(el);
	symbol_table_destroy(sns->st);
	free(sns->st);
}
