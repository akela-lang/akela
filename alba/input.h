#ifndef _INPUT_H
#define _INPUT_H

#include <stdio.h>
#include <stdbool.h>
#include "buffer.h"

typedef int (*input_getchar)(void*);
typedef void* input_data;

struct input_state {
    input_getchar f;
    input_data d;
    UConverter* conv;
    int done;
    int has_next;
    bool last_was_newline;
    struct buffer current_line;
    UChar32 uc;
    struct buffer bf;
    size_t line;
    size_t col;
};

struct string_data {
	struct buffer* bf;
	int pos;
};

int file_getchar(FILE* fp);
void string_data_init(struct buffer* bf, struct string_data* sd);
int string_getchar(struct string_data* sd);
void input_state_init(input_getchar f, input_data d, UConverter* conv, struct input_state* is);
void input_state_push_uchar(struct input_state* is);
void input_state_pop_uchar(struct input_state* is);
enum result get_uchar(struct allocator* al, struct input_state* is);

#endif
