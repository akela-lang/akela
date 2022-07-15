#include <stdio.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdbool.h>
#include "zinc/buffer.h"
#include "input.h"
#include "zinc/result.h"
#include "uconv.h"

/* static-output */
/* resource-input fp */
/* resource-use fp */
int file_getchar(FILE* fp)
{
	return getc(fp);
}

/* static-output */
/* resource-input fp */
/* resource-use fp */
int file_seek(FILE* fp, size_t pos)
{
	return fseek(fp, pos, SEEK_SET);
}

/*  static-output */
void string_data_init(struct buffer* bf, struct string_data* sd)
{
	sd->bf = bf;
	sd->pos = 0;
}

/* static-output */
int string_getchar(struct string_data* sd)
{
	if (sd->pos >= 0 && sd->pos < sd->bf->size) {
		return sd->bf->buf[sd->pos++];
	}

	return EOF;
}

/* static-output */
int string_seek(struct string_data* sd, size_t pos)
{
	if (pos >= sd->bf->size) return -1;
	sd->pos = pos;
	return 0;
}
