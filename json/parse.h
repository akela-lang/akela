#ifndef JSON_PARSE_H
#define JSON_PARSE_H

#include "dom.h"
#include "parse_tools.h"

Json_dom* Json_parse(Json_parse_data* pd);
bool Json_parse_is_valid(Json_parse_data* pd, Json_dom* dom);

#endif