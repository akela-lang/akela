#ifndef JSON_PARSE_H
#define JSON_PARSE_H

#include "dom.h"
#include "parse_tools.h"

Json_result Json_parse_str(char* s);
Json_result Json_parse(Json_parse_data* pd);

#endif
