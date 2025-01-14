#include "zinc/zstring.h"
#include "akela/symbol_table.h"
#include "akela/lex.h"
#include "zinc/memory.h"
#include "zinc/unit_test.h"
#include "zinc/input_unicode_string.h"
#include <string.h>

void lex_setup(char* line, struct Ake_lex_state* ls, struct error_list* el)
{
    Vector* text = NULL;
    VectorCreate(&text, sizeof(char));
    VectorAdd(text, line, strlen(line));

    InputUnicodeString* input_string = NULL;
    InputUnicodeStringCreate(&input_string, text);

	error_list_init(el);

	struct Ake_symbol_table* st = NULL;
	malloc_safe((void**)&st, sizeof(struct Ake_symbol_table));
	Ake_symbol_table_init(st);

    Ake_lex_state_init(ls, input_string, input_string->input_vtable, el, st);
}

void lex_teardown(struct Ake_lex_state* ls)
{
    InputUnicodeString* input_string = ls->input_obj;
    VectorDestroy(input_string->text);
    free(input_string->text);
    free(input_string);

	struct error_list* el = ls->el;

	error_list_destroy(el);
	Ake_symbol_table_destroy(ls->st);
	free(ls->st);
}
