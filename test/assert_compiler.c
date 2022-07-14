#include <stdlib.h>
#include "assert.h"
#include "alba/source.h"
#include "alba/uconv.h"

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

void expect_utf32_char(UChar32 a, UChar32 b, char* message)
{
	test_called();
	if (a == b) return;
	error_triggered();
	printf("(%d:%c) = (%d:%c) utf32 error: %s\n", a, a, b, b, message);
}

void expect_compile_error_message(struct compile_error_list* el, char* s, size_t line, size_t col, size_t byte_pos)
{
	bool found = false;
	bool has_error = false;
	test_called();
	for (struct compile_error* p = el->head; p; p = p->next) {
		if (strcmp(p->message, s) == 0) {
			found = true;
			if (p->line != line) {
				set_error("(%uv) = (%uv) line error", p->line, line);
				has_error = true;
			} else if (p->col != col) {
				set_error("(%uv) = (%uv) col error", p->col, col);
				has_error = true;
			} else if (p->byte_pos != byte_pos) {
				set_error("(%uv) = (%uv) col error", p->byte_pos, byte_pos);
				has_error = true;
			}
			break;
		}
	}
	if (found && !has_error) return;
	error_triggered();
	if (has_error) {
		printf("%s\n", error_message);
	} else if (!found) {
		printf("(%s): no matching compile error message\n", s);
	}
}
