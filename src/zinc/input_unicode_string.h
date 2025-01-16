#ifndef ZINC_INPUT_UNICODE_STRING_H
#define ZINC_INPUT_UNICODE_STRING_H

#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "vector.h"
#include "input_unicode.h"

typedef struct {
    Zinc_location loc;
    Zinc_location prev_loc;
    bool repeat_char;
    size_t pos;
    Zinc_vector* text;
    Zinc_input_unicode_next_interface Next;
    Zinc_input_unicode_repeat_interface Repeat;
    Zinc_input_unicode_seek_interface Seek;
    Zinc_input_unicode_get_all_interface GetAll;
    Zinc_input_unicode_get_location_interface GetLocation;
    Zinc_input_unicode_vtable* input_vtable;
} Zinc_input_unicode_string;

extern Zinc_input_unicode_vtable Zinc_input_unicode_string_vtable;

void Zinc_input_unicode_string_init(Zinc_input_unicode_string* input_string, Zinc_vector* text);
void Zinc_input_unicode_string_create(Zinc_input_unicode_string** input_string, Zinc_vector* text);
void Zinc_input_unicode_string_clear(Zinc_input_unicode_string* data);
enum Zinc_result Zinc_input_unicode_string_next(Zinc_input_unicode_string* data,
        char c[4],
        int* num,
        Zinc_location* loc,
        bool* done);
void Zinc_input_unicode_string_repeat(Zinc_input_unicode_string* data);
void Zinc_input_unicode_string_seek(Zinc_input_unicode_string* data, Zinc_location* loc);
void Zinc_input_unicode_string_get_all(Zinc_input_unicode_string* data, Zinc_vector** text);
Zinc_location Zinc_input_unicode_string_get_location(Zinc_input_unicode_string* data);

#endif