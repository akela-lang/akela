#ifndef JSON_TEST_PARSE_TOOLS_H
#define JSON_TEST_PARSE_TOOLS_H

#include "json/parse_tools.h"
#include <string.h>
#include "zinc/input_unicode_string.h"

void Zinc_unit_parse_setup(Json_parse_data* pd, char* text);
void Zinc_unit_parse_destroy(Json_parse_data* pd);

#endif