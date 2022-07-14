#ifndef _BUFFER_H
#define _BUFFER_H

#include <stdio.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include "allocator.h"

#define BUFFER_CHUNK 64

struct buffer {
    size_t size;
    size_t buf_size;
    char* buf;
};

void buffer_init(struct buffer* bf);
enum result buffer_add_char(struct allocator* al, struct buffer* bf, char c);
void buffer_reset(struct buffer* bf);
void buffer_clear(struct buffer* bf);
enum result buffer_copy(struct allocator* al, struct buffer* a, struct buffer* b);
enum result buffer_copy_str(struct allocator* al, struct buffer* a, char* b);
enum result buffer2array(struct allocator* al, struct buffer* bf, char** array);
enum result array2buffer(struct allocator* al, char* a, struct buffer* bf);
enum result next_char(struct allocator* al, struct buffer* bf, size_t* pos, struct buffer* bf2);
int buffer_compare(struct buffer* a, struct buffer* b);
int buffer_str_compare(struct buffer* a, char* b);
enum result buffer_uslice(struct allocator* al, struct buffer* src, struct buffer* dest, size_t start, size_t end);

#endif
