#ifndef LAVA_AST_H
#define LAVA_AST_H
#include <zinc/error.h>
#include <zinc/vector.h>
#include <zinc/zstring.h>
#include "zinc/input_bounds.h"

typedef enum Lava_dom_kind Lava_dom_kind;
enum Lava_dom_kind {
    LAVA_DOM_HEADER,
    LAVA_DOM_TEXT,
    LAVA_DOM_BACKQUOTE,
};

typedef struct Lava_dom Lava_dom;
struct Lava_dom {
    Lava_dom_kind kind;
    union {
        struct { Zinc_string title; int level; Zinc_vector items; } LAVA_DOM_HEADER;
        Zinc_string LAVA_DOM_TEXT;
        struct {
            Zinc_string format;
            Zinc_string text;
            Zinc_input_bounds bounds;
        } LAVA_DOM_BACKQUOTE;
    } data;
    Zinc_location loc;
};

void Lava_dom_init(Lava_dom* dom, Lava_dom_kind kind);
void Lava_dom_create(Lava_dom** dom, Lava_dom_kind kind);
void Lava_dom_destroy(Lava_dom* dom);

#endif