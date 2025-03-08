#include "zinc/os.h"
#include <stdbool.h>
#include "input_unicode.h"
#include "result.h"

Zinc_location* Zinc_input_unicode_location(void* input_obj, Zinc_input_unicode_vtable* input_vtable)
{
#if IS_UNIX
    return input_obj + input_vtable->loc_offset;
#elif IS_WIN
    return (uint8_t*)input_obj + input_vtable->loc_offset;
#else #error Unsupported platform
#endif
}

Zinc_result Zinc_input_unicode_next(
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable,
    char c[4],
    int* num,
    Zinc_location* loc,
    bool* done)
{
#if IS_UNIX
    Zinc_input_unicode_next_interface *get_unicode = input_obj + input_vtable->next_offset;
#elif IS_WIN
    Zinc_input_unicode_next_interface *get_unicode = (uint8_t*)input_obj + input_vtable->next_offset;
#else #error Unsupported platform
#endif
    return (*get_unicode)(input_obj, c, num, loc, done);
}

void Zinc_input_unicode_repeat(void* input_obj, Zinc_input_unicode_vtable* input_vtable)
{
#if IS_UNIX
    Zinc_input_unicode_repeat_interface *repeat_unicode = input_obj + input_vtable->repeat_offset;
#elif IS_WIN
    Zinc_input_unicode_repeat_interface *repeat_unicode = (uint8_t*)input_obj + input_vtable->repeat_offset;
#else #error Unsupported platform
#endif
    (*repeat_unicode)(input_obj);
}

void Zinc_input_unicode_seek(
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable,
    Zinc_location* loc)
{
#if IS_UNIX
    Zinc_input_unicode_seek_interface *seek = input_obj + input_vtable->seek_offset;
#elif IS_WIN
    Zinc_input_unicode_seek_interface *seek = (uint8_t*)input_obj + input_vtable->seek_offset;
#else #error "unsupported platform"
#endif
    (*seek)(input_obj, loc);
}

void Zinc_input_unicode_get_all(
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable,
    Zinc_vector** text)
{
#if IS_UNIX
    Zinc_input_unicode_get_all_interface *get_all = input_obj + input_vtable->get_all_offset;
#elif IS_WIN
    Zinc_input_unicode_get_all_interface *get_all = (uint8_t*)input_obj + input_vtable->get_all_offset;
#else #error "unsupported platform"
#endif
    (*get_all)(input_obj, text);
}

Zinc_location Zinc_input_unicode_get_location(
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable)
{
#if IS_UNIX
    Zinc_input_unicode_get_location_interface *get_location =
        input_obj + input_vtable->get_location_offset;
#elif IS_WIN
    Zinc_input_unicode_get_location_interface *get_location =
        (uint8_t*)input_obj + input_vtable->get_location_offset;
#else #error "unsupported OS"
#endif
    return (*get_location)(input_obj);
}

void Zinc_input_unicode_destroy(void* input_obj, Zinc_input_unicode_vtable* input_vtable)
{
#if IS_UNIX
    Zinc_input_unicode_destroy_interface *destroy = input_obj + input_vtable->destroy_offset;
#elif IS_WIN
    Zinc_input_unicode_destroy_interface *destroy = (uint8_t*)input_obj + input_vtable->destroy_offset;
#else #error "unsupported OS"
#endif
    (*destroy)(input_obj);
}