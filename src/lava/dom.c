#include "dom.h"
#include <assert.h>
#include "zinc/memory.h"

void Lava_dom_init(Lava_dom* dom, Lava_dom_kind kind)
{
    dom->kind = kind;
    switch (kind) {
    case LAVA_DOM_HEADER:
        Zinc_string_init(&dom->data.LAVA_DOM_HEADER.title);
        dom->data.LAVA_DOM_HEADER.level = 0;
        Zinc_vector_init(&dom->data.LAVA_DOM_HEADER.items, sizeof(Lava_dom));
        break;
    case LAVA_DOM_TEXT:
        Zinc_string_init(&dom->data.LAVA_DOM_TEXT);
        break;
    case LAVA_DOM_BACKQUOTE:
        Zinc_string_init(&dom->data.LAVA_DOM_BACKQUOTE.format);
        Zinc_string_init(&dom->data.LAVA_DOM_BACKQUOTE.text);
    default:
        assert(false && "not possible");
    }
}

void Lava_item_create(Lava_dom** dom, Lava_dom_kind kind)
{
    Zinc_malloc_safe((void**)dom, sizeof(Lava_dom));
    Lava_dom_init(*dom, kind);
}

void Lava_item_destroy(Lava_dom* dom)
{
    switch (dom->kind) {
    case LAVA_DOM_HEADER:
        Zinc_string_destroy(&dom->data.LAVA_DOM_HEADER.title);
        Zinc_vector_destroy(&dom->data.LAVA_DOM_HEADER.items);
        break;
    case LAVA_DOM_TEXT:
        Zinc_string_destroy(&dom->data.LAVA_DOM_TEXT);
        break;
    case LAVA_DOM_BACKQUOTE:
        Zinc_string_destroy(&dom->data.LAVA_DOM_BACKQUOTE.format);
        Zinc_string_destroy(&dom->data.LAVA_DOM_BACKQUOTE.text);
    default:
        assert(false && "not possible");
    }
}