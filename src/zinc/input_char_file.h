#ifndef ZINC_INPUT_CHAR_FILE_H
#define ZINC_INPUT_CHAR_FILE_H

#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "vector.h"
#include "input_char.h"
#include <stdio.h>

typedef struct {
    struct Zinc_location loc;
    struct Zinc_location prev_loc;
    bool repeat_char;
    char prev_c;
    FILE* fp;
    Zinc_input_char_next_interface Next;
    Zinc_input_char_repeat_interface Repeat;
    Zinc_input_char_seek_interface Seek;
    Zinc_input_char_get_all_interface GetAll;
    Zinc_input_char_get_location_interface GetLocation;
    Zinc_input_char_vtable* input_vtable;
} InputCharFile;

extern Zinc_input_char_vtable InputCharFileVTable;

void InputCharFileCreate(InputCharFile** input_string, FILE* fp);
void InputCharFileClear(InputCharFile* data);
bool InputCharFileNext(InputCharFile* data, char* c, struct Zinc_location* loc);
void InputCharFileRepeat(InputCharFile* data);
void InputCharFileSeek(InputCharFile* data, struct Zinc_location* loc);
void InputCharFileGetAll(InputCharFile* data, Vector** text);
struct Zinc_location InputCharFileGetLocation(InputCharFile* input);

#endif