#ifndef _IO_H
#define _IO_H

#include <stdio.h>
#include "ustring.h"

typedef int (*io_getchar)(void*);
typedef void* io_data;

struct input_state {
    io_getchar f;
    io_data d;
    UConverter* conv;
    int done;
    UChar32 uc;
    struct string s;
    int has_next;
    UChar32 next_uc;
    struct string next_s;
};

struct string_data {
	struct string* s;
	int pos;
};

int file_getchar(FILE* fp);
void string_data_init(struct string* s, struct string_data* sd);
int string_getchar(struct string_data* sd);
void input_state_init(io_getchar f, io_data d, UConverter* conv, struct input_state* is);
void input_state_push_uchar(struct input_state* is);
void input_state_pop_uchar(struct input_state* is);
enum result get_uchar(struct allocator* al, struct input_state* is);

#endif
