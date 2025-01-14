#include <stdbool.h>
#include "input_char.h"

struct Zinc_location* InputCharLocation(void* input_obj, InputCharVTable* input_vtable)
{
    return input_obj + input_vtable->loc_offset;
}

bool InputCharNext(void* input_obj, InputCharVTable* input_vtable, char* c, struct Zinc_location* loc)
{
    InputCharNextInterface *get_char = input_obj + input_vtable->next_offset;
    return (*get_char)(input_obj, c, loc);
}

void InputCharRepeat(void* input_obj, InputCharVTable* input_vtable)
{
    InputCharRepeatInterface *repeat_char = input_obj + input_vtable->repeat_offset;
    (*repeat_char)(input_obj);
}

void InputCharSeek(void* input_obj, InputCharVTable* input_vtable, struct Zinc_location* loc)
{
    InputCharSeekInterface *seek = input_obj + input_vtable->seek_offset;
    (*seek)(input_obj, loc);
}

void InputCharGetAll(void* input_obj, InputCharVTable* input_vtable, Vector** text)
{
    InputCharGetAllInterface *get_all = input_obj + input_vtable->get_all_offset;
    (*get_all)(input_obj, text);
}

struct Zinc_location InputCharGetLocation(void* input_obj, InputCharVTable* input_vtable)
{
    InputCharGetLocationInterface *get_location = input_obj + input_vtable->get_location_offset;
    return (*get_location)(input_obj);
}