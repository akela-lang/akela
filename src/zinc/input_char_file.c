#include "input_char_file.h"
#include "memory.h"
#include <stddef.h>
#include "vector.h"

Zinc_input_char_vtable Zinc_input_char_file_vtable = {
        .loc_offset = offsetof(Zinc_input_char_file, loc),
        .next_offset = offsetof(Zinc_input_char_file, Next),
        .repeat_offset = offsetof(Zinc_input_char_file, Repeat),
        .seek_offset = offsetof(Zinc_input_char_file, Seek),
        .get_all_offset = offsetof(Zinc_input_char_file, GetAll),
        .get_location_offset = offsetof(Zinc_input_char_file, GetLocation),
};

void Zinc_input_char_file_init(Zinc_input_char_file* input, FILE* fp)
{
    Zinc_location_init(&input->loc);
    Zinc_location_init(&input->prev_loc);
    input->repeat_char = false;
    input->prev_c = 0;
    input->fp = fp;
    input->Next = (Zinc_input_char_next_interface) Zinc_input_char_file_next;
    input->Repeat = (Zinc_input_char_repeat_interface) Zinc_input_char_file_repeat;
    input->Seek = (Zinc_input_char_seek_interface) Zinc_input_char_file_seek;
    input->GetAll = (Zinc_input_char_get_all_interface) Zinc_input_char_file_get_all;
    input->GetLocation = (Zinc_input_char_get_location_interface) Zinc_input_char_file_get_location;
    input->input_vtable = &Zinc_input_char_file_vtable;
}

void Zinc_input_char_file_create(Zinc_input_char_file** input, FILE* fp)
{
    Zinc_malloc_safe((void**)input, sizeof(Zinc_input_char_file));
    Zinc_input_char_file_init(*input, fp);
}

void Zinc_input_char_file_clear(Zinc_input_char_file* input)
{
    Zinc_location_init(&input->loc);
    Zinc_location_init(&input->prev_loc);
    input->loc.line = 1;
    input->loc.col = 1;
    input->loc.end_pos = 1;
}

/**
 * Get the next character.
 * @param lex_data lexer data
 * @param c the next char
 * @return done
 */
bool Zinc_input_char_file_next(Zinc_input_char_file* input, char* c, struct Zinc_location* loc)
{
    if (input->loc.start_pos == 0) {
        Zinc_input_char_file_clear(input);
    }

    if (input->repeat_char) {
        input->loc = input->prev_loc;
        input->repeat_char = false;
        *c = input->prev_c;
        *loc = input->loc;
    } else {
        int x = getc(input->fp);
        *loc = input->loc;
        if (x == EOF) {
            return true;
        }
        *c = (char)x;
        input->prev_c = *c;
    }

    input->prev_loc = input->loc;
    input->loc.start_pos++;
    input->loc.end_pos = input->loc.start_pos + 1;
    if (*c == '\n') {
        input->loc.line++;
        input->loc.col = 1;
    } else {
        input->loc.col++;
    }
    return false;
}

/**
 * Repeat the previous character.
 * @param input lexer data
 */
void Zinc_input_char_file_repeat(Zinc_input_char_file* input)
{
    input->repeat_char = true;
}

/**
 * Seek to position in data stream. This call invalidates location data.
 * @param input the input
 * @param pos position to go to
 */
void Zinc_input_char_file_seek(Zinc_input_char_file* input, struct Zinc_location* loc)
{
    input->loc = *loc;
    input->prev_loc = *loc;
    fseek(input->fp, (long)loc->start_pos, SEEK_SET);
}

/**
 * Get entire input stream.
 * @param input the input
 * @param v the output vector
 */
void Zinc_input_char_file_get_all(Zinc_input_char_file* input, Zinc_vector** text)
{
    Zinc_input_char_file_clear(input);
    Zinc_vector* v = NULL;
    Zinc_vector_create(&v, sizeof(char));
    fseek(input->fp, 0, SEEK_SET);
    do {
        int x = getc(input->fp);
        if (x == EOF) break;
        char c = (char)x;
        Zinc_vector_add(v, &c, 1);
    } while (true);
    Zinc_vector_add_null(v);
    *text = v;
}

Zinc_location Zinc_input_char_file_get_location(Zinc_input_char_file* input)
{
    return input->loc;
}