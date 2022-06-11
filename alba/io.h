#ifndef _IO_H
#define _IO_H

#include <stdio.h>
#include "ustring.h"

struct string_data {
	struct string* s;
	int pos;
};

typedef int (*io_getchar)(void*);
typedef void* io_data;

int file_getchar(FILE* fp);
void string_data_init(struct string* s, struct string_data* sd);
int string_getchar(struct string_data* sd);

#endif
