#ifndef _BUFFER_H
#define _BUFFER_H

#include <stdio.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include "allocator.h"
#include "io.h"

#define BUFFER_CHUNK 64

struct buffer {
    size_t size;
    size_t buf_size;
    char* buf;
};

void buffer_init(struct buffer* s);
enum result buffer_add_char(struct allocator* al, struct buffer* s, char c);
void buffer_reset(struct buffer* s);
void buffer_clear(struct buffer* s);
enum result buffer_copy(struct allocator* al, struct buffer* a, struct buffer* b);
enum result buffer2array(struct allocator* al, struct buffer* s, char** array);
enum result array2buffer(struct allocator* al, char* a, struct buffer* s);
enum result next_char(struct allocator* al, struct buffer* s, size_t* pos, struct buffer* s2);
int buffer_compare(struct buffer* a, struct buffer* b);
int str_compare(struct buffer* a, char* b);

#endif
