#ifndef ZINC_INPUT_CHAR_FILE_H
#define ZINC_INPUT_CHAR_FILE_H

#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "vector.h"
#include "input_char.h"
#include <stdio.h>

typedef struct {
    Zinc_location loc;
    Zinc_location prev_loc;
    bool repeat_char;
    char prev_c;
    FILE* fp;
    Zinc_input_char_next_interface Next;
    Zinc_input_char_repeat_interface Repeat;
    Zinc_input_char_seek_interface Seek;
    Zinc_input_char_get_all_interface GetAll;
    Zinc_input_char_get_location_interface GetLocation;
    Zinc_input_char_vtable* input_vtable;
} Zinc_input_char_file;

extern Zinc_input_char_vtable Zinc_input_char_file_vtable;

void Zinc_input_char_file_create(Zinc_input_char_file** input_string, FILE* fp);
void Zinc_input_char_file_clear(Zinc_input_char_file* data);
bool Zinc_input_char_file_next(Zinc_input_char_file* data, char* c, Zinc_location* loc);
void Zinc_input_char_file_repeat(Zinc_input_char_file* data);
void Zinc_input_char_file_seek(Zinc_input_char_file* data, Zinc_location* loc);
void Zinc_input_char_file_get_all(Zinc_input_char_file* data, Vector** text);
Zinc_location Zinc_input_char_file_get_location(Zinc_input_char_file* input);

#endif