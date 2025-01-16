#ifndef __ZINC_INPUT_CHAR_STRING_H__
#define __ZINC_INPUT_CHAR_STRING_H__

#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "vector.h"
#include "input_char.h"

typedef struct {
    Zinc_location loc;
    Zinc_location prev_loc;
    bool repeat_char;
    size_t pos;
    Zinc_vector* text;
    Zinc_input_char_next_interface Next;
    Zinc_input_char_repeat_interface Repeat;
    Zinc_input_char_seek_interface Seek;
    Zinc_input_char_get_all_interface GetAll;
    Zinc_input_char_get_location_interface GetLocation;
    Zinc_input_char_vtable* input_vtable;
} Zinc_input_char_string;

extern Zinc_input_char_vtable InputCharStringVTable;

void Zinc_input_char_string_init(Zinc_input_char_string* input_string, Zinc_vector* text);
void Zinc_input_char_string_create(Zinc_input_char_string** input_string, Zinc_vector* text);
bool Zinc_input_char_string_next(Zinc_input_char_string* data, char* c, Zinc_location* loc);
void Zinc_input_char_string_repeat(Zinc_input_char_string* data);
void Zinc_input_char_string_seek(Zinc_input_char_string* data, Zinc_location* loc);
void Zinc_input_char_string_get_all(Zinc_input_char_string* data, Zinc_vector** text);
Zinc_location Zinc_input_char_string_get_location(Zinc_input_char_string* data);

#endif