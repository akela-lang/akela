#ifndef JSON_STRINGIFY_H
#define JSON_STRINGIFY_H

#include "dom.h"

typedef struct Json_stringify_property_data {
    struct Zinc_error_list* el;
    Json_dom* dom;
    struct Zinc_string* bf;
} Json_stringify_property_data;

void Json_stringify(struct Zinc_error_list* el, Json_dom* dom, struct Zinc_string *bf);

#endif