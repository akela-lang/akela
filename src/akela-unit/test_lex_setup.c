#include "zinc/zstring.h"
#include "akela/symbol_table.h"
#include "akela/lex.h"
#include "zinc/memory.h"
#include "zinc/unit_test.h"
#include "zinc/input_unicode_string.h"
#include <string.h>

void lex_setup(char* line, struct Ake_lex_state* ls, struct Zinc_error_list* el)
{
    Zinc_vector* text = NULL;
    Zinc_vector_create(&text, sizeof(char));
    Zinc_vector_add(text, line, strlen(line));

    Zinc_input_unicode_string* input_string = NULL;
    Zinc_input_unicode_string_create(&input_string, text);

	Zinc_error_list_init(el);

	struct Ake_symbol_table* st = NULL;
	Zinc_malloc_safe((void**)&st, sizeof(struct Ake_symbol_table));
	Ake_symbol_table_init(st);

    Ake_lex_state_init(ls, input_string, input_string->input_vtable, el, st);
}

void lex_teardown(struct Ake_lex_state* ls)
{
    Zinc_input_unicode_string* input_string = ls->input_obj;
    Zinc_vector_destroy(input_string->text);
    free(input_string->text);
    free(input_string);

	struct Zinc_error_list* el = ls->el;

	Zinc_error_list_destroy(el);
	Ake_symbol_table_destroy(ls->st);
	free(ls->st);
}
