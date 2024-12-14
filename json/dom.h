#ifndef JSON_DOM_H
#define JSON_DOM_H

#include "zinc/buffer.h"
#include <stdbool.h>
#include "zinc/hash.h"
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
        struct buffer string;           /* string */
        /* array: use head and tail */  /* array */
        struct hash_table object;       /* object: hash struct buffer -> Json_dom */
    } value;
    bool has_error;
    struct location loc;
    struct Json_dom *next;
    struct Json_dom *prev;
    struct Json_dom *head;
    struct Json_dom *tail;
} Json_dom;

void Json_dom_init(Json_dom* dom);
void Json_dom_set_type(Json_dom* dom, Json_dom_type type);
void Json_dom_create(Json_dom** dom);
void Json_dom_destroy(Json_dom* dom);
void Json_dom_add(Json_dom* p, Json_dom* c);

#endif