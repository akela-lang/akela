#ifndef _USTRING_H
#define _USTRING_H

#include <stdio.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include "allocator.h"
#include "io.h"

#define STRING_CHUNK 64

struct string {
    size_t size;
    size_t buf_size;
    char* buf;
};

enum result next_line(struct allocator* al, FILE* f, struct string* s, int is_utf8, int* last_line);
void string_init(struct string* s);
enum result string_add_char(struct allocator* al, struct string* s, char c);
void string_reset(struct string* s);
void string_clear(struct string* s);
enum result string_copy(struct allocator* al, struct string* a, struct string* b);
enum result string2array(struct allocator* al, struct string* s, char** array);
enum result array2string(struct allocator* al, char* a, struct string* s);
enum result next_char(struct allocator* al, struct string* s, size_t* pos, struct string* s2);
int string_compare(struct string* a, struct string* b);
int str_compare(struct string* a, char* b);

#endif
