#ifndef __ZINC_INPUT_CHAR_STRING_H__
#define __ZINC_INPUT_CHAR_STRING_H__

#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "vector.h"
#include "input_char.h"

typedef struct {
    struct Zinc_location loc;
    struct Zinc_location prev_loc;
    bool repeat_char;
    size_t pos;
    Vector* text;
    Zinc_input_char_next_interface Next;
    Zinc_input_char_repeat_interface Repeat;
    Zinc_input_char_seek_interface Seek;
    Zinc_input_char_get_all_interface GetAll;
    Zinc_input_char_get_location_interface GetLocation;
    Zinc_input_char_vtable* input_vtable;
} InputCharString;

extern Zinc_input_char_vtable InputCharStringVTable;

void InputCharStringInit(InputCharString* input_string, Vector* text);
void InputCharStringCreate(InputCharString** input_string, Vector* text);
bool InputCharStringNext(InputCharString* data, char* c, struct Zinc_location* loc);
void InputCharStringRepeat(InputCharString* data);
void InputCharStringSeek(InputCharString* data, struct Zinc_location* loc);
void InputCharStringGetAll(InputCharString* data, Vector** text);
struct Zinc_location InputCharStringGetLocation(InputCharString* data);

#endif