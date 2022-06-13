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
enum result char2uchar(struct allocator* al, UConverter* conv, char* src, size_t src_size, UChar** dest, size_t dest_size, size_t* len);
enum result uchar2char(struct allocator* al, UConverter* conv, UChar* src, size_t src_size, char** dest, size_t dest_size, size_t* len);
enum result conv_open(UConverter** conv);
void conv_close(UConverter* conv);
void input_state_init(io_getchar f, io_data d, UConverter* conv, struct input_state* is);
void input_state_push_uchar(struct input_state* is);
void input_state_pop_uchar(struct input_state* is);
enum result get_uchar(struct allocator* al, struct input_state* is);

#endif
