#ifndef ZINC_INPUT_UNICODE_FILE_H
#define ZINC_INPUT_UNICODE_FILE_H

#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "vector.h"
#include "input_unicode.h"
#include <stdio.h>

typedef struct {
    Zinc_location loc;
    bool repeat_char;
    char prev_c[4];
    int prev_num;
    Zinc_location prev_loc;
    bool prev_done;
    FILE* fp;
    Zinc_input_unicode_next_interface Next;
    Zinc_input_unicode_repeat_interface Repeat;
    Zinc_input_unicode_seek_interface Seek;
    Zinc_input_unicode_get_all_interface GetAll;
    Zinc_input_unicode_get_location_interface GetLocation;
    Zinc_input_unicode_vtable* input_vtable;
} Zinc_input_unicode_file;

extern Zinc_input_unicode_vtable Zinc_input_unicode_file_vtable;

void Zinc_input_unicode_file_create(Zinc_input_unicode_file** input_string, FILE* fp);
void Zinc_input_unicode_file_clear(Zinc_input_unicode_file* data);
enum result Zinc_input_unicode_file_next(
        Zinc_input_unicode_file* data,
        char c[4],
        int* num,
        Zinc_location* loc,
        bool* done);
void Zinc_input_unicode_file_repeat(Zinc_input_unicode_file* data);
void Zinc_input_unicode_file_seek(Zinc_input_unicode_file* data, Zinc_location* loc);
void Zinc_input_unicode_file_get_all(Zinc_input_unicode_file* data, Vector** text);
Zinc_location Zinc_input_unicode_file_get_location(Zinc_input_unicode_file* input);

#endif