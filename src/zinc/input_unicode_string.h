#ifndef ZINC_INPUT_UNICODE_STRING_H
#define ZINC_INPUT_UNICODE_STRING_H

#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "vector.h"
#include "input_unicode.h"

typedef struct {
    struct location loc;
    struct location prev_loc;
    bool repeat_char;
    size_t pos;
    Vector* text;
    InputUnicodeNextInterface Next;
    InputUnicodeRepeatInterface Repeat;
    InputUnicodeSeekInterface Seek;
    InputUnicodeGetAllInterface GetAll;
    InputUnicodeGetLocationInterface GetLocation;
    InputUnicodeVTable* input_vtable;
} InputUnicodeString;

extern InputUnicodeVTable InputUnicodeStringVTable;

void InputUnicodeStringInit(InputUnicodeString* input_string, Vector* text);
void InputUnicodeStringCreate(InputUnicodeString** input_string, Vector* text);
void InputUnicodeStringClear(InputUnicodeString* data);
enum result InputUnicodeStringNext(InputUnicodeString* data,
        char c[4],
        int* num,
        struct location* loc,
        bool* done);
void InputUnicodeStringRepeat(InputUnicodeString* data);
void InputUnicodeStringSeek(InputUnicodeString* data, struct location* loc);
void InputUnicodeStringGetAll(InputUnicodeString* data, Vector** text);
struct location InputUnicodeStringGetLocation(InputUnicodeString* data);

#endif