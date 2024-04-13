#include "zinc/buffer.h"
#include "akela/input.h"
#include "akela/uconv.h"
#include "akela/symbol_table.h"
#include "akela/lex.h"
#include "zinc/memory.h"
#include "zinc/unit_test.h"
#include "zinc/input_unicode_string.h"
#include <string.h>

void scan_setup(char* line, struct lex_state* sns, struct lookahead_char* lc, struct error_list* el)
{
    Vector* text = NULL;
    VectorCreate(&text, sizeof(char));
    VectorAdd(text, line, strlen(line));

    InputUnicodeString* input_string = NULL;
    InputUnicodeStringCreate(&input_string, text);

	error_list_init(el);

	struct symbol_table* st = NULL;
	malloc_safe((void**)&st, sizeof(struct symbol_table));
	symbol_table_init(st);

    lex_state_init(sns, input_string, input_string->input_vtable, el, st);
}

void scan_teardown(struct lex_state* sns)
{
    InputUnicodeString* input_string = sns->input_obj;
    VectorDestroy(input_string->text);

	struct error_list* el = sns->el;

	error_list_destroy(el);
	symbol_table_destroy(sns->st);
	free(sns->st);
}
