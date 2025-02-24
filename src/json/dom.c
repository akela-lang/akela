#include "dom.h"
#include "zinc/memory.h"
#include <assert.h>
#include "general.h"

void Json_dom_init(Json_dom* dom)
{
    dom->type = Json_dom_type_none;
    dom->number_type = Json_dom_number_type_none;
    dom->has_error = false;
    Zinc_location_init(&dom->loc);
    dom->next = NULL;
    dom->prev = NULL;
    dom->head = NULL;
    dom->tail = NULL;
}

void Json_dom_set_type(Json_dom* dom, Json_dom_type type)
{
    dom->type = type;
    switch (type) {
        case Json_dom_type_none:
        case Json_dom_type_null:
            break;
        case Json_dom_type_boolean:
            dom->value.boolean = false;
            break;
        case Json_dom_type_string:
            Zinc_string_init(&dom->value.string);
            break;
        case Json_dom_type_number:
            dom->value.integer = 0;
            dom->value.fp = 0.0;
            break;
        case Json_dom_type_array:
            break;
        case Json_dom_type_object:
            Zinc_hash_map_string_init(&dom->value.object, 32);
            break;
        default:
            assert(false && "invalid type");
    }
}

void Json_dom_create(Json_dom** dom)
{
    Zinc_malloc_safe((void**)dom, sizeof(Json_dom));
    Json_dom_init(*dom);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Json_dom_destroy(Json_dom* dom)
{
    switch (dom->type) {
    case Json_dom_type_none:
    case Json_dom_type_null:
    case Json_dom_type_boolean:
        break;
    case Json_dom_type_string:
        Zinc_string_destroy(&dom->value.string);
        break;
    case Json_dom_type_number:
    case Json_dom_type_array:
        Json_dom* p = dom->head;
        while (p) {
            Json_dom* temp = p;
            p = p->next;
            Json_dom_destroy(temp);
        }
        break;
    case Json_dom_type_object:
        Zinc_hash_map_string_map(&dom->value.object, (Zinc_hash_map_string_func)Json_dom_destroy);
        Zinc_hash_map_string_destroy(&dom->value.object);
        break;
    default:
        assert(false && "invalid type");
    }

    free(dom);
}

/**
 * Add an array element to the JSON DOM
 * @param p parent
 * @param c child
 */
void Json_dom_add_element(Json_dom* p, Json_dom* c)
{
    assert(p->type == Json_dom_type_array);

    if (p->head && p->tail) {
        p->tail->next = c;
        c->prev = p->tail;
        p->tail = c;
    } else {
        p->head = c;
        p->tail = c;
    }

    if (c->has_error) {
        p->has_error = true;
    }

    Json_location_combine(&p->loc, &c->loc);
}

Json_dom* Json_dom_get_element(Json_dom* dom, size_t index)
{
    size_t i = 0;
    Json_dom* p = dom->head;
    while (p) {
        if (i == index) return p;
        p = p->next;
        i++;
    }

    return NULL;
}

void Json_dom_add_property(Json_dom* dom, struct Zinc_string* name, Json_dom* value)
{
    assert(dom->type == Json_dom_type_object);
    Zinc_hash_map_string_add(&dom->value.object, name, value);
}

void Json_dom_add_property_str(Json_dom* dom, char* name, Json_dom* value)
{
    assert(dom->type == Json_dom_type_object);
    Zinc_hash_map_string_add_str(&dom->value.object, name, value);
}

Json_dom* Json_dom_get_property(Json_dom* dom, struct Zinc_string* name)
{
    assert(dom->type == Json_dom_type_object);
    return Zinc_hash_map_string_get(&dom->value.object, name);
}

Json_dom* Json_dom_get_property_str(Json_dom* dom, char* name)
{
    assert(dom->type == Json_dom_type_object);
    return Zinc_hash_map_string_get_str(&dom->value.object, name);
}