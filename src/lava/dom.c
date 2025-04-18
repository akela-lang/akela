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
        dom->data.LAVA_DOM_HEADER.head = NULL;
        dom->data.LAVA_DOM_HEADER.tail = NULL;
        break;
    case LAVA_DOM_TEXT:
        Zinc_string_init(&dom->data.LAVA_DOM_TEXT);
        break;
    case LAVA_DOM_BACKQUOTE:
        Zinc_string_init(&dom->data.LAVA_DOM_BACKQUOTE.format);
        Zinc_string_init(&dom->data.LAVA_DOM_BACKQUOTE.text);
        Zinc_input_bounds_init(&dom->data.LAVA_DOM_BACKQUOTE.bounds);
        break;
    default:
        assert(false && "not possible");
    }
    Zinc_location_init(&dom->loc);
    dom->next = NULL;
    dom->prev = NULL;
}

void Lava_dom_create(Lava_dom** dom, Lava_dom_kind kind)
{
    Zinc_malloc_safe((void**)dom, sizeof(Lava_dom));
    Lava_dom_init(*dom, kind);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Lava_dom_destroy(Lava_dom* dom)
{
    switch (dom->kind) {
    case LAVA_DOM_HEADER:
        Zinc_string_destroy(&dom->data.LAVA_DOM_HEADER.title);
        Lava_dom* p = dom->data.LAVA_DOM_HEADER.head;
        while (p) {
            Lava_dom* temp = p;
            p = p->next;
            Lava_dom_destroy(temp);
            free(temp);
        }
        break;
    case LAVA_DOM_TEXT:
        Zinc_string_destroy(&dom->data.LAVA_DOM_TEXT);
        break;
    case LAVA_DOM_BACKQUOTE:
        Zinc_string_destroy(&dom->data.LAVA_DOM_BACKQUOTE.format);
        Zinc_string_destroy(&dom->data.LAVA_DOM_BACKQUOTE.text);
        break;
    default:
        assert(false && "not possible");
    }
}

void Lava_dom_add(Lava_dom* header, Lava_dom* child)
{
    assert(header->kind == LAVA_DOM_HEADER);
    if (header->data.LAVA_DOM_HEADER.head && header->data.LAVA_DOM_HEADER.tail) {
        header->data.LAVA_DOM_HEADER.tail->next = child;
        child->prev = header->data.LAVA_DOM_HEADER.tail;
        header->data.LAVA_DOM_HEADER.tail = child;
    } else {
        header->data.LAVA_DOM_HEADER.head = child;
        header->data.LAVA_DOM_HEADER.tail = child;
    }
}

size_t Lava_dom_count(Lava_dom* header)
{
    assert(header->kind == LAVA_DOM_HEADER);
    size_t count = 0;
    Lava_dom* p = header->data.LAVA_DOM_HEADER.head;
    while (p) {
        count++;
        p = p->next;
    }
    return count;
}

Lava_dom* Lava_dom_get(Lava_dom* header, size_t index)
{
    assert(header->kind == LAVA_DOM_HEADER);
    size_t count = 0;
    Lava_dom* p = header->data.LAVA_DOM_HEADER.head;
    while (p) {
        if (count == index) {
            return p;
        }
        count++;
        p = p->next;
    }
    return NULL;
}