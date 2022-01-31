#ifndef _USTRING_H
#define _USTRING_H

#include <stdio.h>

#define STRING_CHUNK 64

struct string {
    size_t size;
    size_t buf_size;
    char* buf;
};

enum result_enum next_line(FILE* f, struct string* s, int is_utf8, int* last_line);
void string_init(struct string* s);
enum result_enum string_add_char(struct string* s, char c);
void string_reset(struct string* s);
void string_clear(struct string* s);
enum result_enum string2array(struct string* s, char** array);
enum result_enum array2string(char* a, struct string* s);
int string_compare(struct string* a, struct string* b);

#endif
