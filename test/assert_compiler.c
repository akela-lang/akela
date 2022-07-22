#include <stdlib.h>
#include <string.h>
#include "zinc/assert.h"
#include "alba/source.h"
#include "alba/uconv.h"
#include "zinc/result.h"

/* static-output */
void expect_utf8_char(char* a, char* b, char* message)
{
	test_called();
	int count_a = NUM_BYTES(a[0]);
	int count_b = NUM_BYTES(b[0]);
	if (count_a == count_b) {
		for (int i = 0; i < count_a; i++) {
			if (a[i] != b[i]) {
				break;
			}
		}
		return;
	}
	error_triggered();
	printf("utf8 chars not equal: %s\n", message);
}

/* static-output */
void expect_utf32_char(UChar32 a, UChar32 b, char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	printf("(%d:%c) = (%d:%c) utf32 error: %s\n", a, a, b, b, message);
}

/* static-output */
struct compile_error* assert_compile_error(struct compile_error_list* el, char* s)
{
	test_called();
	for (struct compile_error* p = el->head; p; p = p->next) {
		if (strcmp(p->message, s) == 0) {
			return p;
		}
	}

	error_triggered();
	printf("(%s): no matching compile error message\n", s);
	panic();
	return NULL;
}

/* static-output */
void expect_compile_error_fields(struct compile_error* e, size_t line, size_t col, size_t byte_pos)
{
	bool has_error = 0;
	test_called();

	if (e->line != line) {
		set_error("(%zu) = (%zu) line error", e->line, line);
		has_error = true;
	} else if (e->col != col) {
		set_error("(%zu) = (%zu) col error", e->col, col);
		has_error = true;
	} else if (e->byte_pos != byte_pos) {
		set_error("(%zu) = (%zu) col error", e->byte_pos, byte_pos);
		has_error = true;
	}

	if (!has_error) return;

	error_triggered();
	printf("%s\n", error_message);
}
