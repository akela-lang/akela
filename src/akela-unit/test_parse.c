#include <stdbool.h>
#include "zinc/unit_test.h"
#include "zinc/zstring.h"
#include "zinc/memory.h"
#include "akela/parse_tools.h"
#include "akela/parse.h"
#include "akela/lex.h"
#include "akela/comp_unit.h"
#include "zinc/input_unicode.h"
#include "zinc/input_unicode_string.h"
#include <string.h>
#include "test_parse.h"

bool parse_setup(char* line, struct Ake_comp_unit* cu)
{
    Zinc_vector* text = NULL;
    Zinc_vector_create(&text, sizeof(char));
    Zinc_vector_add(text, line, strlen(line));

    Zinc_input_unicode_string* input_obj = NULL;
    Zinc_input_unicode_string_create(&input_obj, text);

	Ake_comp_unit_init(cu);
	bool valid = Ake_comp_unit_compile(cu, input_obj, input_obj->vtable);

	return valid;
}

void parse_teardown(struct Ake_comp_unit* cu)
{
    Zinc_input_unicode_string* input = cu->input;
    Zinc_vector* text = input->text;
    Zinc_vector_destroy(text);
    free(text);
    free(input);
	Ake_comp_unit_destroy(cu);
}