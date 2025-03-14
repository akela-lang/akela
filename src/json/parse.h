#ifndef JSON_PARSE_H
#define JSON_PARSE_H

#include "parse_tools.h"
#include "zinc/string_slice.h"

Json_result Json_parse_str(char* s);
Json_result Json_parse_zstring(Zinc_string* string);
Json_result Json_parse_slice(Zinc_string_slice slice);
Json_result Json_parse(Json_parse_data* pd);

#endif
