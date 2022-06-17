#include <stdio.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
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
    buffer_init(&is->next_s);
}

void input_state_push_uchar(struct input_state* is)
{
    is->next_s = is->bf;
    is->next_uc = is->uc;
    is->has_next = 1;
}

void input_state_pop_uchar(struct input_state* is)
{
    is->bf = is->next_s;
    is->uc = is->next_uc;
    is->has_next = 0;
}

enum result next_line(struct allocator* al, FILE* f, struct buffer* bf, int is_utf8, int* last_line)
{
    *last_line = 0;

    int i = 0;
    int count;
    enum result r;
    while (1) {
        int c = getc(f);

        if (c == EOF) {
            *last_line = 1;
            break;
        }

        if (c == '\n' || c == '\r') {
            break;
        }

        if (is_utf8) {
            r = num_bytes(c, &count);
            if (r == result_error) {
                return r;
            }
            r = buffer_add_char(al, bf, c);
            if (r == result_error) {
                return r;
            }

            for (int j = 1; j < count; j++) {
                c = getc(f);
                r = check_extra_byte(c);
                if (r == result_error) {
                    return r;
                }
                r = buffer_add_char(al, bf, c);
                if (r == result_error) {
                    return r;
                }
            }
        } else {
            r = buffer_add_char(al, bf, c);
            if (r == result_error) {
                return r;
            }
        }
    }

    return result_ok;
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

    buffer_clear(&is->bf);
    is->done = 0;

    c = is->f(is->d);
    if (c == EOF) {
        is->done = 1;
        return r;
    }

    r = num_bytes(c, &count);
    if (r == result_error) {
        return r;
    }

    r = buffer_add_char(al, &is->bf, c);
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
    }

    UChar* dest;
    size_t dest_len;
    r = char2uchar(al, is->conv, is->bf.buf, is->bf.size, &dest, is->bf.size, &dest_len);
    if (r == result_error) {
        return r;
    }

    size_t pos = 0;
    U16_NEXT(dest, pos, dest_len, is->uc);

    return r;
}