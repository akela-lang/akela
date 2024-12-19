#include <stdbool.h>
#include "zinc/unit_test.h"
#include "zinc/buffer.h"
#include "zinc/memory.h"
#include "akela/parse_tools.h"
#include "akela/parse.h"
#include "akela/lex.h"
#include "akela/comp_unit.h"
#include "zinc/input_unicode.h"
#include "zinc/input_unicode_string.h"
#include <string.h>
#include "test_parse.h"

bool parse_setup(char* line, struct comp_unit* cu)
{
    Vector* text = NULL;
    VectorCreate(&text, sizeof(char));
    VectorAdd(text, line, strlen(line));

    InputUnicodeString* input_obj = NULL;
    InputUnicodeStringCreate(&input_obj, text);

	comp_unit_init(cu);
	bool valid = comp_unit_compile(cu, input_obj, input_obj->input_vtable);

	return valid;
}

void parse_teardown(struct comp_unit* cu)
{
    InputUnicodeString* input = cu->input_obj;
    Vector* text = input->text;
    VectorDestroy(text);
    free(text);
    free(input);
	comp_unit_destroy(cu);
}