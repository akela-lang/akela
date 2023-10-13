#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include "zinc/result.h"
#include "zinc/buffer.h"
#include "token.h"
#include "input.h"
#include "lookahead_char.h"
#include "source.h"
#include "zinc/memory.h"

/* dynamic-output bf{} */
/* resource-input d */
/* resource-use d */
enum result format_error(struct error* e, input_getchar f, input_seek seek, input_data d, struct buffer* bf)
{
	/* allocate bf{} */
	buffer_copy_str(bf, e->message);
	
	/* allocate bf{} */
	buffer_add_char(bf, '\n');

	/* use d */
	int err = seek(d, e->loc.byte_pos);
	if (err) return set_error("seek error: seek %zu", e->loc.byte_pos);

	int c;
	while ((c = f(d))) {
		if (c == EOF) break;
		if (c == '\n') break;

		/* allocate bf{} */
		buffer_add_char(bf, c);
	}

	return result_ok;
}
