#include <stdio.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdbool.h>
#include "buffer.h"
#include "input.h"
#include "result.h"
#include "uconv.h"

int file_getchar(FILE* fp)
{
	return getc(fp);
}

void string_data_init(struct buffer* bf, struct string_data* sd)
{
	sd->bf = bf;
	sd->pos = 0;
}

int string_getchar(struct string_data* sd)
{
	if (sd->pos >= 0 && sd->pos < sd->bf->size) {
		return sd->bf->buf[sd->pos++];
	}

	return EOF;
}

void input_state_init(io_getchar f, io_data d, UConverter* conv, struct input_state* is)
{
    is->f = f;
    is->d = d;
    is->conv = conv;
    is->done = 0;
    buffer_init(&is->bf);
    is->has_next = 0;
    is->last_was_newline = false;
    is->line = 1;
    is->col = 0;
    buffer_init(&is->current_line);
}

enum result input_state_save_char(struct allocator *al, struct input_state* is, char c)
{
    enum result r = result_ok;

    if (is->col == 0) {
        buffer_clear(&is->current_line);
    }

    r = buffer_add_char(al, &is->current_line, c);
    if (r == result_error) {
        return r;
    }

    return r;
}

void input_state_push_uchar(struct input_state* is)
{
    is->has_next = 1;
}

void input_state_pop_uchar(struct input_state* is)
{
    is->has_next = 0;
}

enum result get_uchar(struct allocator* al, struct input_state* is)
{
    int count;
    enum result r = result_ok;
    int c;

    if (is->has_next) {
        input_state_pop_uchar(is);
        return r;
    }

    if (is->last_was_newline) {
        is->line++;
        is->col = 0;
        is->last_was_newline = false;
    }

    buffer_clear(&is->bf);
    is->done = 0;

    c = is->f(is->d);
    if (c == EOF) {
        is->done = 1;
        is->uc = EOF;
        buffer_clear(&is->bf);
        is->col++;
        return r;
    }

    r = check_num_bytes(c, &count);
    if (r == result_error) {
        return r;
    }

    r = buffer_add_char(al, &is->bf, c);
    if (r == result_error) {
        return r;
    }

    r = input_state_save_char(al, is, c);
    if (r == result_error) {
        return r;
    }

    for (int i = 1; i < count; i++) {
        c = is->f(is->d);
        if (c == EOF) {
            is->done = 1;
            return r;
        }

        r = check_extra_byte(c);
        if (r == result_error) {
            return r;
        }

        r = buffer_add_char(al, &is->bf, c);
        if (r == result_error) {
            return r;
        }

        r = input_state_save_char(al, is, c);
        if (r == result_error) {
            return r;
        }
    }

    UChar* dest;
    size_t dest_len;
    r = char2uchar(al, is->conv, is->bf.buf, is->bf.size, &dest, is->bf.size, &dest_len);
    if (r == result_error) {
        return r;
    }

    size_t pos = 0;
    U16_NEXT(dest, pos, dest_len, is->uc);

    is->col++;
    if (is->uc == '\n') {
        is->last_was_newline = true;
    }

    return r;
}

