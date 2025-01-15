#ifndef ZINC_INPUT_UNICODE_STRING_H
#define ZINC_INPUT_UNICODE_STRING_H

#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "vector.h"
#include "input_unicode.h"

typedef struct {
    struct Zinc_location loc;
    struct Zinc_location prev_loc;
    bool repeat_char;
    size_t pos;
    Vector* text;
    Zinc_input_unicode_next_interface Next;
    Zinc_input_unicode_repeat_interface Repeat;
    Zinc_input_unicode_seek_interface Seek;
    Zinc_input_unicode_get_all_interface GetAll;
    Zinc_input_unicode_get_location_interface GetLocation;
    Zinc_input_unicode_vtable* input_vtable;
} InputUnicodeString;

extern Zinc_input_unicode_vtable InputUnicodeStringVTable;

void InputUnicodeStringInit(InputUnicodeString* input_string, Vector* text);
void InputUnicodeStringCreate(InputUnicodeString** input_string, Vector* text);
void InputUnicodeStringClear(InputUnicodeString* data);
enum result InputUnicodeStringNext(InputUnicodeString* data,
        char c[4],
        int* num,
        struct Zinc_location* loc,
        bool* done);
void InputUnicodeStringRepeat(InputUnicodeString* data);
void InputUnicodeStringSeek(InputUnicodeString* data, struct Zinc_location* loc);
void InputUnicodeStringGetAll(InputUnicodeString* data, Vector** text);
struct Zinc_location InputUnicodeStringGetLocation(InputUnicodeString* data);

#endif