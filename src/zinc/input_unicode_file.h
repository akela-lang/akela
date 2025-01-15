#ifndef ZINC_INPUT_UNICODE_FILE_H
#define ZINC_INPUT_UNICODE_FILE_H

#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "vector.h"
#include "input_unicode.h"
#include <stdio.h>

typedef struct {
    struct Zinc_location loc;
    bool repeat_char;
    char prev_c[4];
    int prev_num;
    struct Zinc_location prev_loc;
    bool prev_done;
    FILE* fp;
    Zinc_input_unicode_next_interface Next;
    Zinc_input_unicode_repeat_interface Repeat;
    Zinc_input_unicode_seek_interface Seek;
    Zinc_input_unicode_get_all_interface GetAll;
    Zinc_input_unicode_get_location_interface GetLocation;
    Zinc_input_unicode_vtable* input_vtable;
} InputUnicodeFile;

extern Zinc_input_unicode_vtable InputUnicodeFileVTable;

void InputUnicodeFileCreate(InputUnicodeFile** input_string, FILE* fp);
void InputUnicodeFileClear(InputUnicodeFile* data);
enum result InputUnicodeFileNext(
        InputUnicodeFile* data,
        char c[4],
        int* num,
        struct Zinc_location* loc,
        bool* done);
void InputUnicodeFileRepeat(InputUnicodeFile* data);
void InputUnicodeFileSeek(InputUnicodeFile* data, struct Zinc_location* loc);
void InputUnicodeFileGetAll(InputUnicodeFile* data, Vector** text);
struct Zinc_location InputUnicodeFileGetLocation(InputUnicodeFile* input);

#endif