#ifndef _INPUT_H
#define _INPUT_H

#include <stdio.h>
#include <stdbool.h>
#include "buffer.h"

typedef int (*io_getchar)(void*);
typedef void* io_data;

struct input_state {
    io_getchar f;
    io_data d;
    UConverter* conv;
    int done;
    UChar32 uc;
    struct buffer bf;
    int has_next;
    bool last_was_newline;
    size_t line;
    size_t col;
    struct buffer current_line;
};

struct string_data {
	struct buffer* bf;
	int pos;
};

int file_getchar(FILE* fp);
void string_data_init(struct buffer* bf, struct string_data* sd);
int string_getchar(struct string_data* sd);
void input_state_init(io_getchar f, io_data d, UConverter* conv, struct input_state* is);
void input_state_push_uchar(struct input_state* is);
void input_state_pop_uchar(struct input_state* is);
enum result get_uchar(struct allocator* al, struct input_state* is);

#endif
