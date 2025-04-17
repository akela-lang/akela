#include "input_unicode_file.h"
#include "memory.h"
#include <stddef.h>
#include "vector.h"
#include "utf8.h"
#include <string.h>
#include "input_bounds.h"

Zinc_input_unicode_vtable Zinc_input_unicode_file_vtable = {
    .loc_offset = offsetof(Zinc_input_unicode_file, loc),
    .next_offset = offsetof(Zinc_input_unicode_file, Next),
    .repeat_offset = offsetof(Zinc_input_unicode_file, Repeat),
    .seek_offset = offsetof(Zinc_input_unicode_file, Seek),
    .get_all_offset = offsetof(Zinc_input_unicode_file, GetAll),
    .get_location_offset = offsetof(Zinc_input_unicode_file, GetLocation),
    .destroy_offset = offsetof(Zinc_input_unicode_file, Destroy),
    .set_bounds_offset = offsetof(Zinc_input_unicode_file, SetBounds),
};

void Zinc_input_unicode_file_init(Zinc_input_unicode_file* input, FILE* fp)
{
    Zinc_location_init(&input->loc);
    input->repeat_char = false;
    memset(input->prev_c, 0, 4);
    input->prev_num = 0;
    Zinc_location_init(&input->prev_loc);
    input->prev_done = false;
    input->fp = fp;
    Zinc_input_bounds_init(&input->bounds);
    input->has_bounds = false;
    input->Next = (Zinc_input_unicode_next_interface) Zinc_input_unicode_file_next;
    input->Repeat = (Zinc_input_unicode_repeat_interface) Zinc_input_unicode_file_repeat;
    input->Seek = (Zinc_input_unicode_seek_interface) Zinc_input_unicode_file_seek;
    input->GetAll = (Zinc_input_unicode_get_all_interface) Zinc_input_unicode_file_get_all;
    input->GetLocation =
        (Zinc_input_unicode_get_location_interface) Zinc_input_unicode_file_get_location;
    input->Destroy = (Zinc_input_unicode_destroy_interface) Zinc_input_unicode_file_destroy;
    input->SetBounds = (Zinc_input_unicode_set_bounds_interface) Zinc_input_unicode_file_set_bounds;
    input->vtable = &Zinc_input_unicode_file_vtable;
}

void Zinc_input_unicode_file_create(Zinc_input_unicode_file** input, FILE* fp)
{
    Zinc_malloc_safe((void**)input, sizeof(Zinc_input_unicode_file));
    Zinc_input_unicode_file_init(*input, fp);
}

void Zinc_input_unicode_file_clear(Zinc_input_unicode_file* input)
{
    Zinc_location_init(&input->loc);
    Zinc_location_init(&input->prev_loc);
    input->loc.line = 1;
    input->loc.col = 1;
}

/**
 * Get the next character.
 * @param input input object
 * @param c the next char
 * @param num the number of bytes
 * @param done true if input is done
 * @return result_ok if successful, otherwise result_error
 */
enum Zinc_result Zinc_input_unicode_file_next(
        Zinc_input_unicode_file* input,
        char c[4],
        int* num,
        Zinc_location* loc,
        bool* done)
{
    enum Zinc_result r = Zinc_result_ok;
    *done = false;
    if (input->loc.start_pos == 0) {
        Zinc_input_unicode_file_clear(input);
    }

    if (input->has_bounds && input->loc.start_pos >= input->bounds.end) {
        *num = 0;
        *done = true;
        *loc = input->loc;
        return Zinc_result_ok;
    }

    if (input->repeat_char) {
        input->repeat_char = false;
        memcpy(c, input->prev_c, 4);
        *num = input->prev_num;
        *loc = input->prev_loc;
        *done = input->prev_done;
        input->loc = input->prev_loc;
    } else {
        int x = getc(input->fp);
        *loc = input->loc;
        if (x == EOF) {
            input->loc.end_pos = input->loc.start_pos + 1;
            (*loc).end_pos = (*loc).start_pos + 1;
            *num = 0;
            *done = true;
            input->prev_num = *num;
            input->prev_done = *done;
            return Zinc_result_ok;
        }

        c[0] = (char)x;
        *num = ZINC_NUM_BYTES(c[0]);
        for (int i = 1; i < *num; i++) {
            x = getc(input->fp);
            if (x == EOF) {
                r = Zinc_set_error("EOF before last utf8 trailing byte");
                *num = i;
                *done = true;
                break;
            }
            c[i] = (char)x;
            if (!ZINC_IS_EXTRA_BYTE(c[i])) {
                r = Zinc_set_error("utf8 trailing byte invalid");
            }
        }

        loc->end_pos = loc->start_pos + *num;
        input->loc = *loc;

        memcpy(input->prev_c, c, *num);
        input->prev_num = *num;
        input->prev_loc = input->loc;
        input->prev_done = *done;
    }

    input->loc.start_pos += *num;
    if (*num == 1 && c[0] == '\n') {
        input->loc.line++;
        input->loc.col = 1;
    } else {
        input->loc.col++;
    }
    return r;
}

/**
 * Repeat the previous character.
 * @param input lexer data
 */
void Zinc_input_unicode_file_repeat(Zinc_input_unicode_file* input)
{
    input->repeat_char = true;
}

/**
 * Seek to position in data stream. This call invalidates location data.
 * @param input the input
 * @param pos position to go to
 */
void Zinc_input_unicode_file_seek(Zinc_input_unicode_file* input, Zinc_location* loc)
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
void Zinc_input_unicode_file_get_all(Zinc_input_unicode_file* input, Zinc_vector** text)
{
    Zinc_input_unicode_file_clear(input);
    Zinc_vector* v = NULL;
    Zinc_vector_create(&v, sizeof(char));
    fseek(input->fp, 0, SEEK_SET);
    int x;
    int num;
    char c[4];
    do {
        x = getc(input->fp);
        if (x == EOF) break;
        c[0] = (char)x;
        num = ZINC_NUM_BYTES(c[0]);
        for (int i = 1; i < num; i++) {
            x = getc(input->fp);
            c[i] = (char)x;
        }
        Zinc_vector_add(v, c, num);
    } while (true);
    Zinc_vector_add_null(v);
    *text = v;
}

Zinc_location Zinc_input_unicode_file_get_location(Zinc_input_unicode_file* input)
{
    return input->loc;
}

void Zinc_input_unicode_file_destroy(Zinc_input_unicode_file* input)
{
    fclose(input->fp);
}

void Zinc_input_unicode_file_set_bounds(Zinc_input_unicode_file* input, Zinc_input_bounds* bounds)
{
    input->has_bounds = true;
    input->bounds = *bounds;
    input->loc = input->bounds.loc;
    input->prev_loc = input->loc;
    Zinc_input_unicode_file_seek(input, &input->loc);
}