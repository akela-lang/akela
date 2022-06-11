#include <stdio.h>
#include "ustring.h"
#include "io.h"

int file_getchar(FILE* fp)
{
	return getc(fp);
}

void string_data_init(struct string* s, struct string_data* sd)
{
	sd->s = s;
	sd->pos = 0;
}

int string_getchar(struct string_data* sd)
{
	if (sd->pos >= 0 && sd->pos < sd->s->size) {
		return sd->s->buf[sd->pos++];
	}

	return EOF;
}
