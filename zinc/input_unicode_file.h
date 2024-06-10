#ifndef ZINC_INPUT_UNICODE_FILE_H
#define ZINC_INPUT_UNICODE_FILE_H

#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "vector.h"
#include "input_unicode.h"
#include <stdio.h>

typedef struct {
    struct location loc;
    bool repeat_char;
    char prev_c[4];
    int prev_num;
    struct location prev_loc;
    bool prev_done;
    FILE* fp;
    InputUnicodeNextInterface Next;
    InputUnicodeRepeatInterface Repeat;
    InputUnicodeSeekInterface Seek;
    InputUnicodeGetAllInterface GetAll;
    InputUnicodeGetLocationInterface GetLocation;
    InputUnicodeVTable* input_vtable;
} InputUnicodeFile;

extern InputUnicodeVTable InputUnicodeFileVTable;

void InputUnicodeFileCreate(InputUnicodeFile** input_string, FILE* fp);
void InputUnicodeFileClear(InputUnicodeFile* data);
enum result InputUnicodeFileNext(
        InputUnicodeFile* data,
        char c[4],
        int* num,
        struct location* loc,
        bool* done);
void InputUnicodeFileRepeat(InputUnicodeFile* data);
void InputUnicodeFileSeek(InputUnicodeFile* data, struct location* loc);
void InputUnicodeFileGetAll(InputUnicodeFile* data, Vector** text);
struct location InputUnicodeFileGetLocation(InputUnicodeFile* input);

#endif