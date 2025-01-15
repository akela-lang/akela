#include "input_char_string.h"
#include "memory.h"
#include <stddef.h>

Zinc_input_char_vtable Zinc_input_char_stringVTable = {
        .loc_offset = offsetof(Zinc_input_char_string, loc),
        .next_offset = offsetof(Zinc_input_char_string, Next),
        .repeat_offset = offsetof(Zinc_input_char_string, Repeat),
        .seek_offset = offsetof(Zinc_input_char_string, Seek),
        .get_all_offset = offsetof(Zinc_input_char_string, GetAll),
        .get_location_offset = offsetof(Zinc_input_char_string, GetLocation),
};

void Zinc_input_char_string_init(Zinc_input_char_string* input_string, Vector* text)
{
    Zinc_location_init(&input_string->loc);
    Zinc_location_init(&input_string->prev_loc);
    input_string->repeat_char = false;
    input_string->pos = 0;
    input_string->text = text;
    input_string->Next = (Zinc_input_char_next_interface)Zinc_input_char_string_next;
    input_string->Repeat = (Zinc_input_char_repeat_interface)Zinc_input_char_string_repeat;
    input_string->Seek = (Zinc_input_char_seek_interface)Zinc_input_char_string_seek;
    input_string->GetAll = (Zinc_input_char_get_all_interface)Zinc_input_char_string_get_all;
    input_string->GetLocation =
        (Zinc_input_char_get_location_interface)Zinc_input_char_string_get_location;
    input_string->input_vtable = &Zinc_input_char_stringVTable;
}

void Zinc_input_char_string_create(Zinc_input_char_string** input_string, Vector* text)
{
    malloc_safe((void**)input_string, sizeof(Zinc_input_char_string));
    Zinc_input_char_string_init(*input_string, text);
}

void Zinc_input_char_string_clear(Zinc_input_char_string* data)
{
    Zinc_location_init(&data->loc);
    Zinc_location_init(&data->prev_loc);
    data->loc.end_pos = 1;
    data->loc.line = 1;
    data->loc.col = 1;
}

/**
 * Get the next character.
 * @param data lexer data
 * @param c the next char
 * @param loc the location
 * @return done
 */
bool Zinc_input_char_string_next(Zinc_input_char_string* data, char* c, Zinc_location* loc)
{
    if (data->loc.start_pos == 0) {
        Zinc_input_char_string_clear(data);
    }

    if (data->repeat_char && data->pos > 0) {
        data->pos--;
        data->loc = data->prev_loc;
        data->repeat_char = false;
    }

    data->prev_loc = data->loc;
    if (data->pos < data->text->count) {
        *c = VECTOR_CHAR(data->text, data->pos++);
        *loc = data->loc;
        data->loc.start_pos++;
        data->loc.end_pos = data->loc.start_pos + 1;
        if (*c == '\n') {
            data->loc.line++;
            data->loc.col = 1;
        } else {
            data->loc.col++;
        }
        return false;
    } else {
        *c = 0;
        *loc = data->loc;
        return true;
    }
}

/**
 * Repeat the previous character.
 * @param data lexer data
 */
void Zinc_input_char_string_repeat(Zinc_input_char_string* data)
{
    data->repeat_char = true;
}

/**
 * Seek to position in data stream. This call invalidates location data.
 * @param data the data
 * @param loc the location to go to
 */
void Zinc_input_char_string_seek(Zinc_input_char_string* data, Zinc_location* loc)
{
    if (loc->start_pos < data->text->count)
    {
        Zinc_input_char_string_clear(data);
        data->loc = *loc;
        data->prev_loc = *loc;
        data->pos = loc->start_pos;
    }
}

/**
 * Get entire input stream.
 * @param data the data
 * @param text the output vector
 */
void Zinc_input_char_string_get_all(Zinc_input_char_string* data, Vector** text)
{
    Zinc_input_char_string_clear(data);
    data->pos = data->text->count;
    *text = data->text;
}

Zinc_location Zinc_input_char_string_get_location(Zinc_input_char_string* data)
{
    return data->loc;
}