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
        struct {
            Zinc_string title;
            int level;
            Lava_dom* head;
            Lava_dom* tail;
        } header;
        Zinc_string text;
        struct {
            Zinc_string format;
            Zinc_string text;
            Zinc_input_bounds bounds;
        } backquote;
    } data;
    Zinc_location loc;
    Lava_dom* next;
    Lava_dom* prev;
};

void Lava_dom_init(Lava_dom* dom, Lava_dom_kind kind);
void Lava_dom_create(Lava_dom** dom, Lava_dom_kind kind);
void Lava_dom_destroy(Lava_dom* dom);
void Lava_dom_add(Lava_dom* header, Lava_dom* child);
size_t Lava_dom_count(Lava_dom* header);
Lava_dom* Lava_dom_get(Lava_dom* header, size_t index);

#endif