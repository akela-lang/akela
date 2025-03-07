#include <stdbool.h>
#include "input_char.h"

Zinc_location* Zinc_input_char_location(void* input_obj, Zinc_input_char_vtable* input_vtable)
{
    return (Zinc_location*)((uint8_t*)input_obj + input_vtable->loc_offset);
}

bool Zinc_input_char_next(
    void* input_obj,
    Zinc_input_char_vtable* input_vtable,
    char* c,
    Zinc_location* loc)
{
    Zinc_input_char_next_interface *get_char =
        (Zinc_input_char_next_interface*)((uint8_t*)input_obj + input_vtable->next_offset);
    return (*get_char)(input_obj, c, loc);
}

void Zinc_input_char_repeat(void* input_obj, Zinc_input_char_vtable* input_vtable)
{
    Zinc_input_char_repeat_interface *repeat_char =
        (Zinc_input_char_repeat_interface*)((uint8_t*)input_obj + input_vtable->repeat_offset);
    (*repeat_char)(input_obj);
}

void Zinc_input_char_seek(void* input_obj, Zinc_input_char_vtable* input_vtable, Zinc_location* loc)
{
    Zinc_input_char_seek_interface *seek =
        (Zinc_input_char_seek_interface*)((uint8_t*)input_obj + input_vtable->seek_offset);
    (*seek)(input_obj, loc);
}

void Zinc_input_char_get_all(void* input_obj, Zinc_input_char_vtable* input_vtable, Zinc_vector** text)
{
    Zinc_input_char_get_all_interface *get_all =
        (Zinc_input_char_get_all_interface*)((uint8_t*)input_obj + input_vtable->get_all_offset);
    (*get_all)(input_obj, text);
}

Zinc_location Zinc_input_input_char_get_location(void* input_obj, Zinc_input_char_vtable* input_vtable)
{
    Zinc_input_char_get_location_interface *get_location =
        (Zinc_input_char_get_location_interface*)((uint8_t*)input_obj + input_vtable->get_location_offset);
    return (*get_location)(input_obj);
}