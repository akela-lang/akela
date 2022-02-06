#ifndef _USTRING_H
#define _USTRING_H

#include <stdio.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>

#define STRING_CHUNK 64

struct string {
    size_t size;
    size_t buf_size;
    char* buf;
};

enum result next_line(FILE* f, struct string* s, int is_utf8, int* last_line);
void string_init(struct string* s);
enum result string_add_char(struct string* s, char c);
void string_reset(struct string* s);
void string_clear(struct string* s);
enum result string_copy(struct string* a, struct string* b);
enum result string2array(struct string* s, char** array);
enum result array2string(char* a, struct string* s);
int string_compare(struct string* a, struct string* b);
int str_compare(struct string* a, char* b);
enum result char2uchar(UConverter* conv, char* src, size_t src_size, UChar** dest, size_t dest_size, size_t* len);
enum result uchar2char(UConverter* conv, UChar* src, size_t src_size, char** dest, size_t dest_size, size_t* len);

#endif
