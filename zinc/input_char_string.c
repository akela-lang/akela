#include "input_char_string.h"
#include "memory.h"
#include <stddef.h>

InputCharVTable InputCharStringVTable = {
        .loc_offset = offsetof(InputCharString, loc),
        .next_offset = offsetof(InputCharString, Next),
        .repeat_offset = offsetof(InputCharString, Repeat),
        .seek_offset = offsetof(InputCharString, Seek),
        .get_all_offset = offsetof(InputCharString, GetAll),
        .get_location_offset = offsetof(InputCharString, GetLocation),
};

void InputCharStringInit(InputCharString* input_string, Vector* text)
{
    location_init(&input_string->loc);
    location_init(&input_string->prev_loc);
    input_string->repeat_char = false;
    input_string->pos = 0;
    input_string->text = text;
    input_string->Next = (InputCharNextInterface) InputCharStringNext;
    input_string->Repeat = (InputCharRepeatInterface) InputCharStringRepeat;
    input_string->Seek = (InputCharSeekInterface) InputCharStringSeek;
    input_string->GetAll = (InputCharGetAllInterface) InputCharStringGetAll;
    input_string->GetLocation = (InputCharGetLocationInterface) InputCharStringGetLocation;
    input_string->input_vtable = &InputCharStringVTable;
}

void InputCharStringCreate(InputCharString** input_string, Vector* text)
{
    malloc_safe((void**)input_string, sizeof(InputCharString));
    InputCharStringInit(*input_string, text);
}

void InputCharStringClear(InputCharString* data)
{
    location_init(&data->loc);
    location_init(&data->prev_loc);
    data->loc.end_pos = 1;
    data->loc.line = 1;
    data->loc.col = 1;
}

/**
 * Get the next character.
 * @param lex_data lexer data
 * @param c the next char
 * @return done
 */
bool InputCharStringNext(InputCharString* data, char* c, struct location* loc)
{
    if (data->loc.start_pos == 0) {
        InputCharStringClear(data);
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
void InputCharStringRepeat(InputCharString* data)
{
    data->repeat_char = true;
}

/**
 * Seek to position in data stream. This call invalidates location data.
 * @param data the data
 * @param pos position to go to
 */
void InputCharStringSeek(InputCharString* data, struct location* loc)
{
    if (loc->start_pos < data->text->count)
    {
        InputCharStringClear(data);
        data->loc = *loc;
        data->prev_loc = *loc;
        data->pos = loc->start_pos;
    }
}

/**
 * Get entire input stream.
 * @param data the data
 * @param v the output vector
 */
void InputCharStringGetAll(InputCharString* data, Vector** text)
{
    InputCharStringClear(data);
    data->pos = data->text->count;
    *text = data->text;
}

struct location InputCharStringGetLocation(InputCharString* data)
{
    return data->loc;
}