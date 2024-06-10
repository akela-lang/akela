#ifndef ZINC_INPUT_CHAR_FILE_H
#define ZINC_INPUT_CHAR_FILE_H

#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "vector.h"
#include "input_char.h"
#include <stdio.h>

typedef struct {
    struct location loc;
    struct location prev_loc;
    bool repeat_char;
    char prev_c;
    FILE* fp;
    InputCharNextInterface Next;
    InputCharRepeatInterface Repeat;
    InputCharSeekInterface Seek;
    InputCharGetAllInterface GetAll;
    InputCharGetLocationInterface GetLocation;
    InputCharVTable* input_vtable;
} InputCharFile;

extern InputCharVTable InputCharFileVTable;

void InputCharFileCreate(InputCharFile** input_string, FILE* fp);
void InputCharFileClear(InputCharFile* data);
bool InputCharFileNext(InputCharFile* data, char* c, struct location* loc);
void InputCharFileRepeat(InputCharFile* data);
void InputCharFileSeek(InputCharFile* data, struct location* loc);
void InputCharFileGetAll(InputCharFile* data, Vector** text);
struct location InputCharFileGetLocation(InputCharFile* input);

#endif