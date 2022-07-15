#ifndef _INPUT_H
#define _INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "zinc/buffer.h"

typedef int (*input_getchar)(void*);
typedef int (*input_seek)(void*, size_t);
typedef void* input_data;

struct string_data {
	struct buffer* bf;
	int pos;
};

int file_getchar(FILE* fp);
int file_seek(FILE* fp, size_t pos);

void string_data_init(struct buffer* bf, struct string_data* sd);
int string_getchar(struct string_data* sd);
int string_seek(struct string_data* sd, size_t pos);

#endif
