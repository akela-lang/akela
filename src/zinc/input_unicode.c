#include <stdbool.h>
#include "input_unicode.h"
#include "result.h"

struct Zinc_location* InputUnicodeLocation(void* input_obj, InputUnicodeVTable* input_vtable)
{
    return input_obj + input_vtable->loc_offset;
}

enum result InputUnicodeNext(void* input_obj,
                      InputUnicodeVTable* input_vtable,
                      char c[4],
                      int* num,
                      struct Zinc_location* loc,
                      bool* done)
{
    InputUnicodeNextInterface *get_unicode = input_obj + input_vtable->next_offset;
    return (*get_unicode)(input_obj, c, num, loc, done);
}

void InputUnicodeRepeat(void* input_obj, InputUnicodeVTable* input_vtable)
{
    InputUnicodeRepeatInterface *repeat_unicode = input_obj + input_vtable->repeat_offset;
    (*repeat_unicode)(input_obj);
}

void InputUnicodeSeek(void* input_obj, InputUnicodeVTable* input_vtable, struct Zinc_location* loc)
{
    InputUnicodeSeekInterface *seek = input_obj + input_vtable->seek_offset;
    (*seek)(input_obj, loc);
}

void InputUnicodeGetAll(void* input_obj, InputUnicodeVTable* input_vtable, Vector** text)
{
    InputUnicodeGetAllInterface *get_all = input_obj + input_vtable->get_all_offset;
    (*get_all)(input_obj, text);
}

struct Zinc_location InputUnicodeGetLocation(void* input_obj, InputUnicodeVTable* input_vtable)
{
    InputUnicodeGetLocationInterface *get_location = input_obj + input_vtable->get_location_offset;
    return (*get_location)(input_obj);
}