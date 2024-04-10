#include <stdbool.h>
#include "zinc/unit_test.h"
#include "zinc/buffer.h"
#include "zinc/memory.h"
#include "akela/input.h"
#include "akela/uconv.h"
#include "akela/parse_tools.h"
#include "akela/parse.h"
#include "akela/lex.h"
#include "akela/source.h"
#include "akela/comp_unit.h"
#include "zinc/input_char.h"
#include "zinc/input_char_string.h"
#include <string.h>

bool parse_setup(char* line, struct comp_unit* cu)
{
    Vector* text = NULL;
    VectorCreate(&text, sizeof(char));
    VectorAdd(text, line, strlen(line));

    InputCharString* input_obj = NULL;
    InputCharStringCreate(&input_obj, text);

	comp_unit_init(cu);
	bool valid = comp_unit_compile(cu, input_obj, input_obj->input_vtable);

	return valid;
}

void parse_teardown(struct comp_unit* cu)
{
    InputCharString* input_obj = cu->input_obj;
    Vector* text = input_obj->text;
    VectorDestroy(text);
    free(text);
	comp_unit_destroy(cu);
}
