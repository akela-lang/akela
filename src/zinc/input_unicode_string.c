#include "input_unicode_string.h"
#include "memory.h"
#include <stddef.h>
#include "utf8.h"

Zinc_input_unicode_vtable Zinc_input_unicode_string_vtable = {
    .loc_offset = offsetof(Zinc_input_unicode_string, loc),
    .next_offset = offsetof(Zinc_input_unicode_string, Next),
    .repeat_offset = offsetof(Zinc_input_unicode_string, Repeat),
    .seek_offset = offsetof(Zinc_input_unicode_string, Seek),
    .get_all_offset = offsetof(Zinc_input_unicode_string, GetAll),
    .get_location_offset = offsetof(Zinc_input_unicode_string, GetLocation),
    .destroy_offset = offsetof(Zinc_input_unicode_string, Destroy),
};

void Zinc_input_unicode_string_init(Zinc_input_unicode_string* input_string, Zinc_vector* text)
{
    Zinc_location_init(&input_string->loc);
    Zinc_location_init(&input_string->prev_loc);
    input_string->repeat_char = false;
    input_string->pos = 0;
    input_string->text = text;
    input_string->Next = (Zinc_input_unicode_next_interface) Zinc_input_unicode_string_next;
    input_string->Repeat = (Zinc_input_unicode_repeat_interface) Zinc_input_unicode_string_repeat;
    input_string->Seek = (Zinc_input_unicode_seek_interface) Zinc_input_unicode_string_seek;
    input_string->GetAll = (Zinc_input_unicode_get_all_interface) Zinc_input_unicode_string_get_all;
    input_string->GetLocation =
        (Zinc_input_unicode_get_location_interface) Zinc_input_unicode_string_get_location;
    input_string->Destroy = (Zinc_input_unicode_destroy_interface) Zinc_input_unicode_string_destroy;
    input_string->vtable = &Zinc_input_unicode_string_vtable;
}

void Zinc_input_unicode_string_create(Zinc_input_unicode_string** input_string, Zinc_vector* text)
{
    Zinc_malloc_safe((void**)input_string, sizeof(Zinc_input_unicode_string));
    Zinc_input_unicode_string_init(*input_string, text);
}

void Zinc_input_unicode_string_clear(Zinc_input_unicode_string* data)
{
    Zinc_location_init(&data->loc);
    Zinc_location_init(&data->prev_loc);
    data->loc.line = 1;
    data->loc.col = 1;
}

/**
 * Get the next character.
 * @param data lexer data
 * @param c the next char
 * @param num byte count
 * @param loc location
 * @param done stream is done
 * @return result
 */
enum Zinc_result Zinc_input_unicode_string_next(
        Zinc_input_unicode_string* data,
        char c[4],
        int* num,
        Zinc_location* loc,
        bool* done)
{
    enum Zinc_result r = Zinc_result_ok;

    if (data->loc.line == 0) {
        Zinc_input_unicode_string_clear(data);
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
        c[0] = ZINC_VECTOR_CHAR(data->text, data->pos++);
        *num = ZINC_NUM_BYTES(c[0]);
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
                c[i] = ZINC_VECTOR_CHAR(data->text, data->pos++);
                data->loc.start_pos++;
                if (!ZINC_IS_EXTRA_BYTE(c[i])) {
                    r = Zinc_set_error("invalid utf8 trailing byte");
                }
            } else {
                *done = true;
                r = Zinc_set_error("EOF before utf8 trailing byte(s)");
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
void Zinc_input_unicode_string_repeat(Zinc_input_unicode_string* data)
{
    data->repeat_char = true;
    data->loc = data->prev_loc;
}

/**
 * Seek to position in data stream. This call invalidates location data.
 * @param data the data
 * @param pos position to go to
 */
void Zinc_input_unicode_string_seek(Zinc_input_unicode_string* data, Zinc_location* loc)
{
    if (loc->start_pos < data->text->count)
    {
        Zinc_input_unicode_string_clear(data);
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
void Zinc_input_unicode_string_get_all(Zinc_input_unicode_string* data, Zinc_vector** text)
{
    Zinc_input_unicode_string_clear(data);
    data->pos = data->text->count;
    *text = data->text;
}

Zinc_location Zinc_input_unicode_string_get_location(Zinc_input_unicode_string* data)
{
    return data->loc;
}

void Zinc_input_unicode_string_destroy(Zinc_input_unicode_string* input)
{
    Zinc_vector_destroy(input->text);
    free(input->text);
}