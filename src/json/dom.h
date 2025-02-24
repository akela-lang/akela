#ifndef JSON_DOM_H
#define JSON_DOM_H

#include "zinc/zstring.h"
#include <stdbool.h>
#include "zinc/hash_map_string.h"
#include "zinc/error.h"

typedef enum Json_dom_type {
    Json_dom_type_none,
    Json_dom_type_null,
    Json_dom_type_boolean,
    Json_dom_type_number,
    Json_dom_type_string,
    Json_dom_type_array,
    Json_dom_type_object,
} Json_dom_type;

typedef enum Json_dom_number_type {
    Json_dom_number_type_none,
    Json_dom_number_type_integer,
    Json_dom_number_type_fp,
} Json_dom_number_type;

typedef struct Json_dom {
    Json_dom_type type;
    Json_dom_number_type number_type;
    union {
        /* null: no value */            /* null */
        bool boolean;                   /* boolean */
        long long integer;     /* number integer */
        double fp;                      /* number fp */
        struct Zinc_string string;           /* string */
        /* array: use head and tail */  /* array */
        Zinc_hash_map_string object;       /* object: hash struct buffer -> Json_dom */
    } value;
    bool has_error;
    struct Zinc_location loc;
    struct Json_dom *next;
    struct Json_dom *prev;
    struct Json_dom *head;
    struct Json_dom *tail;
} Json_dom;

void Json_dom_init(Json_dom* dom);
void Json_dom_set_type(Json_dom* dom, Json_dom_type type);
void Json_dom_create(Json_dom** dom);
void Json_dom_destroy(Json_dom* dom);
void Json_dom_add_element(Json_dom* p, Json_dom* c);
Json_dom* Json_dom_get_element(Json_dom* dom, size_t index);
void Json_dom_add_property(Json_dom* dom, struct Zinc_string* name, Json_dom* value);
void Json_dom_add_property_str(Json_dom* dom, char* name, Json_dom* value);
Json_dom* Json_dom_get_property(Json_dom* dom, struct Zinc_string* name);
Json_dom* Json_dom_get_property_str(Json_dom* dom, char* name);

#endif
