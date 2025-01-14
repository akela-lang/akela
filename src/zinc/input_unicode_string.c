#include "input_unicode_string.h"
#include "memory.h"
#include <stddef.h>
#include "utf8.h"

InputUnicodeVTable InputUnicodeStringVTable = {
        .loc_offset = offsetof(InputUnicodeString, loc),
        .next_offset = offsetof(InputUnicodeString, Next),
        .repeat_offset = offsetof(InputUnicodeString, Repeat),
        .seek_offset = offsetof(InputUnicodeString, Seek),
        .get_all_offset = offsetof(InputUnicodeString, GetAll),
        .get_location_offset = offsetof(InputUnicodeString, GetLocation),
};

void InputUnicodeStringInit(InputUnicodeString* input_string, Vector* text)
{
    Zinc_location_init(&input_string->loc);
    Zinc_location_init(&input_string->prev_loc);
    input_string->repeat_char = false;
    input_string->pos = 0;
    input_string->text = text;
    input_string->Next = (InputUnicodeNextInterface) InputUnicodeStringNext;
    input_string->Repeat = (InputUnicodeRepeatInterface) InputUnicodeStringRepeat;
    input_string->Seek = (InputUnicodeSeekInterface) InputUnicodeStringSeek;
    input_string->GetAll = (InputUnicodeGetAllInterface) InputUnicodeStringGetAll;
    input_string->GetLocation = (InputUnicodeGetLocationInterface) InputUnicodeStringGetLocation;
    input_string->input_vtable = &InputUnicodeStringVTable;
}

void InputUnicodeStringCreate(InputUnicodeString** input_string, Vector* text)
{
    malloc_safe((void**)input_string, sizeof(InputUnicodeString));
    InputUnicodeStringInit(*input_string, text);
}

void InputUnicodeStringClear(InputUnicodeString* data)
{
    Zinc_location_init(&data->loc);
    Zinc_location_init(&data->prev_loc);
    data->loc.line = 1;
    data->loc.col = 1;
}

/**
 * Get the next character.
 * @param lex_data lexer data
 * @param c the next char
 * @return done
 */
enum result InputUnicodeStringNext(
        InputUnicodeString* data,
        char c[4],
        int* num,
        struct Zinc_location* loc,
        bool* done)
{
    enum result r = result_ok;

    if (data->loc.line == 0) {
        InputUnicodeStringClear(data);
    }

    if (data->repeat_char && data->pos > 0) {
        data->pos = data->prev_loc.start_pos;
        data->repeat_char = false;
        data->loc = data->prev_loc;
        *loc = data->loc;
    }

    data->prev_loc = data->loc;
    if (data->pos < data->text->count) {
        /* first byte */
        c[0] = VECTOR_CHAR(data->text, data->pos++);
        *num = NUM_BYTES(c[0]);
        *loc = data->loc;
        data->loc.start_pos++;
        if (c[0] == '\n') {
            data->loc.line++;
            data->loc.col = 1;
        } else {
            data->loc.col++;
        }

        /* other bytes */
        *done = false;
        for (int i = 1; i < *num; i++) {
            if (data->pos < data->text->count) {
                c[i] = VECTOR_CHAR(data->text, data->pos++);
                data->loc.start_pos++;
                if (!IS_EXTRA_BYTE(c[i])) {
                    r = set_error("invalid utf8 trailing byte");
                }
            } else {
                *done = true;
                r = set_error("EOF before utf8 trailing byte(s)");
                break;
            }
        }
        loc->end_pos = data->loc.start_pos;
    } else {
        *num = 0;
        *loc = data->loc;
        loc->end_pos = loc->start_pos + 1;
        *done = true;
    }

    return r;
}

/**
 * Repeat the previous character.
 * @param data lexer data
 */
void InputUnicodeStringRepeat(InputUnicodeString* data)
{
    data->repeat_char = true;
    data->loc = data->prev_loc;
}

/**
 * Seek to position in data stream. This call invalidates location data.
 * @param data the data
 * @param pos position to go to
 */
void InputUnicodeStringSeek(InputUnicodeString* data, struct Zinc_location* loc)
{
    if (loc->start_pos < data->text->count)
    {
        InputUnicodeStringClear(data);
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
void InputUnicodeStringGetAll(InputUnicodeString* data, Vector** text)
{
    InputUnicodeStringClear(data);
    data->pos = data->text->count;
    *text = data->text;
}

struct Zinc_location InputUnicodeStringGetLocation(InputUnicodeString* data)
{
    return data->loc;
}