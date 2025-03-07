#include <stdbool.h>
#include "input_unicode.h"
#include "result.h"

Zinc_location* Zinc_input_unicode_location(void* input_obj, Zinc_input_unicode_vtable* input_vtable)
{
    return (uint8_t*)input_obj + input_vtable->loc_offset;
}

Zinc_result Zinc_input_unicode_next(
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable,
    char c[4],
    int* num,
    Zinc_location* loc,
    bool* done)
{
    Zinc_input_unicode_next_interface *get_unicode = (uint8_t*)input_obj + input_vtable->next_offset;
    return (*get_unicode)(input_obj, c, num, loc, done);
}

void Zinc_input_unicode_repeat(void* input_obj, Zinc_input_unicode_vtable* input_vtable)
{
    Zinc_input_unicode_repeat_interface *repeat_unicode = (uint8_t*)input_obj + input_vtable->repeat_offset;
    (*repeat_unicode)(input_obj);
}

void Zinc_input_unicode_seek(
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable,
    Zinc_location* loc)
{
    Zinc_input_unicode_seek_interface *seek = (uint8_t*)input_obj + input_vtable->seek_offset;
    (*seek)(input_obj, loc);
}

void Zinc_input_unicode_get_all(
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable,
    Zinc_vector** text)
{
    Zinc_input_unicode_get_all_interface *get_all = (uint8_t*)input_obj + input_vtable->get_all_offset;
    (*get_all)(input_obj, text);
}

Zinc_location Zinc_input_unicode_get_location(
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable)
{
    Zinc_input_unicode_get_location_interface *get_location =
        (uint8_t*)input_obj + input_vtable->get_location_offset;
    return (*get_location)(input_obj);
}

void Zinc_input_unicode_destroy(void* input_obj, Zinc_input_unicode_vtable* input_vtable)
{
    Zinc_input_unicode_destroy_interface *destroy = (uint8_t*)input_obj + input_vtable->destroy_offset;
    (*destroy)(input_obj);
}