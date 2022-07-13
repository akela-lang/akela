#include <stdio.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdbool.h>
#include "buffer.h"
#include "input.h"
#include "result.h"
#include "uconv.h"

int file_getchar(FILE* fp)
{
	return getc(fp);
}

void string_data_init(struct buffer* bf, struct string_data* sd)
{
	sd->bf = bf;
	sd->pos = 0;
}

int string_getchar(struct string_data* sd)
{
	if (sd->pos >= 0 && sd->pos < sd->bf->size) {
		return sd->bf->buf[sd->pos++];
	}

	return EOF;
}

